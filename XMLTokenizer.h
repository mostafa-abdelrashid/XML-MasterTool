#ifndef XMLTOKENIZER_H
#define XMLTOKENIZER_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Token.h"
using namespace std;

class XMLTokenizer {
public:
    //Takes file path, returns list of tokens
    vector<Token> tokenize(string filePath);
private:
    // Helper to read file into a single string
    string readFileToString(string filePath);
    // Helper to trim whitespace (newlines/tabs) from text
    string trim(const string& str);
    };

#endif
