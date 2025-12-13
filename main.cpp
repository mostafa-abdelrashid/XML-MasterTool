#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "error_check.h"      
#include "errorFixing.h"
#include "prettify.h"
#include "XMLToJSONConverter.h"
#include "comp_decomp.h"
#include "XMLParser.h"

using namespace std;


void printUsage() {
    cout << "Usage:\n";
    cout << "  xml_editor verify -i <input> [-f] [-o <output>]   : Check validity (optional -f to fix)\n";
    cout << "  xml_editor format -i <input> -o <output>         : Prettify XML\n";
    cout << "  xml_editor json -i <input> -o <output>           : Convert XML to JSON\n";
    cout << "  xml_editor mini -i <input> -o <output>           : Minify XML\n";
    cout << "  xml_editor compress -i <input> -o <output>       : Compress XML\n";
    cout << "  xml_editor decompress -i <input> -o <output>     : Decompress XML\n";
}

//get argument value
string getArg(int argc, char* argv[], string flag) {
    for (int i = 1; i < argc - 1; ++i) {
        if (string(argv[i]) == flag) {
            return string(argv[i + 1]);
        }
    }
    return "";
}

//check if flag exists
bool hasFlag(int argc, char* argv[], string flag) {
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == flag) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 0;
    }

    string action = argv[1];
    string inputFile = getArg(argc, argv, "-i");
    string outputFile = getArg(argc, argv, "-o");

    //VERIFY / CHECK CONSISTENCY
    if (action == "verify") {
        if (inputFile.empty()) {
            cerr << "Error: Input file required (-i)\n";
            return 1;
        }

        // Read file content (using helper from comp_decomp)
        string content;
        try {
            content = readFile(inputFile); 
        } catch (...) {
            cerr << "Error: Cannot open " << inputFile << endl;
            return 1;
        }

        // Perform Check
        ErrorInfo errors = countErrorSummary(content); 

        if (errors.count == 0) {
            cout << "Valid\n"; 
        } else {
            cout << "Invalid\n";
            cout << errors.count << " errors found.\n";
            for(size_t i=0; i<errors.lines.size(); i++) {
                cout << "Line " << errors.lines[i] << ": " << errors.descriptions[i] << endl;
            }
        }

        // Fix option (-f)
        if (hasFlag(argc, argv, "-f")) {
            if (outputFile.empty()) {
                cerr << "Error: Output file required for fixing (-o)\n";
                return 1;
            }
            string fixedContent = fixErrors(content, errors);
            if (writeFile(outputFile, fixedContent)) {
                 cout << "Fixed XML written to " << outputFile << endl;
            } else {
                 cerr << "Error writing to " << outputFile << endl;
            }
        }
    }

    // FORMAT / PRETTIFY
    else if (action == "format") {
        if (inputFile.empty() || outputFile.empty()) {
            cerr << "Error: Input and Output files required\n";
            return 1;
        }
        prettifyXML(inputFile, outputFile);
        cout << "Formatted XML saved to " << outputFile << endl;
    }

    // CONVERT TO JSON
    else if (action == "json") {
        if (inputFile.empty() || outputFile.empty()) {
            cerr << "Error: Input and Output files required\n";
            return 1;
        }
        // NOTE: Make sure your XMLToJSONConverter supports passing filenames directly
        // If not, use the logic below:
        string xmlContent = readFile(inputFile);
        XMLParser parser;
        XMLNode* root = parser.parse(xmlContent);
        
        XMLToJSONConverter converter(true); 
        string jsonOutput = converter.convert(root);
        
        writeFile(outputFile, jsonOutput);
        cout << "JSON saved to " << outputFile << endl;
        
        delete root;
    }

    // MINIFY
    else if (action == "mini") {
        if (inputFile.empty() || outputFile.empty()) {
            cerr << "Error: Input and Output files required\n";
            return 1;
        }
        minifyXML(inputFile, outputFile);
        cout << "Minified XML saved to " << outputFile << endl;
    }

    // COMPRESS
    else if (action == "compress") {
        if (inputFile.empty() || outputFile.empty()) {
            cerr << "Error: Input and Output files required\n";
            return 1;
        }
        compress(inputFile, outputFile);
        cout << "Compressed file saved to " << outputFile << endl;
    }

    // DECOMPRESS
    else if (action == "decompress") {
        if (inputFile.empty() || outputFile.empty()) {
            cerr << "Error: Input and Output files required\n";
            return 1;
        }
        decompress(inputFile, outputFile);
        cout << "Decompressed file saved to " << outputFile << endl;
    }

    else {
        cout << "Unknown command: " << action << endl;
        printUsage();
    }

    return 0;
}