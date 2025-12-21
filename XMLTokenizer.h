#ifndef XML_TOKENIZER_H
#define XML_TOKENIZER_H

#include "Token.h"

class XMLTokenizer {
private:
    string readFileToString(string filePath);
    string trim(const string& str);

public:
    vector<Token> tokenize(string filePath);
};

#endif
