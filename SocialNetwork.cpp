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
void SocialNetwork::parseUsersFromXML(shared_ptr<XMLNode> root) {

    // ROOT → users
    for (auto& usersNode : root->children) {
        if (usersNode->tagName != "users") continue;

        // users → user
        for (auto& userNode : usersNode->children) {
            if (userNode->tagName != "user") continue;

            string userName = "";
            vector<int> followerIDs;

            // Read child nodes
            for (auto& child : userNode->children) {
                if (child->tagName == "name") {
                    userName = child->value;
                }
                else if (child->tagName == "followers") {
                    for (auto& followerNode : child->children) {
                        for (auto& fChild : followerNode->children) {
                            if (fChild->tagName == "id") {
                                followerIDs.push_back(stoi(fChild->value));
                            }
                        }
                    }
                }
            }

            // Create User with automatic unique ID
            User* user = new User(userName);

            // Store follower IDs temporarily
            for (int fid : followerIDs) {
                user->addFollowerID(fid);
            }

            // Add to map using the unique ID generated in User
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
