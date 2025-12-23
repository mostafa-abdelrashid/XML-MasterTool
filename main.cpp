#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib> 
#include <sstream>

// Core Utils
#include "error_check.h"      
#include "errorFixing.h"
#include "prettify.h"
#include "comp_decomp.h"
#include "minfiy.h"

// XML Parsing & Conversion
#include "XMLToJSONConverter.h"
#include "XMLParser.h"
#include "XMLNode.h"

// Social Network & Analysis headers
#include "SocialNetwork.h"
#include "Mutual.h"
#include "Suggest.h"
#include "Influencer.h"
#include "ActiveUser.h"
#include "SearchWord.h"
#include "SearchTopic.h"
#include "GraphVisualizer.h"

using namespace std;

// --- Helper Functions ---

void printUsage() {
    cout << "Usage:\n";
    cout << "  xml_editor verify -i <input> [-f] [-o <output>]   : Check validity (optional -f to fix)\n";
    cout << "  xml_editor format -i <input> -o <output>         : Prettify XML\n";
    cout << "  xml_editor json -i <input> -o <output>           : Convert XML to JSON\n";
    cout << "  xml_editor mini -i <input> -o <output>           : Minify XML\n";
    cout << "  xml_editor compress -i <input> -o <output>       : Compress XML\n";
    cout << "  xml_editor decompress -i <input> -o <output>     : Decompress XML\n";
    cout << "  xml_editor draw -i <input.xml> -o <output.jpg>   : Draw social network graph (Requires GraphViz)\n";
    cout << "  xml_editor most_active -i <input>                : Find the most active user\n";
    cout << "  xml_editor most_influencer -i <input>            : Find the most influential user\n";
    cout << "  xml_editor mutual -i <input> -ids <id1,id2...>   : Find mutual followers\n";
    cout << "  xml_editor suggest -i <input> -id <id>           : Suggest users to follow\n";
    cout << "  xml_editor search -i <input> -w <word>           : Search posts by word\n";
    cout << "  xml_editor search -i <input> -t <topic>          : Search posts by topic\n";
}

string getArg(int argc, char* argv[], string flag) {
    for (int i = 1; i < argc - 1; ++i) {
        if (string(argv[i]) == flag) {
            return string(argv[i + 1]);
        }
    }
    return "";
}

bool hasFlag(int argc, char* argv[], string flag) {
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == flag) {
            return true;
        }
    }
    return false;
}

vector<int> parseIds(string idsStr) {
    vector<int> ids;
    string segment;
    stringstream ss(idsStr);
    while (getline(ss, segment, ',')) {
        if (!segment.empty()) ids.push_back(stoi(segment));
    }
    return ids;
}

// Helper to load network for analysis commands
bool loadNetwork(const string& inputFile) {
    string xmlContent;
    try {
        xmlContent = readFile(inputFile);
    } catch (...) {
        cerr << "Error: Cannot open " << inputFile << endl;
        return false;
    }

    // Parse XML
    XMLParser parser;
    XMLNode* root = nullptr;
    try {
        root = parser.parse(xmlContent);
    } catch (const exception& e) {
        cerr << "XML Parsing Error: " << e.what() << endl;
        return false;
    }

    // Build Social Network (Populate global g_network)
    g_network.parseUsersFromXML(root);
    g_network.linkUsers();

    delete root; 
    return true;
}

