#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>

struct Token
{
    std::string type; 
    std::string text;
    int level {};
};

std::vector<Token> tokenizeLine(const std::string& line) {
    std::vector<Token> tokens;
    int i = 0;

    while (i < (int)line.size()) {

        int numSpace = 0;
        while (i < (int)line.size() && line[i] == ' ') {
            numSpace++;
            i++;
        }

        // Tag
        if (i < (int)line.size() && line[i] == '<') {
            Token token;
            token.level = numSpace / 4;
            token.type = "tag";
            i++;

            while (i < (int)line.size() && line[i] != '>') {
                if (line[i] == '<') break;
                token.text += line[i++];
            }
            if (i < (int)line.size() && line[i] == '>') i++;

            tokens.push_back(token);
        }

        // Value
        else if (!isspace(line[i])) {
            Token token;
            token.type = "value";

            while (i < (int)line.size() && line[i] != '<') {
                token.text += line[i++];
            }

            tokens.push_back(token);
        }

        else {
            i++;
        }
    }
    return tokens;
}


std::string readFileToString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool isValidChar(const char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    if (c >= '0' && c <= '9') return true;
    if (c == '-' || c == '_' || c == '.' || c == ':') return true;
    return false;
}

bool isValidFirstChar(const char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool verifyImproved(const std::string& tokens)
{
    std::stack<std::string> st;
    int i = 0;

    while (i < (int)tokens.size()) {

        char c = tokens[i];

        if (c == '<') {
            i++;
            if (i >= tokens.size()) return false;

            bool isClosing = false;

            if (tokens[i] == '/') {
                isClosing = true;
                i++;
                if (st.empty()) return false;
            }

            if (i >= tokens.size() || !isValidFirstChar(tokens[i])) return false;

            std::string tag = "";
            while (i < tokens.size() &&
                   tokens[i] != '>' && tokens[i] != '/' &&
                   tokens[i] != ' ' && tokens[i] != '\n') {

                if (!isValidChar(tokens[i])) return false;
                tag += tokens[i++];
            }

            bool isSelfClosing = false;

            if (isClosing) {
                while (i < tokens.size() && (tokens[i] == ' ' || tokens[i] == '\n')) i++;
                if (i >= tokens.size() || tokens[i] != '>') return false;
            }
            else {
                while (i < tokens.size() && tokens[i] != '>') {
                    if (tokens[i] == '/' && i + 1 < tokens.size() && tokens[i + 1] == '>') {
                        isSelfClosing = true;
                        i++;
                        break;
                    }
                    i++;
                }
            }

            if (i >= tokens.size() || tokens[i] != '>') return false;

            if (isClosing) {
                if (st.top() == tag) st.pop();
                else return false;
            }
            else if (!isSelfClosing) {
                st.push(tag);
            }

            i++;
        }

        else if (c == '\n' || c == ' ') {
            i++;
        }

        else if (c == '>') {
            return false;
        }

        else {
            while (i < tokens.size() && tokens[i] != '<') i++;
        }
    }

    return st.empty();
}


bool isClosingTag(const std::string &tag)
{
    return !tag.empty() && tag[0] == '/';
}

std::string getTagName(const std::string &tagText)
{
    int i = 0;
    if (tagText[i] == '/') i++;

    std::string tagName = "";
    while (i < tagText.length() &&
           tagText[i] != ' ' && tagText[i] != '/' && tagText[i] != '>') {
        tagName += tagText[i++];
    }
    return tagName;
}



struct ErrorInfo
{
    int count {};
    std::vector<int> lines;
    std::vector<std::string> descriptions;
    std::vector<std::string> types;
    std::vector<std::string> tags;
    std::vector<int> levels;

    void addError(int line, int level,
                  const std::string& tag,
                  const std::string& type,
                  const std::string& desc)
    {
        count++;
        lines.push_back(line);
        levels.push_back(level);
        tags.push_back(tag);
        types.push_back(type);
        descriptions.push_back(desc);
    }
};



ErrorInfo countErrorSummary(const std::string &content)
{
    ErrorInfo errors;
    std::string line;
    std::istringstream stream(content);
    std::stack<Token> tagStack;
    int lineNum = 1;

    while (std::getline(stream, line)) {

        std::vector<Token> tokens = tokenizeLine(line);

        for (auto &token : tokens)
        {
            if (token.type == "tag")
            {
                if (isClosingTag(token.text))
                {
                    if (tagStack.empty() || tagStack.top().text != getTagName(token.text))
                    {
                        std::string cause = tagStack.top().text;
                        int level = tagStack.top().level;
                        tagStack.pop();

                        if (!tagStack.empty() && tagStack.top().text == getTagName(token.text))
                        {
                            std::string msg = "The missing closing for <" + cause + ">";
                            errors.addError(lineNum - 1, level, cause, "Missing", msg);
                            tagStack.pop();
                        }
                        else
                        {
                            std::string msg = "The mismatching closing for <" + cause + ">";
                            errors.addError(lineNum, level, cause, "Mismatched", msg);
                        }
                    }
                    else tagStack.pop();
                }
                else
                {
                    if (!tagStack.empty() && tagStack.top().level == token.level)
                    {
                        std::string msg = "The missing closing for <" +
                                     tagStack.top().text + ">";
                        errors.addError(lineNum - 1,
                                        tagStack.top().level,
                                        tagStack.top().text,
                                        "Missing",
                                        msg);
                        tagStack.pop();
                    }

                    tagStack.push(token);
                }
            }
        }
        lineNum++;
    }

    while (!tagStack.empty())
    {
        std::string msg = "The missing closing for <" + tagStack.top().text + ">";
        errors.addError(lineNum,
                        tagStack.top().level,
                        tagStack.top().text,
                        "Missing",
                        msg);
        tagStack.pop();
    }

    return errors;
}



void highlightErrors(const std::string &content,const ErrorInfo &errors)
{
    std::istringstream stream(content);
    std::string line;
    int i = 0;

    for (int lineNum = 1; std::getline(stream, line); lineNum++) {

        if (i < errors.count && lineNum == errors.lines[i]) {
            std::cout << ">> ERROR: "
                 << errors.descriptions[i]
                 << " line: " << line << "\n";
            i++;
        }
        else {
            std::cout << line << "\n";
        }
    }

    while (i < errors.count) {
        std::cout << ">> ERROR: " << errors.descriptions[i] << "\n";
        i++;
    }
}


// ======================= TEST SUITE (Unchanged) =======================

void runTest(const std::string& name, const std::string& input, bool expected) {
    bool actual = verifyImproved(input);
    std::cout << "  - " << name << ": ";
    if (actual == expected) std::cout << "[PASS]";
    else std::cout << "[FAIL] (Expected " << expected << ", Got " << actual << ")";
    std::cout << " -> Input: \"" << input << "\"\n";
}

void testValidCases() {
    std::cout << "\n--- Valid Cases ---\n";
    runTest("Simple Nesting", "<root><child></child></root>", true);
    runTest("Empty Content", "<tag></tag>", true);
    runTest("Whitespace", "\n<t>\n <a></a> </t> ", true);
    runTest("Attributes", "<doc id=\"1\"><item name='a'></item></doc>", true);
    runTest("Self Closing", "<root><br/></root>", true);
}

void testInvalidNesting() {
    std::cout << "\n--- Invalid Nesting ---\n";
    runTest("Mismatched", "<A><B></A></B>", false);
    runTest("Unclosed", "<root><child>", false);
}

void testInvalidStructure() {
    std::cout << "\n--- Invalid Structure ---\n";
    runTest("Stray >", ">>><root></root>", false);
    runTest("Invalid Char", "<tag!name></tag!name>", false);
}
// ======================= MAIN =======================

int main()
{
    std::string xmlContent = readFileToString("sample.xml");


    std::cout << xmlContent << "\n";
    std::cout << "------------------\n";
    std::cout << verifyImproved(xmlContent) << "\n";
     std::cout << "------------------\n";                            

    ErrorInfo errors = countErrorSummary(xmlContent);
    std::cout<<"num of errors= " << errors.count << "\n";


    for (auto &e : errors.lines) std::cout << e << "\n";

    std::cout << "\n\n";

    highlightErrors(xmlContent, errors);

    std::cout << "Starting Test Suite\n";
    testValidCases();
    testInvalidNesting();
    testInvalidStructure();
    std::cout << "\n--- Done ---\n";
}
