#ifndef XMLTREEHELPER_H
#define XMLTREEHELPER_H

#include <vector>
#include <string>
#include "Token.h"
#include "XMLNode.h"
#include "XMLTokenizer.h"

using namespace std;
extern XMLNode* g_root;
extern std::vector<XMLError> g_errors;

XMLNode* buildTree(const vector<Token>& tokens, vector<XMLError>& errors);

// Print tree
void printTree(XMLNode* node, int depth = 0);

// Print errors
void printErrors(const vector<XMLError>& errors);

void Tree();

#endif
