#include "XMLTokenizer.h"
#include <iostream>

XMLTokenizer g_tokenizer;
//Helper to remove extra spaces/newlines from the edges of text
string XMLTokenizer::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

//Breaking the string into Tokens
vector<Token> XMLTokenizer::tokenize(string filePath) {
    string content = readFile(filePath);
    if (content.empty()) return tokens;

    int i = 0, n = content.length();

    while (i < n) {
        if (content[i] == '<') {
            int j = i + 1;
            bool isClosing = false;

            if (j < n && content[j] == '/') {
                isClosing = true;
                j++;
            }

            int startName = j;
            // Find end of tag '>' or first whitespace
            while (j < n && content[j] != '>' && content[j] != ' ' && content[j] != '\n') j++;

            string tagPart = content.substr(i, j - i);

            if (j < n && content[j] == '>') {
                string tagName = content.substr(startName, j - startName);
                Token t;
                t.type = isClosing ? CLOSE_TAG : OPEN_TAG;
                t.value = tagName;
                tokens.push_back(t);
                i = j + 1;
            } else {
                Token t;
                t.type = UNKNOWN;
                t.value = tagPart;
                tokens.push_back(t);

                // Handle remaining text until next '<'
                int textStart = j;
                while (j < n && content[j] != '<') j++;
                string textContent = content.substr(textStart, j - textStart);
                string cleanText = trim(textContent);
                if (!cleanText.empty()) {
                    Token textToken;
                    textToken.type = TEXT_NODE;
                    textToken.value = cleanText;
                    tokens.push_back(textToken);
                }
                i = j;
            }
        } else {
            // Text outside tags
            int j = i;
            while (j < n && content[j] != '<') j++;

            string textContent = content.substr(i, j - i);
            string cleanText = trim(textContent);
            if (!cleanText.empty()) {
                if (cleanText.find('>') != string::npos) {
                    Token t;
                    t.type = UNKNOWN;
                    t.value = cleanText;
                    tokens.push_back(t);
                } else {
                    Token t;
                    t.type = TEXT_NODE;
                    t.value = cleanText;
                    tokens.push_back(t);
                }
            }
            i = j;
        }
    }

    return tokens;
}
const vector<Token>& XMLTokenizer::getTokens() const {
    return tokens;
}
 string readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) throw runtime_error("Error: Cannot open file " + filename);
    return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}
// Main function to convert XML to Tokens
void Tokens(const string& inputFile)
{
 g_tokenizer.tokenize(inputFile);
     for (const auto& t : g_tokenizer.getTokens()) {
        string typeStr = (t.type == OPEN_TAG) ? "OPEN" :
                         (t.type == CLOSE_TAG) ? "CLOSE" : "TEXT";
        cout << "Line " << t.line << " | [" << typeStr << "]: " << t.value << endl;
    }
}





