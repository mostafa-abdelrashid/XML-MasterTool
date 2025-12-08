#include <iostream>
#include <string>
#include <fstream>
#include "minfiy.cpp"

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
    return string((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
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

            // closing tag: decrease indent first
            if (isClosing) {
                indent = max(0, indent - 1);
            }

            addIndented(buffer);

            // opening tag: increase indent
            if (!isClosing && !isSelfClosing)
                indent++;

            buffer.clear();
        }
        else if (xml[i] != '<') {
            // case 2: we are reading TEXT content
            if (i + 1 == xml.size() || xml[i + 1] == '<') {
                addIndented(buffer);
                buffer.clear();

            }
        }
    }
    writeFile(outfilename, pretty);
}



