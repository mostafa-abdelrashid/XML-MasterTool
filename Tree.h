#ifndef XML_TREE_BUILDER_H
#define XML_TREE_BUILDER_H

#include <vector>
#include <stack>
#include "Token.h"
#include "XMLNode.h"

class XMLTreeBuilder {
private:
    vector<XMLError> errors;
    XMLNode* root;

public:
    XMLTreeBuilder() : root(nullptr) {}
    ~XMLTreeBuilder() { delete root; }

    void build(const vector<Token>& tokens);
    void printErrors();

    XMLNode* getRoot() { return root; }
};

#endif
