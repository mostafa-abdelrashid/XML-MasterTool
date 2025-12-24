#include "XMLTokenizer.h"
#include "comp_decomp.h" // Added to use the centralized readFile
#include <fstream>
#include <stdexcept>

using namespace std;

XMLTokenizer g_tokenizer;

// Helper: trim spaces and newlines
string XMLTokenizer::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";

    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// readFile function removed from here to avoid duplication.
// It is already defined in comp_decomp.cpp

// Tokenize XML
vector<Token> XMLTokenizer::tokenize(string filePath) {

    tokens.clear();

    // Now uses the readFile from comp_decomp.cpp
    string content = readFile(filePath); 
    if (content.empty()) return tokens;

    int i = 0;
    int n = content.length();
    int lineNumber = 1;

    while (i < n) {

        // Count new lines
        if (content[i] == '\n') {
            lineNumber++;
            i++;
            continue;
        }

        // TAG
        if (content[i] == '<') {

            int j = i + 1;
            bool isClosing = false;

            if (j < n && content[j] == '/') {
                isClosing = true;
                j++;
            }

            int startName = j;

            // Read tag name
            while (j < n &&
                   content[j] != '>' &&
                   content[j] != ' ' &&
                   content[j] != '\n') {
                j++;
            }

            string tagName = content.substr(startName, j - startName);

            // Skip until '>'
            while (j < n && content[j] != '>') {
                if (content[j] == '\n')
                    lineNumber++;
                j++;
            }

            if (j < n && content[j] == '>') {
                Token t;
                t.type  = isClosing ? CLOSE_TAG : OPEN_TAG;
                t.value = tagName;
                t.line  = lineNumber;
                tokens.push_back(t);

                i = j + 1;
            }
        }
        // TEXT
        else {
            int j = i;

            while (j < n && content[j] != '<') {
                if (content[j] == '\n')
                    lineNumber++;
                j++;
            }

            string text = content.substr(i, j - i);
            string cleanText = trim(text);

            if (!cleanText.empty()) {
                Token t;
                t.type  = TEXT_NODE;
                t.value = cleanText;
                t.line  = lineNumber;
                tokens.push_back(t);
            }

            i = j;
        }
    }

    return tokens;
}

// Getter
const vector<Token>& XMLTokenizer::getTokens() const {
    return tokens;
}

void Tokens(const string& inputFile) {

    g_tokenizer.tokenize(inputFile);

    for (const auto& t : g_tokenizer.getTokens()) {

        string typeStr =
            (t.type == OPEN_TAG)  ? "OPEN"  :
            (t.type == CLOSE_TAG) ? "CLOSE" :
            (t.type == TEXT_NODE) ? "TEXT"  : "UNKNOWN";

        cout << "Line " << t.line
             << " | [" << typeStr << "]: "
             << t.value << endl;
    }
}