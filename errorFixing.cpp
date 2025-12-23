#include "errorFixing.h"
#include "error_check.h"
#include <string>
#include <vector>
#include <stack>
#include <sstream>

using namespace std;

// --- Private Helpers for Fixing Logic ---

// We define a local struct because we need 'level' (indentation) info
// which might not be present in the standard Token struct.
struct FixerToken {
    string type;
    string text;
    int level;
};

bool isClosingTagFixer(const string &tag) {
    return !tag.empty() && tag[0] == '/';
}

string getTagNameFixer(const string &tagText) {
    int i = 0;
    if (tagText[i] == '/') i++;
    string tagName = "";
    while (i < tagText.length() && tagText[i] != ' ' && tagText[i] != '>' && tagText[i] != '/') {
        tagName += tagText[i++];
    }
    return tagName;
}

vector<FixerToken> tokenizeLineFixer(const string& line) {
    vector<FixerToken> tokens;
    int i = 0;
    while (i < (int)line.size()) {
        int numSpace = 0;
        while (i < (int)line.size() && line[i] == ' ') {
            numSpace++;
            i++;
        }

        if (i < (int)line.size() && line[i] == '<') {
            FixerToken token;
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
        else if (!isspace(line[i])) {
            FixerToken token;
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

// --- Main Fixing Function ---

string fixErrors(const string& content, ErrorInfo& errors) {
    istringstream stream(content);
    string line;
    stack<FixerToken> tagStack;
    vector<string> fixedLines;
    int lineNum = 1;

    while (getline(stream, line)) {
        vector<FixerToken> tokens = tokenizeLineFixer(line);
        string fixedLine = "";

        for (auto& token : tokens) {
            if (token.type == "tag") {
                if (isClosingTagFixer(token.text)) {
                    string tagName = getTagNameFixer(token.text);

                    if (!tagStack.empty() && tagStack.top().text == tagName) {
                        tagStack.pop();
                        fixedLine += "<" + token.text + ">";
                    }
                    else {
                        // FIX: Insert missing closing tags
                        while (!tagStack.empty() && tagStack.top().text != tagName) {
                            FixerToken topToken = tagStack.top();
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
                        // Note: Mismatched tag error might be logged by verify, 
                        // but here we just ensure structure is valid.
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

    // FIX: Close any tags remaining on the stack at EOF
    while (!tagStack.empty()) {
        FixerToken topToken = tagStack.top();
        tagStack.pop();
        fixedLines.push_back(string(topToken.level * 4, ' ') + "</" + topToken.text + ">");
        
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
