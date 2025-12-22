#include "XMLTreeBuilder.h"
#include <iostream>

using namespace std;
XMLTreeBuilder g_treeBuilder;
XMLNode* g_root = nullptr;

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

void XMLTreeBuilder::printErrors()const  {
    cout << "\n--- Error Report ---\n";
    if (errors.empty()) cout << " No errors found.\n";
    for (const auto& err : errors) {
        cout << "[Line " << err.line << "] " << err.message << "\n";
    }
}
// Print tree recursively
void XMLTreeBuilder::printTree(XMLNode* node, int depth )const  {
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

// Main function to bulid a tree

extern XMLTokenizer g_tokenizer;

void Tree() {

    const vector<Token>& tokens = g_tokenizer.getTokens();

    if (tokens.empty()) {
        cout << "Error: Tokens not built yet!\n";
        return;
    }

    g_treeBuilder.build(tokens);
    g_root = g_treeBuilder.getRoot();

    cout << "\n--- Tree implementation ---\n";
    g_treeBuilder.printTree(g_root);

    g_treeBuilder.printErrors();
}
