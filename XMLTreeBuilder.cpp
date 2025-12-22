#include "XMLTreeBuilder.h"
#include "XMLTokenizer.h"
#include <stack>
#include <iostream>
#include <vector>

using namespace std;

// Build tree and return root
XMLNode* buildTree(const vector<Token>& tokens, vector<XMLError>& errors) {
    stack<XMLNode*> nodeStack;
    XMLNode* root = new XMLNode("ROOT");
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

    return root;
}

// Print tree recursively
void printTree(XMLNode* node, int depth ) {
    if (!node) return;

    string indent(depth * 4, ' ');
    cout << indent << "[" << node->getName() << "]";

    if (!node->getContent().empty()) {
        cout << " -> Value: \"" << node->getContent() << "\"";
    }
    cout << endl;

    for (XMLNode* child : node->getChildren()) {
        printTree(child, depth + 1);
    }
}

// Print errors
void printErrors(const vector<XMLError>& errors) {
    cout << "\n--- Error Report ---\n";
    if (errors.empty()) cout << " No errors found.\n";
    for (const auto& err : errors) {
        cout << "[Line " << err.line << "] " << err.message << "\n";
    }
}

XMLNode* g_root = nullptr;
vector<XMLError> g_errors;
void Tree() {
    if (g_tokens.empty()) {
        cout << "Error: Tokens not built yet!\n";
        return;
    }

    g_root = buildTree(g_tokens, g_errors);

    cout << "\n--- Tree implementation ---\n";
    printTree(g_root);

    if (!g_errors.empty()) {
        printErrors(g_errors);
    }
}
