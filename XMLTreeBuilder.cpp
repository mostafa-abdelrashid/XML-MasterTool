#include "XMLTreeBuilder.h"
#include <iostream>

using namespace std;

void XMLTreeBuilder::build(const vector<Token>& tokens) {
    stack<XMLNode*> nodeStack;
    root = new XMLNode("ROOT");
    nodeStack.push(root);
    errors.clear();

    for (const auto& token : tokens) {
        if (token.type == OPEN_TAG) {
            XMLNode* newNode = new XMLNode(token.value);
            nodeStack.top()->addChild(newNode);
            newNode->setParent(nodeStack.top());
            nodeStack.push(newNode);
        }
        else if (token.type == TEXT_NODE) {
            nodeStack.top()->setContent(token.value);
        }
        else if (token.type == CLOSE_TAG) {
            if (nodeStack.size() <= 1) {
                errors.push_back({token.line,
                    "Extra closing tag </" + token.value + "> found (Stack empty)."});
            }
            else if (nodeStack.top()->getName() != token.value) {
                errors.push_back({token.line,
                    "Mismatch: Found </" + token.value +
                    "> but expected </" + nodeStack.top()->getName() + ">."});
                nodeStack.pop();
            }
            else {
                nodeStack.pop();
            }
        }
    }

    while (nodeStack.size() > 1) {
        errors.push_back({-1,
            "EOF Error: Tag <" + nodeStack.top()->getName() + "> was never closed."});
        nodeStack.pop();
    }
}

void XMLTreeBuilder::printErrors() {
    cout << "\n--- Error Report ---\n";
    if (errors.empty()) cout << " No errors found.\n";
    for (const auto& err : errors) {
        cout << "[Line " << err.line << "] " << err.message << "\n";
    }
}
