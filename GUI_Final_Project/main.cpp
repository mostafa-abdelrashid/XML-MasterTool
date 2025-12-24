#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

// ================= Qt =================
#include <QApplication>
#include "launcherwindow.h"

// ================= Core Utils =================
#include "error_check.h"
#include "errorFixing.h"
#include "prettify.h"
#include "comp_decomp.h"
#include "minfiy.h"

// ================= XML Parsing =================
#include "XMLToJSONConverter.h"
#include "XMLParser.h"
#include "XMLNode.h"

// ================= Social Network =================
#include "SocialNetwork.h"
#include "Mutual.h"
#include "Suggest.h"
#include "Influencer.h"
#include "ActiveUser.h"
#include "SearchWord.h"
#include "SearchTopic.h"
#include "GraphVisualizer.h"

using namespace std;

// =================================================
//                  CLI HELPERS
// =================================================

void printUsage() {
    cout << "Usage:\n";
    cout << "  xml_editor verify -i <input> [-f] [-o <output>]\n";
    cout << "  xml_editor format -i <input> -o <output>\n";
    cout << "  xml_editor json -i <input> -o <output>\n";
    cout << "  xml_editor mini -i <input> -o <output>\n";
    cout << "  xml_editor compress -i <input> -o <output>\n";
    cout << "  xml_editor decompress -i <input> -o <output>\n";
    cout << "  xml_editor draw -i <input.xml> -o <output.jpg>\n";
    cout << "  xml_editor most_active -i <input>\n";
    cout << "  xml_editor most_influencer -i <input>\n";
    cout << "  xml_editor mutual -i <input> -ids 1,2,3\n";
    cout << "  xml_editor suggest -i <input> -id <id>\n";
    cout << "  xml_editor search -i <input> -w <word>\n";
    cout << "  xml_editor search -i <input> -t <topic>\n";
}

string getArg(int argc, char* argv[], const string& flag) {
    for (int i = 1; i < argc - 1; i++) {
        if (argv[i] == flag)
            return argv[i + 1];
    }
    return "";
}

bool hasFlag(int argc, char* argv[], const string& flag) {
    for (int i = 1; i < argc; i++)
        if (argv[i] == flag)
            return true;
    return false;
}

vector<int> parseIds(const string& idsStr) {
    vector<int> ids;
    string temp;
    stringstream ss(idsStr);
    while (getline(ss, temp, ','))
        ids.push_back(stoi(temp));
    return ids;
}

// =================================================
//                  CLI RUNNER
// =================================================

int runCLI(int argc, char* argv[])
{
    if (argc < 2) {
        printUsage();
        return 0;
    }

    string action = argv[1];
    string inputFile = getArg(argc, argv, "-i");
    string outputFile = getArg(argc, argv, "-o");

    // ================= VERIFY =================
    if (action == "verify") {
        if (inputFile.empty()) {
            cerr << "Missing input file\n";
            return 1;
        }

        ErrorInfo errors = countErrorSummary(inputFile);

        if (errors.count == 0) {
            cout << "XML is valid\n";
        } else {
            cout << "XML is invalid (" << errors.count << " errors)\n";
            highlightErrors(inputFile, errors);
        }

        if (hasFlag(argc, argv, "-f")) {
            if (outputFile.empty()) {
                cerr << "Missing output file\n";
                return 1;
            }
            string content = readFile(inputFile);
            string fixed = fixErrors(content, errors);
            writeFile(outputFile, fixed);
            cout << "Fixed XML saved to " << outputFile << endl;
        }
    }

    // ================= FORMAT =================
    else if (action == "format") {
        prettifyXML(inputFile, outputFile);
    }

    // ================= JSON =================
    else if (action == "json") {
        XMLParser parser;
        XMLNode* root = parser.parse(readFile(inputFile));
        XMLToJSONConverter conv(true);
        writeFile(outputFile, conv.convert(root));
        delete root;
    }

    // ================= MINIFY =================
    else if (action == "mini") {
        minifyXML(inputFile, outputFile);
    }

    // ================= COMPRESS =================
    else if (action == "compress") {
        compress(inputFile, outputFile);
    }

    // ================= DECOMPRESS =================
    else if (action == "decompress") {
        decompress(inputFile, outputFile);
    }

    // ================= MOST ACTIVE =================
    else if (action == "most_active") {
        most_activeUser();
    }

    // ================= MOST INFLUENCER =================
    else if (action == "most_influencer") {
        most_influencer();
    }

    // ================= MUTUAL =================
    else if (action == "mutual") {
        vector<int> ids = parseIds(getArg(argc, argv, "-ids"));
        mutual(inputFile, ids);
    }

    // ================= SUGGEST =================
    else if (action == "suggest") {
        suggest(inputFile, stoi(getArg(argc, argv, "-id")));
    }

    // ================= SEARCH =================
    else if (action == "search") {
        string content = readFile(inputFile);
        if (hasFlag(argc, argv, "-w"))
            printMatches(searchPostsByWord(content, getArg(argc, argv, "-w")));
        else
            printMatches(searchPostsByTopic(content, getArg(argc, argv, "-t")));
    }

    // ================= DRAW =================
    else if (action == "draw") {
        generateDOTFile(inputFile, "temp.dot");
        system(("dot -Tjpg temp.dot -o " + outputFile).c_str());
        remove("temp.dot");
    }

    else {
        printUsage();
    }

    return 0;
}

// =================================================
//                  MAIN ENTRY
// =================================================

int main(int argc, char *argv[])
{
    // ===== CLI MODE =====
    if (argc > 1) {
        return runCLI(argc, argv);
    }

    // ===== GUI MODE =====
    QApplication app(argc, argv);
    LauncherWindow w;
    w.show();
    return app.exec();
}
