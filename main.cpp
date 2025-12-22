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
#include "Mutual.h"
#include "Suggest.h"
#include "SocialNetwork.h"
// #include "GraphVisualizer.h"
// #include <cstdlib> 

using namespace std;


void printUsage() {
    cout << "Usage:\n";
    cout << "  xml_editor verify -i <input> [-f] [-o <output>]   : Check validity (optional -f to fix)\n";
    cout << "  xml_editor format -i <input> -o <output>         : Prettify XML\n";
    cout << "  xml_editor json -i <input> -o <output>           : Convert XML to JSON\n";
    cout << "  xml_editor mini -i <input> -o <output>           : Minify XML\n";
    cout << "  xml_editor compress -i <input> -o <output>       : Compress XML\n";
    cout << "  xml_editor decompress -i <input> -o <output>     : Decompress XML\n";
    // cout << "  xml_editor draw -i <input.xml> -o <output>       : Draw social network graph (JPG/PNG)\n";
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
    // MUTUAL FOLLOWERS
    else if (action == "mutual") {
        string idsStr = getArg(argc, argv, "-ids");
        if (inputFile.empty() || idsStr.empty()) {
            cerr << "Error: Input file and ids required\n";
            return 1;
        }
        vector<int> ids = parseIds(idsStr);
        mutual(inputFile, ids);
        cout << "Mutual followers printed." << endl;
    }

    // SUGGEST USERS
    else if (action == "suggest") {
        string idStr = getArg(argc, argv, "-id");
        if (inputFile.empty() || idStr.empty()) {
            cerr << "Error: Input file and id required\n";
            return 1;
        }
        suggest(inputFile, stoi(idStr));
        cout << "Suggestions printed." << endl;
    }
    // else if (action == "draw") {
    //     if (inputFile.empty() || outputFile.empty()) {
    //         cerr << "Error: Need -i input.xml and -o output.png\n";
    //         return 1;
    //     }

    //     size_t pos = outputFile.find_last_of('.');
    //     if (pos == string::npos) {
    //         cerr << "Error: Output file must have an extension\n";
    //         return 1;
    //     }

    //     string ext = outputFile.substr(pos + 1);
    //     if (ext != "png" && ext != "jpg") {
    //         cerr << "Error: Output must be .png or .jpg\n";
    //         return 1;
    //     }

    //     try {
    //         string tempFile = "temp_graph_" + to_string(time(nullptr)) + ".dot";

    //         generateDOTFile(inputFile, tempFile);

    //         string cmd = "dot -T" + ext + " \"" + tempFile + "\" -o \"" + outputFile + "\"";

    //         if (system(cmd.c_str()) != 0) {
    //             cerr << "Graphviz execution failed.\n";
    //             cerr << "DOT file saved as: " << tempFile << endl;
    //             return 1;
    //         }

    //         remove(tempFile.c_str());
    //         cout << "Graph saved: " << outputFile << endl;

    //     } catch (...) {
    //         cerr << "Error creating graph\n";
    //         return 1;
    //     }
    // }

    else {
        cout << "Unknown command: " << action << endl;
        printUsage();
    }

    return 0;

}
