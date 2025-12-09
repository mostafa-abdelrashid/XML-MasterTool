#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "XMLNode.h"
#include <string>

using namespace std;

class XMLParser {
private:
	size_t position;   // Current position in the XML string
	string xmlString;  // The XML string being parsed
	XMLNode* rootNode;  // Root node of the parsed XML tree

    void skipWhitespace();
    char peekChar();
    char getChar();
    bool match(const string& str);

    string parseTagName();
    string parseTextContent();
    void parseComment();
    XMLNode* parseElement();

    void throwError(const string& message);

public:
    XMLParser();
    XMLNode* parse(const string& xmlStr);

    bool isParsingComplete() const;
    size_t getPosition() const;
};

#endif // XMLPARSER_H
