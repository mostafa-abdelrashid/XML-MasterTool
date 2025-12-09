#include "XMLParser.h"
#include <cctype>
#include <stdexcept>

using namespace std;

XMLParser::XMLParser() : position(0), rootNode(nullptr) {}

XMLNode* XMLParser::parse(const string& xmlStr) {
    position = 0;
    xmlString = xmlStr;
    rootNode = nullptr;

    skipWhitespace();
    if (peekChar() != '<') throwError("XML must start with '<'");

    rootNode = parseElement();
    skipWhitespace();

    if (!isParsingComplete()) throwError("Extra content after root element");

    return rootNode;
}

bool XMLParser::isParsingComplete() const {
    return position >= xmlString.length();
}

size_t XMLParser::getPosition() const {
    return position;
}

void XMLParser::skipWhitespace() {
    while (position < xmlString.length() && isspace(xmlString[position])) {
        position++;
    }
}

char XMLParser::peekChar() {
    return (position < xmlString.length()) ? xmlString[position] : '\0';
}

char XMLParser::getChar() {
    return (position < xmlString.length()) ? xmlString[position++] : '\0';
}

bool XMLParser::match(const string& str) {
    if (position + str.length() > xmlString.length()) return false;

    for (size_t i = 0; i < str.length(); i++)
        if (xmlString[position + i] != str[i]) return false;

    position += str.length();
    return true;
}

void XMLParser::throwError(const string& message) {
    size_t start = (position > 20) ? position - 20 : 0;
    size_t end = (position + 20 < xmlString.length()) ? position + 20 : xmlString.length();

    string context = xmlString.substr(start, end - start);
    throw runtime_error(message +
        "\nPosition: " + to_string(position) +
        "\nContext: ..." + context + "...");
}

string XMLParser::parseTagName() {
    skipWhitespace();
    string name;
    char c = peekChar();

    if (!isalpha(c) && c != '_') throwError("Tag name must start with letter or underscore");

    while (position < xmlString.length()) {
        c = peekChar();
        if (c == '>' || c == '/' || isspace(c)) break;
        name += getChar();
    }

    if (name.empty()) throwError("Empty tag name");
    return name;
}

string XMLParser::parseTextContent() {
    string text;

    while (position < xmlString.length()) {
        char c = peekChar();
        if (c == '<') break;

        if (c == '&') {
            while (position < xmlString.length() && peekChar() != ';')
                text += getChar();
            if (position < xmlString.length()) text += getChar();
        }
        else {
            text += getChar();
        }
    }

    while (!text.empty() && isspace(text.back()))
        text.pop_back();

    return text;
}

void XMLParser::parseComment() {
    while (position < xmlString.length()) {
        if (match("-->")) return;
        getChar();
    }
    throwError("Unclosed comment");
}

XMLNode* XMLParser::parseElement() {
    if (getChar() != '<') throwError("Expected '<'");

    if (match("!--")) {
        parseComment();
        return nullptr;
    }

    if (peekChar() == '/') {
        getChar();
        string closingName = parseTagName();
        skipWhitespace();
        if (getChar() != '>') throwError("Expected '>' in closing tag");
        return nullptr;
    }

    string tagName = parseTagName();
    skipWhitespace();

    if (peekChar() == '/') {
        getChar();
        if (getChar() != '>') throwError("Expected '>' in self-closing tag");
        return new XMLNode(tagName);
    }

    if (getChar() != '>') throwError("Expected '>' after tag name");

    XMLNode* node = new XMLNode(tagName);

    skipWhitespace();

    if (peekChar() != '<') {
        node->setContent(parseTextContent());
        skipWhitespace();
    }

    while (position < xmlString.length() && peekChar() == '<') {
        if (position + 1 < xmlString.length() && xmlString[position + 1] == '/') {
            size_t savedPos = position;
            position += 2;
            skipWhitespace();

            string closingName = parseTagName();
            if (closingName == tagName) {
                position = savedPos;
                break;
            }
            position = savedPos;
        }

        XMLNode* child = parseElement();
        if (child) node->addChild(child);
        skipWhitespace();
    }

    if (!match("</")) throwError("Expected closing tag for: " + tagName);

    skipWhitespace();
    string closingName = parseTagName();

    if (closingName != tagName)
        throwError("Mismatched closing tag: expected " + tagName + ", got " + closingName);

    skipWhitespace();
    if (getChar() != '>') throwError("Expected '>' at end of closing tag");

    return node;
}
