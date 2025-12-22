#include "XMLTokenizer.h"

using namespace std;


 string readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) throw runtime_error("Error: Cannot open file " + filename);
    return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

vector<Token>tokenize(string filePath) {
    vector<Token> tokens;
    string content = readFile(filePath);
    if (content.empty()) return tokens;

    int i = 0, n = content.length();
    int currentLine = 1;

    while (i < n) {
        // Track line numbers
        if (content[i] == '\n') { currentLine++; i++; continue; }

        if (content[i] == '<') {
            int j = i + 1;
            bool isClosing = false;

            if (j < n && content[j] == '/') {
                isClosing = true;
                j++;
            }

            int startName = j;
            while (j < n && content[j] != '>' && content[j] != ' ' && content[j] != '\n') j++;

            if (j < n && content[j] == '>') {
                string tagName = content.substr(startName, j - startName);
                tokens.push_back({isClosing ? CLOSE_TAG : OPEN_TAG, tagName, currentLine});
                i = j + 1;
            } else {
                // Handle malformed tag (Case 3 logic starts here)
                tokens.push_back({UNKNOWN, content.substr(i, j - i), currentLine});
                i = j;
            }
        } else {
            int j = i;
            while (j < n && content[j] != '<') {
                if (content[j] == '\n') currentLine++;
                j++;
            }
            string cleanText = trim(content.substr(i, j - i));
           if (!cleanText.empty() && cleanText != "...") {
           tokens.push_back({TEXT_NODE, cleanText, currentLine});
        }
            i = j;
        }
    }
    return tokens;
}
vector<Token> g_tokens;
void Tokens(const string& inputFile)
{
 g_tokens = tokenize(inputFile);
     for (const auto& t : g_tokens) {
        string typeStr = (t.type == OPEN_TAG) ? "OPEN" :
                         (t.type == CLOSE_TAG) ? "CLOSE" : "TEXT";
        cout << "Line " << t.line << " | [" << typeStr << "]: " << t.value << endl;
    }
}





