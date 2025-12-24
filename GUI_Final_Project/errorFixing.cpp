#include "errorFixing.h"
#include "error_check.h"
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <cctype>

using namespace std;

// ================= Structures =================

struct FixerToken {
    string tagName;
    int level;
    int lineIndex; // position in fixedLines where opening tag was inserted
};

struct LineToken {
    string type;   // "tag" or "value"
    string text;
    int level;
};

// ================= Helpers =================

bool isClosingTagFixer(const string& tag) {
    return !tag.empty() && tag[0] == '/';
}

bool isSelfClosingTagFixer(const string& tag) {
    return !tag.empty() && tag.back() == '/';
}

string getTagNameFixer(const string& tagText) {
    int i = 0;
    if (tagText[i] == '/') i++;

    string tagName;
    while (i < (int)tagText.size() &&
           tagText[i] != ' ' &&
           tagText[i] != '>' &&
           tagText[i] != '/') {
        tagName += tagText[i++];
    }
    return tagName;
}

vector<LineToken> tokenizeLineFixer(const string& line) {
    vector<LineToken> tokens;
    int i = 0;

    while (i < (int)line.size()) {
        int spaces = 0;
        while (i < (int)line.size() && line[i] == ' ') {
            spaces++;
            i++;
        }

        if (i < (int)line.size() && line[i] == '<') {
            LineToken token;
            token.type = "tag";
            token.level = spaces / 4;
            i++;

            while (i < (int)line.size() && line[i] != '>') {
                token.text += line[i++];
            }

            if (i < (int)line.size() && line[i] == '>') i++;
            tokens.push_back(token);
        }
        else if (i < (int)line.size() && !isspace(line[i])) {
            LineToken token;
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

// ================= Main Fixer =================

string fixErrors(const string& content, ErrorInfo& errors) {
    istringstream stream(content);
    string line;
    stack<FixerToken> tagStack;
    vector<string> fixedLines;
    int lineNum = 1;

    while (getline(stream, line)) {
        vector<LineToken> tokens = tokenizeLineFixer(line);
        string fixedLine;
        bool hasTextOnLine = false;

        for (auto& token : tokens) {

            // ---------- VALUE ----------
            if (token.type == "value") {
                fixedLine += token.text;
                hasTextOnLine = true;
                continue;
            }

            string tagName = getTagNameFixer(token.text);

            // ---------- CLOSING TAG ----------
            if (isClosingTagFixer(token.text)) {

                // Case 1: perfect match
                if (!tagStack.empty() && tagStack.top().tagName == tagName) {
                    tagStack.pop();
                    fixedLine += "<" + token.text + ">";
                }
                else {
                    // Check if exists deeper in stack
                    stack<FixerToken> temp = tagStack;
                    bool found = false;

                    while (!temp.empty()) {
                        if (temp.top().tagName == tagName) {
                            found = true;
                            break;
                        }
                        temp.pop();
                    }

                    // Case 2: close until matched
                    if (found) {
                        while (!tagStack.empty() && tagStack.top().tagName != tagName) {
                            FixerToken top = tagStack.top();
                            tagStack.pop();

                            fixedLine += "</" + top.tagName + ">";
                            errors.addError(
                                lineNum,
                                top.level,
                                top.tagName,
                                "Missing",
                                "Inserted missing closing tag for <" + top.tagName + ">"
                                );
                        }

                        if (!tagStack.empty()) {
                            tagStack.pop();
                            fixedLine += "<" + token.text + ">";
                        }
                    }
                    // Case 3: missing OPENING tag
                    else {
                        // INLINE: value</tag>
                        if (hasTextOnLine) {
                            fixedLine = "<" + tagName + ">" + fixedLine + "</" + tagName + ">";

                            errors.addError(
                                lineNum,
                                token.level,
                                tagName,
                                "Missing",
                                "Inserted missing opening tag <" + tagName + ">"
                                );
                        }
                        // BLOCK: </user> with no <user>
                        else {
                            int insertIndex = fixedLines.size();
                            if (!tagStack.empty()) {
                                insertIndex = tagStack.top().lineIndex + 1;
                            }

                            // Insert opening tag at correct position
                            fixedLines.insert(
                                fixedLines.begin() + insertIndex,
                                string(token.level * 4, ' ') + "<" + tagName + ">"
                                );

                            FixerToken ft;
                            ft.tagName = tagName;
                            ft.level = token.level;
                            ft.lineIndex = insertIndex;
                            tagStack.push(ft);

                            errors.addError(
                                lineNum,
                                token.level,
                                tagName,
                                "Missing",
                                "Inserted missing opening tag <" + tagName + ">"
                                );

                            // Now close it
                            tagStack.pop();
                            fixedLine += "</" + tagName + ">";
                        }
                    }
                }
            }

            // ---------- OPENING TAG ----------
            else {
                if (!isSelfClosingTagFixer(token.text)) {
                    FixerToken ft;
                    ft.tagName = tagName;
                    ft.level = token.level;
                    ft.lineIndex = fixedLines.size();
                    tagStack.push(ft);
                }
                fixedLine += "<" + token.text + ">";
            }
        }

        fixedLines.push_back(fixedLine);
        lineNum++;
    }

    // ---------- CLOSE REMAINING TAGS ----------
    while (!tagStack.empty()) {
        FixerToken top = tagStack.top();
        tagStack.pop();

        fixedLines.push_back(
            string(top.level * 4, ' ') + "</" + top.tagName + ">"
            );

        errors.addError(
            lineNum,
            top.level,
            top.tagName,
            "Missing",
            "Inserted missing closing tag for <" + top.tagName + ">"
            );
    }

    // ---------- BUILD OUTPUT ----------
    string fixedContent;
    for (const auto& l : fixedLines) {
        fixedContent += l + "\n";
    }

    return fixedContent;
}