// --- Main ---

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 0;
    }

    string action = argv[1];
    string inputFile = getArg(argc, argv, "-i");
    string outputFile = getArg(argc, argv, "-o");

    // VERIFY
    if (action == "verify") {
        if (inputFile.empty()) {
            cerr << "Error: Input file required (-i)\n";
            return 1;
        }

        // Pass the file PATH to countErrorSummary (updated logic)
        ErrorInfo errors = countErrorSummary(inputFile); 

        if (errors.count == 0) {
            cout << "Valid\n"; 
        } else {
            cout << "Invalid\n";
            cout << errors.count << " errors found.\n";
            highlightErrors(inputFile, errors);
        }

        // Fix option (-f)
        if (hasFlag(argc, argv, "-f")) {
            if (outputFile.empty()) {
                cerr << "Error: Output file required for fixing (-o)\n";
                return 1;
            }
            
            // Read content specifically for the fixing logic
            string content;
            try { content = readFile(inputFile); } 
            catch (...) { cerr << "Error: Cannot read file for fixing\n"; return 1; }

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

    // JSON CONVERSION
    else if (action == "json") {
        if (inputFile.empty() || outputFile.empty()) {
            cerr << "Error: Input and Output files required\n";
            return 1;
        }
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
            cerr << "Error: Input file and ids required (-i input.xml -ids 1,2,3)\n";
            return 1;
        }
        vector<int> ids = parseIds(idsStr);
        mutual(inputFile, ids);
    }

    // SUGGESTIONS
    else if (action == "suggest") {
        string idStr = getArg(argc, argv, "-id");
        if (inputFile.empty() || idStr.empty()) {
            cerr << "Error: Input file and id required (-i input.xml -id 1)\n";
            return 1;
        }
        suggest(inputFile, stoi(idStr));
    }

    // MOST ACTIVE USER
    else if (action == "most_active") {
        if (inputFile.empty()) {
            cerr << "Error: Input file required (-i)\n";
            return 1;
        }
        if (!loadNetwork(inputFile)) return 1;
        most_activeUser();
    }

    // MOST INFLUENTIAL USER
    else if (action == "most_influencer") {
        if (inputFile.empty()) {
            cerr << "Error: Input file required (-i)\n";
            return 1;
        }
        if (!loadNetwork(inputFile)) return 1;
        most_influencer();
    }

    // SEARCH (WORD OR TOPIC)
    else if (action == "search") {
        if (inputFile.empty()) {
            cerr << "Error: Input file required (-i)\n";
            return 1;
        }
        string word = getArg(argc, argv, "-w");
        string topic = getArg(argc, argv, "-t");
        string content = readFile(inputFile);

        if (!word.empty()) {
            cout << "Searching for word: " << word << endl;
            vector<PostMatch> results = searchPostsByWord(content, word);
            printMatches(results); 
        } 
        else if (!topic.empty()) {
            cout << "Searching for topic: " << topic << endl;
            vector<PostMatch> results = searchPostsByTopic(content, topic);
            printMatches(results); 
        }
        else {
            cerr << "Error: Provide a search term using -w (word) or -t (topic)\n";
            return 1;
        }
    }

    // DRAW GRAPH
    else if (action == "draw") {
        if (inputFile.empty() || outputFile.empty()) {
            cerr << "Error: Need -i input.xml and -o output.jpg\n";
            return 1;
        }
        size_t pos = outputFile.find_last_of('.');
        if (pos == string::npos) {
            cerr << "Error: Output file must have an extension\n";
            return 1;
        }
        string ext = outputFile.substr(pos + 1);

        try {
            // Generate the DOT file (intermediate format)
            string tempDotFile = "temp_graph.dot";
            generateDOTFile(inputFile, tempDotFile);

            // Call GraphViz (dot) command via system call
            string cmd = "dot -T" + ext + " \"" + tempDotFile + "\" -o \"" + outputFile + "\"";
            
            cout << "Executing: " << cmd << endl;
            if (system(cmd.c_str()) != 0) {
                cerr << "Error: Graphviz execution failed. Make sure 'dot' is installed and in PATH.\n";
                return 1;
            }

            // Clean up temp file
            remove(tempDotFile.c_str());
            cout << "Graph saved to: " << outputFile << endl;

        } catch (const exception& e) {
            cerr << "Error creating graph: " << e.what() << endl;
            return 1;
        }
    }
    else {
        cout << "Unknown command: " << action << endl;
        printUsage();
    }

    return 0;
}
