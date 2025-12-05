#include <iostream>
#include <string>
#include <fstream>
#include <stack>

using namespace std;
bool writeFile(const string &filename, const string &content) {
    ofstream out(filename);

    if (!out.is_open())
        return false;  

    out << content;   
    out.close();      
    return true;
}

string readFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Error: Cannot open file " + filename);
    }

    // Read whole file into a string
    return string((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
}
void minifyXML(const string &infilename, const string &outfilename) {
    string xml =readFile(infilename);
    string output;
    bool insideTag = false;
    bool insideText = false;

    for (size_t i = 0; i < xml.size(); i++) {
        char c = xml[i];

        if (c == '<') {
            // entering a tag → remove whitespace before '<'
            insideTag = true;
            insideText = false;
            output += '<';
            continue;
        }

        if (c == '>') {
            insideTag = false;
            output += '>';
            continue;
        }

        if (insideTag) {
            // keep everything inside tag <...>
            if (!isspace(c))
                output += c;
        }
        else {
            // Outside tag → text content
            if (!isspace(c)) {
                insideText = true;
                output += c;
            } else {
                // whitespace inside text node is kept as a single space if meaningful
                if (insideText)
                    output += ' ';
            }
        }
    }

    writeFile(outfilename, output);
}
void prettifyXML(const string &infilename, const string &outfilename) {
    minifyXML(infilename, outfilename);
    string xml = readFile(outfilename);
    string pretty;
    int indent = 0;
    string buffer;

    auto addIndented = [&](const string &s) {
        pretty += string(indent * 4, ' ') + s + "\n";
    };

    for (size_t i = 0; i < xml.size(); ++i) {
        buffer += xml[i];

        // case 1: tag ended
        if (xml[i] == '>') {
            bool isClosing = false;
            bool isSelfClosing = false;

            if (buffer.size() >= 2 && buffer[1] == '/')
                isClosing = true;

            if (buffer.size() >= 2 && buffer[buffer.size() - 2] == '/')
                isSelfClosing = true;

            // closing tag? decrease indent first
            if (isClosing) {
                indent = max(0, indent - 1);
            }

            addIndented(buffer);

            // opening tag (not self closing), increase indent
            if (!isClosing && !isSelfClosing)
                indent++;

            buffer.clear();
        }
        else if (xml[i] != '<') {
            // case 2: we are reading TEXT content
            // flush when next char begins a tag or end of file
            if (i + 1 == xml.size() || xml[i + 1] == '<') {
                addIndented(buffer);
                buffer.clear();

            }
        }
    }
    writeFile(outfilename, pretty);
}



int main() {
prettifyXML("sample.xml","output.xml");
 
}