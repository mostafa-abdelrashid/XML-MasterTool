#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED
#include <string>
#include <iostream>
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
// Helper function to print token type
string typeToString()const {
    switch (type) {
        case OPEN_TAG: return "OPEN_TAG";
        case CLOSE_TAG: return "CLOSE_TAG";
        case TEXT_NODE: return "TEXT";
        default: return "UNKNOWN";}
        }
};


#endif
