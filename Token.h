#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <memory>
#include <fstream>
#include <sstream>
using namespace std;
// Define the types of tokens
enum TokenType {
OPEN_TAG,   //<
CLOSE_TAG,  //</
TEXT_NODE,  //content
UNKNOWN };
struct Token {
    TokenType type;
    string value;
    int line;
};
struct XMLNode {
    string tagName;
    string value;
    vector<shared_ptr<XMLNode>> children;
    XMLNode(string name) : tagName(name) {}
};

struct XMLError {
    int line;
    string message;
};

#endif

