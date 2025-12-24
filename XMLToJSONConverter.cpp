/*
* Example Usage:
* 	XMLToJSONConverter converter;
	string inputfile = "input_path";
	converter.XMLToJSON(inputfile);
* the output will be saved in output.json file
*/

#include "comp_decomp.h" 
#include "XMLToJSONConverter.h"
#include <cctype>
#include <algorithm>
#include <cmath>
#include <map>
#include "XMLParser.h"
#include "prettify.h"
using namespace std;

XMLToJSONConverter::XMLToJSONConverter()
    : prettyPrint(true), autoDetectTypes(false), rootName("") {
}

XMLToJSONConverter::XMLToJSONConverter(bool pretty, bool detectTypes)
    : prettyPrint(pretty), autoDetectTypes(detectTypes), rootName("") {
}

void XMLToJSONConverter::setPrettyPrint(bool enabled) {
    prettyPrint = enabled;
}

void XMLToJSONConverter::setAutoDetectTypes(bool enabled) {
    autoDetectTypes = enabled;
}

void XMLToJSONConverter::setRootName(const string& name) {
    rootName = name;
}

string XMLToJSONConverter::escapeJSONString(const string& input) {
    string output;
    for (char c : input) {
        switch (c) {
        case '\"': output += "\\\""; break;
        case '\\': output += "\\\\"; break;
        case '/':  output += "\\/";  break;
        case '\b': output += "\\b";  break;
        case '\f': output += "\\f";  break;
        case '\n': output += "\\n";  break;
        case '\r': output += "\\r";  break;
        case '\t': output += "\\t";  break;
        default:   output += c;      break;
        }
    }
    return output;
}

string XMLToJSONConverter::makeIndent(int depth) {
    return (!prettyPrint || depth <= 0) ? "" : string(depth * 2, ' ');
}

bool XMLToJSONConverter::isNumeric(const string& str) {
    if (str.empty()) return false;

    size_t start = (str[0] == '-') ? 1 : 0;
    bool hasDecimal = false, hasDigits = false;

    for (size_t i = start; i < str.length(); i++) {
        char c = str[i];
        if (isdigit(c)) hasDigits = true;
        else if (c == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        }
        else return false;
    }
    return hasDigits;
}

bool XMLToJSONConverter::isBoolean(const string& str) {
    return str == "true" || str == "false";
}

bool XMLToJSONConverter::isNull(const string& str) {
    return str == "null";
}

bool XMLToJSONConverter::shouldBeArray(const string& parentName, const vector<XMLNode*>& children) {
    if (children.empty()) return false;

    // 1. Uniformity Check: All children must have the same tag name
    string firstName = children[0]->getName();
    for (size_t i = 1; i < children.size(); i++) {
        if (children[i]->getName() != firstName) return false;
    }

    // 2. If there are multiple children, it is definitely an Array.
    if (children.size() > 1) return true;

    // 3. Handling Single Child Cases (The tricky part)
    XMLNode* child = children[0];

    // Case A: The child is a complex object (has its own children).
    // Example: <followers><follower>...</follower></followers>
    // Result: Array of Objects [ { ... } ]
    if (child->hasChildren()) return true;

    // Case B: The child is simple text, BUT the names imply a list.
    // Example: <topics><topic>education</topic></topics>
    // Logic: "topics" contains "topic" -> TRUE. Result: Array of Strings ["education"]
    // Contrast: <follower><id>1</id> -> "follower" doesn't contain "id" -> FALSE. Result: Object {"id": "1"}
    if (parentName.find(child->getName()) != string::npos) {
        return true;
    }

    return false;
}
string XMLToJSONConverter::convertNode(XMLNode* node, int depth) {
    if (node->hasText() && !node->hasChildren())
        return convertTextContent(node);

    if (node->hasChildren()) {
        const auto& children = node->getChildren();

        // FIX: Pass the current node's name as the "Parent Name"
        if (shouldBeArray(node->getName(), children)) {
            return convertArray(children, depth);
        }

        string result = "{\n";
        result += convertChildren(node, depth);
        result += makeIndent(depth) + "}"; 
        return result;
    }

    return "\"\"";
}

string XMLToJSONConverter::convertChildren(XMLNode* node, int depth) {
    string result;
    string childIndent = makeIndent(depth + 1);

    const auto& children = node->getChildren();
    map<string, vector<XMLNode*>> groups;

    for (XMLNode* child : children)
        groups[child->getName()].push_back(child);

    size_t count = 0;
    for (const auto& g : groups) {
        result += childIndent;
        result += "\"" + escapeJSONString(g.first) + "\": ";

        if (g.second.size() == 1) {
            XMLNode* singleChild = g.second[0];
            
            // FIX: Pass singleChild->getName() as the parent name here
            if (singleChild->hasChildren() && shouldBeArray(singleChild->getName(), singleChild->getChildren())) {
                result += convertArray(singleChild->getChildren(), depth + 1);
            } else {
                result += convertNode(singleChild, depth + 1);
            }
        } else {
            result += convertArray(g.second, depth + 1);
        }

        if (++count < groups.size())
            result += ",";
        result += "\n";
    }

    return result;
}

string XMLToJSONConverter::convertArray(const vector<XMLNode*>& children, int depth) {
    string result;
    string indent = makeIndent(depth);          // Indentation of the array brackets []
    string childIndent = makeIndent(depth + 1); // Indentation of items inside the array

    result += "[\n";

    for (size_t i = 0; i < children.size(); i++) {
        XMLNode* child = children[i];
        result += childIndent;

        if (child->hasText() && !child->hasChildren()) {
            result += convertTextContent(child);
        } else {
            // FIX: Pass depth + 1 here. 
            // This ensures the Object inside the array ({...}) treats 'childIndent' as its baseline.
            result += convertNode(child, depth + 1);
        }

        if (i < children.size() - 1)
            result += ",";
        result += "\n";
    }

    result += indent + "]";
    return result;
}
string XMLToJSONConverter::convertTextContent(XMLNode* node) {
    string content = node->getContent();
    if (content.empty()) return "\"\"";

    if (autoDetectTypes) {
        if (isNumeric(content)) return content;
        if (isBoolean(content)) return content;
        if (isNull(content))    return "null";
    }
    return "\"" + escapeJSONString(content) + "\"";
}



string XMLToJSONConverter::convert(XMLNode* xmlRoot) {
    if (!xmlRoot) return "{}";

    string root = rootName.empty() ? xmlRoot->getName() : rootName;
    string result;

    result += "{\n";
    result += "  \"" + escapeJSONString(root) + "\": ";
    result += convertNode(xmlRoot, 2);
    result += "\n}";

    return result;
}

void XMLToJSONConverter::XMLToJSON(string& inputfile) {
    // 1. Read XML file
    string xmlContent = readFile(inputfile);
    // 2. Parse XML
    XMLParser parser;
    XMLNode* xmlTree = parser.parse(xmlContent);

    // 3. Convert to JSON
    XMLToJSONConverter converter(true, true) ;  // prettyPrint = true, autoDetectTypes = true
    string jsonOutput = converter.convert(xmlTree);
    // 4. Write JSON file
    writeFile("output.json", jsonOutput);
    // Cleanup XML tree
    delete xmlTree;
}
