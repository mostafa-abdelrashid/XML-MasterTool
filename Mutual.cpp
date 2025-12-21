#include "Mutual.h"
#include "SocialNetwork.h"
#include "XMLParser.h"
#include "XMLNode.h"
#include "comp_decomp.h"
#include <unordered_set>
#include <iostream>

using namespace std;

vector<int> MutualFollowers(
    const unordered_map<int, vector<int>>& graph,
    const vector<int>& userIds) {

    unordered_set<int> common;
    vector<int> result;

    if (userIds.empty()) return result;
    if (!graph.count(userIds[0])) return result;

    for (int f : graph.at(userIds[0]))
        common.insert(f);

    for (size_t i = 1; i < userIds.size(); i++) {
        unordered_set<int> temp;
        if (!graph.count(userIds[i])) return {};
        for (int f : graph.at(userIds[i])) {
            if (common.count(f))
                temp.insert(f);
        }
        common = temp;
    }

    result.assign(common.begin(), common.end());
    return result;
}
// Main function to find mutual followers
void mutual(const string& inputFile, const vector<int>& ids) {
    string xml = readFile(inputFile);

    XMLParser parser;
    XMLNode* root = parser.parse(xml);

    auto graph = buildFollowersGraph(root);
    auto res = MutualFollowers(graph, ids);

    if (res.empty()) {
        cout << "No mutual followers found\n";
    } else {
        cout << "Mutual followers: ";
        for (int x : res) cout << x << " ";
        cout << endl;
    }

    delete root;
}
