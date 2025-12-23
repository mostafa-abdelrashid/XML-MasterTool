#ifndef XMLTREEHELPER_H
#define XMLTREEHELPER_H

#include <vector>
#include <string>
#include "Token.h"
#include "XMLNode.h"
#include "XMLTokenizer.h"

using namespace std;
extern XMLNode* g_root;
class XMLTreeBuilder {
private:
    vector<XMLError> errors;
    XMLNode* root;

public:
    XMLTreeBuilder() : root(nullptr) {}
    ~XMLTreeBuilder() { delete root; }

    void build(const vector<Token>& tokens);
    void printErrors()const;
    void printTree(XMLNode* node, int depth = 0)const;

    XMLNode* getRoot() { return root; }
};
// global tree builder
extern XMLTreeBuilder g_treeBuilder;
void Tree();
#endif
