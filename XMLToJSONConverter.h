#ifndef XMLTOJSONCONVERTER_H
#define XMLTOJSONCONVERTER_H

#include "XMLNode.h"
#include <string>
#include <vector>

using namespace std;

class XMLToJSONConverter {
private:
    bool prettyPrint;
    bool autoDetectTypes;
    string rootName;

    string escapeJSONString(const string& input);
    string makeIndent(int depth);

    bool isNumeric(const string& str);
    bool isBoolean(const string& str);
    bool isNull(const string& str);

    bool shouldBeArray(const vector<XMLNode*>& children);
    string convertArray(const vector<XMLNode*>& children, int depth);

    string convertNode(XMLNode* node, int depth);
    string convertChildren(XMLNode* node, int depth);
    string convertTextContent(XMLNode* node);

public:
    XMLToJSONConverter();
    XMLToJSONConverter(bool pretty, bool detectTypes = false);

    string convert(XMLNode* xmlRoot);

    void setPrettyPrint(bool enabled);
    void setAutoDetectTypes(bool enabled);
    void setRootName(const string& name);
    void XMLToJSON(string& inputfile);
};

#endif // XMLTOJSONCONVERTER_H
