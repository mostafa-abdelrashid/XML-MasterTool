#include <string>
#include <vector>
#include <stack>
#include <sstream>

using namespace std;

string fixErrors(const string& content, ErrorInfo& errors) {
    istringstream stream(content);
    string line;
    stack<Token> tagStack;
    vector<string> fixedLines;
    int lineNum = 1;

    while (getline(stream, line)) {
        vector<Token> tokens = tokenizeLine(line);
        string fixedLine = "";

        for (auto& token : tokens) {
            if (token.type == "tag") {
                if (isClosingTag(token.text)) {
                    string tagName = getTagName(token.text);

                    if (!tagStack.empty() && tagStack.top().text == tagName) {
                        tagStack.pop();
                        fixedLine += "<" + token.text + ">";
                    }
                    else {
                        while (!tagStack.empty() && tagStack.top().text != tagName) {
                            Token topToken = tagStack.top();
                            tagStack.pop();
                            fixedLine += "</" + topToken.text + ">";
                            errors.addError(lineNum, topToken.level, topToken.text,
                                "Missing",
                                "Inserted missing closing tag for <" + topToken.text + ">");
                        }

                        if (!tagStack.empty() && tagStack.top().text == tagName) {
                            tagStack.pop();
                        }

                        fixedLine += "<" + token.text + ">";
                        errors.addError(lineNum, 0, tagName,
                            "Mismatched",
                            "Fixed mismatched closing tag for <" + tagName + ">");
                    }
                }
                else {
                    tagStack.push(token);
                    fixedLine += "<" + token.text + ">";
                }
            }
            else {
                fixedLine += token.text;
            }
        }

        fixedLines.push_back(fixedLine);
        lineNum++;
    }

    while (!tagStack.empty()) {
        Token topToken = tagStack.top();
        tagStack.pop();
        fixedLines.push_back(string(topToken.level * 4, ' ') +
            "</" + topToken.text + ">");
        errors.addError(lineNum, topToken.level, topToken.text,
            "Missing",
            "Inserted missing closing tag for <" + topToken.text + ">");
    }

    string fixedContent;
    for (auto& l : fixedLines) {
        fixedContent += l + "\n";
    }

    return fixedContent;
}
