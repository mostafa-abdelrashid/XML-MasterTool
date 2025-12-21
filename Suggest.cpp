#include "Suggest.h"
#include "SocialNetwork.h"
#include "XMLParser.h"
#include "XMLNode.h"
#include "comp_decomp.h"
#include <unordered_set>
#include <iostream>

using namespace std;

vector<int> suggestions(
    const unordered_map<int, vector<int>>& graph,
    int userId) {

    unordered_set<int> myFollowers;
    unordered_set<int> result;

    if (!graph.count(userId)) return {};

    for (int f : graph.at(userId))
        myFollowers.insert(f);

    for (int f : myFollowers) {
        if (!graph.count(f)) continue;
        for (int fof : graph.at(f)) {
            if (fof != userId && !myFollowers.count(fof))
                result.insert(fof);
        }
    }

    return vector<int>(result.begin(), result.end());
}

// main function to suggest users to follow
void suggest(const string& inputFile, int userId) {
    string xml = readFile(inputFile);

    XMLParser parser;
    XMLNode* root = parser.parse(xml);

    auto graph = buildFollowersGraph(root);
    auto res = suggestions(graph, userId);

    if (res.empty()) {
        cout << "No suggestions available\n";
    } else {
        cout << "Suggested users: ";
        for (int x : res) cout << x << " ";
        cout << endl;
    }

    delete root;
}
