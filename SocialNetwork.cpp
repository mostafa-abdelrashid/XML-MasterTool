#include "SocialNetwork.h"
#include <iostream>
using namespace std;

SocialNetwork g_network;

SocialNetwork::SocialNetwork() {}

SocialNetwork::~SocialNetwork() {
    // Delete all dynamically allocated User objects
    for (auto& pair : usersMap) {
        delete pair.second;
    }
}

// Phase 1: Parsing XML into User objects
void SocialNetwork::parseUsersFromXML(XMLNode* root) {

    // ROOT → users
    for (XMLNode* usersNode : root->getChildren()) {
        if (usersNode->getName() != "users") continue;

        // users → user
        for (XMLNode* userNode : usersNode->getChildren()) {
            if (userNode->getName() != "user") continue;

            User* user = new User();

            for (XMLNode* child  : userNode->getChildren()) {

                if (child->getName() == "id") {
                    user->setID(stoi(child->getContent()));
                }
                else if (child->getName() == "name") {
                    user->setName(child->getContent());
                }
                else if (child->getName() == "followers") {
                    for (auto& followerNode : child->getChildren()) {
                        for (auto& fChild : followerNode->getChildren()) {
                            if (fChild->getName() == "id") {
                                user->addFollowerID(stoi(fChild->getContent()));
                            }
                        }
                    }
                }
            }
                int id = user->getID();
                if (id <= 0 || usersMap.count(id)) {
                delete user;
                continue;
                                       }
                   usersMap[id] = user;
        }
    }
}

// Phase 2: Linking (The Graph Logic)
void SocialNetwork::linkUsers() {

    for (auto& [idA, userA] : usersMap) {

        for (int idB : userA->getFollowersIDs()) {

            if (usersMap.count(idB)) {
                User* userB = usersMap[idB];

                // B follows A
                userA->addFollower(userB);
                userB->addFollowing(userA);
                userB->addFollowingID(idA);

            }
        }
    }
}



// Display Graph
void SocialNetwork::displayGraph() const {
    cout << "\n--- Social Network Analysis ---\n";
    for (auto const& [id, user] : usersMap) {
        cout << "User: " << user->getName() << " (ID: " << id << ")\n";

        cout << "  Followed by: ";
        for (User* f : user->getFollowers())
            cout << f->getName() << "[ID:" << f->getID() << "] ";
        cout << "\n  Following: ";
        for (User* f : user->getFollowing())
            cout << f->getName() << "[ID:" << f->getID() << "] ";
        cout << "\n-----------------------------\n";
    }
}

const map<int, User*>& SocialNetwork::getAllUsers() const {
    return usersMap;
}

//added function to build followers graph
unordered_map<int, vector<int>>
buildFollowersGraph(XMLNode* root) {
    unordered_map<int, vector<int>> graph;

    // <users>
    for (XMLNode* userNode : root->getChildren()) {
        int userId = -1;

        for (XMLNode* child : userNode->getChildren()) {
            if (child->getName() == "id") {
                userId = stoi(child->getContent());
                break;
            }
        }
        if (userId == -1) continue;

        for (XMLNode* child : userNode->getChildren()) {
            if (child->getName() == "followers") {
                for (XMLNode* followerNode : child->getChildren()) {
                    for (XMLNode* idNode : followerNode->getChildren()) {
                        if (idNode->getName() == "id") {
                            int followerId = stoi(idNode->getContent());
                            if (followerId != userId)   // safety
                                graph[userId].push_back(followerId);
                        }
                    }
                }
            }
        }
    }

    return graph;
}

void Graph(){

if (!g_root) {
        cout << "Error: Tree not built yet!\n";
        return;
    }

    g_network.parseUsersFromXML(g_root);
    g_network.linkUsers();
    g_network.displayGraph();
}
