#ifndef XML_TOKENIZER_H
#define XML_TOKENIZER_H

#include "Token.h"

extern std::vector<Token> g_tokens;

string readFile(const string& filename);
string trim(const string& str);

vector<Token> tokenize(string filePath);
void Tokens(const string& inputFile);
#endif
