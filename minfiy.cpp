#include <iostream>
#include <string>
#include <fstream>
#include <stack>

using namespace std;

void minifyXML(const string& inFile, const string& outFile){
    ifstream inputFile(inFile);
    if (!inputFile.is_open()) {
        cerr << "Error: Cannot open input file " << inFile << endl;
        return;
    }
    string xml((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    string output;

    bool insideTag = false;
    string textBuffer;
    for(size_t i=0; i<xml.size(); i++){
        char c = xml[i];

        // Entering tag
        if (c == '<'){
            insideTag = true;

            // Flush trimmed text before writing the tag
            if(!textBuffer.empty()){
                // Trim leading and trailing whitespace
                size_t start = textBuffer.find_first_not_of(" \t\n\r");
                size_t finish = textBuffer.find_last_not_of(" \t\n\r");

                if (start != string::npos)
                    output += textBuffer.substr(start, finish - start +1);

                textBuffer.clear();
            }

            output += '<';
            continue;
        }

        // Exiting tag
        if (c == '>'){
            insideTag = false;
            output += '>';
            continue;
        }

        if (insideTag){
            // Keep tag contents exactly
            output += c;
        }
        else{
            // Outside tag → text content
            textBuffer += c;
        }
    }
    ofstream outputFile(outFile);
    if (!outputFile.is_open()) {
        cerr << "Error: Cannot open output file " << outFile << endl;
        return;
    }

    outputFile << output;
    outputFile.close();
}
