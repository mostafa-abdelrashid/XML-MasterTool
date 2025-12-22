#include "GraphVisualizer.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "comp_decomp.h"
#include "XMLParser.h"
#include "XMLNode.h"
#include <string>
#include <vector>
#include "SocialNetwork.h"

using namespace std;
void generateDOTFile(string inputfile, string Outputfile) {
    // Read XML file
    string xmlContent = readFile(inputfile);
    // Parse XML
    XMLParser parser;
    XMLNode* xmlTree = parser.parse(xmlContent);
    // Build followers graph
    unordered_map<int, vector<int>> followersGraph;
    followersGraph = buildFollowersGraph(xmlTree);
    // Generate DOT file
    ofstream file(Outputfile);
    file << "digraph G {\n";
    file << "  node [shape=circle];\n";
    
    file << "  node [style=filled, fillcolor=\"#E6F2FF\", color=\"#4D94FF\"];\n";
    file << "  edge [color=\"#66B2FF\"];\n\n";

    for (const auto &pair : followersGraph) {
        int user = pair.first;
        for (int follower : pair.second) {
            file << "  " << follower << " -> " << user << ";\n";
        }
    }

    file << "}\n";
    file.close();
}