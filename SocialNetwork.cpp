#include "SocialNetwork.h"
#include <iostream>
using namespace std;

SocialNetwork::SocialNetwork() {}

SocialNetwork::~SocialNetwork() {
    // Delete all dynamically allocated User objects
    for (auto& pair : usersMap) {
        delete pair.second;
    }
}

// Phase 1
void SocialNetwork::parseUsersFromXML(XMLNode* root) {

    for (XMLNode* usersNode : root->getChildren()) {
        if (usersNode->getName() != "users") continue;

        for (XMLNode* userNode : usersNode->getChildren()) {
            if (userNode->getName() != "user") continue;

            string userName = "";
            vector<int> followerIDs;

            for (XMLNode* child : userNode->getChildren()) {
                if (child->getName() == "name") {
                    userName = child->getContent();
                }
                else if (child->getName() == "followers") {
                    for (XMLNode* followerNode : child->getChildren()) {
                        for (XMLNode* fChild : followerNode->getChildren()) {
                            if (fChild->getName() == "id") {
                                followerIDs.push_back(stoi(fChild->getContent()));
                            }
                        }
                    }
                }
            }

            User* user = new User(userName);

            for (int fid : followerIDs)
                user->addFollowerID(fid);

            usersMap[user->getID()] = user;
        }
    }
}


// Phase 2: Linking users
void SocialNetwork::linkUsers() {
    for (auto& [idA, userA] : usersMap) {
        for (int idB : userA->getFollowersIDs()) {
            if (usersMap.count(idB)) {
                User* userB = usersMap[idB];
                // B follows A
                userA->addFollower(userB);
                userB->addFollowing(userA);
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
