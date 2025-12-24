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
    if (!root) return;

    XMLNode* usersNode = nullptr;

    // FIX: Check if the root itself is <users>
    if (root->getName() == "users") {
        usersNode = root;
    } else {
        // Otherwise, look for <users> inside the children (legacy support for XMLTreeBuilder)
        for (XMLNode* child : root->getChildren()) {
            if (child->getName() == "users") {
                usersNode = child;
                break;
            }
        }
    }

    if (!usersNode) return; // No <users> tag found

    // users -> user
    for (XMLNode* userNode : usersNode->getChildren()) {
        if (userNode->getName() != "user") continue;

        User* user = new User();

        for (XMLNode* child : userNode->getChildren()) {
            if (child->getName() == "id") {
                try {
                    user->setID(stoi(child->getContent()));
                } catch (...) { } 
            }
            else if (child->getName() == "name") {
                user->setName(child->getContent());
            }
            else if (child->getName() == "followers") {
                for (auto& followerNode : child->getChildren()) {
                    for (auto& fChild : followerNode->getChildren()) {
                        if (fChild->getName() == "id") {
                            try {
                                user->addFollowerID(stoi(fChild->getContent()));
                            } catch (...) { }
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

// Function to build followers graph (Used by Suggest/Mutual/Draw)
unordered_map<int, vector<int>> buildFollowersGraph(XMLNode* root) {
    unordered_map<int, vector<int>> graph;
    if (!root) return graph;

    XMLNode* usersNode = nullptr;
    if (root->getName() == "users") {
        usersNode = root;
    } else {
        for (XMLNode* child : root->getChildren()) {
            if (child->getName() == "users") {
                usersNode = child;
                break;
            }
        }
    }

    if (!usersNode) return graph;

    for (XMLNode* userNode : usersNode->getChildren()) {
        int userId = -1;
        for (XMLNode* child : userNode->getChildren()) {
            if (child->getName() == "id") {
                try { userId = stoi(child->getContent()); } catch(...) {}
                break;
            }
        }
        if (userId == -1) continue;

        for (XMLNode* child : userNode->getChildren()) {
            if (child->getName() == "followers") {
                for (XMLNode* followerNode : child->getChildren()) {
                    for (XMLNode* idNode : followerNode->getChildren()) {
                        if (idNode->getName() == "id") {
                            try {
                                int followerId = stoi(idNode->getContent());
                                if (followerId != userId)
                                    graph[userId].push_back(followerId);
                            } catch(...) {}
                        }
                    }
                }
            }
        }
    }
    return graph;
}

// Global graph helper
void Graph() {
    // Note: g_root is from XMLTreeBuilder, which might not be initialized in CLI mode.
    // This function is kept for legacy/GUI support if needed.
    // In CLI, we use loadNetwork() in main.cpp instead.
    cout << "Use CLI commands for graph analysis.\n";
}