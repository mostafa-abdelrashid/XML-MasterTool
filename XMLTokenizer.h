#ifndef XML_TOKENIZER_H
#define XML_TOKENIZER_H

#include "Token.h"

class XMLTokenizer {
private:
    vector<Token> tokens;
    string trim(const string& str);

public:
    vector<Token> tokenize(string filePath);
    const vector<Token>& getTokens() const;

};
string readFile(const string& filename);
extern XMLTokenizer g_tokenizer;
void Tokens(const string& inputFile);
#endif

