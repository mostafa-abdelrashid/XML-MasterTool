#include "SocialNetwork.h"

using namespace std;

SocialNetwork::SocialNetwork() {}

// Clean up dynamically allocated User objects
SocialNetwork::~SocialNetwork() {
    for (auto& pair : usersMap) {
        delete pair.second;
    }
}

// Phase 1: Parsing XML into User objects
void SocialNetwork::parseUsersFromXML(shared_ptr<XMLNode> root) {

    // ROOT → users
    for (auto& usersNode : root->children) {
        if (usersNode->tagName != "users") continue;

        // users → user
        for (auto& userNode : usersNode->children) {
            if (userNode->tagName != "user") continue;

            User* user = new User();

            for (auto& child : userNode->children) {

               if (child->tagName == "id") {
                    int id = stoi(child->value);
                    user = new User(id);                }
                 if (child->tagName == "name") {
                    user->setName(child->value);
                }
                else if (child->tagName == "followers") {
                    for (auto& followerNode : child->children) {
                        for (auto& fChild : followerNode->children) {
                            if (fChild->tagName == "id") {
                                user->addFollowerID(stoi(fChild->value));
                            }
                        }
                    }
                }
            }
            usersMap[user->getID()] = user;
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
            }
        }
    }
}
void SocialNetwork::displayGraph() const {
    cout << "\n--- Social Network Analysis ---\n";
    for (auto const& [id, user] : usersMap) {
        cout << "User: " << user->getName() << " (ID: " << id << ")\n";

        cout << "  Followed by: ";
        for (User* f : user->getFollowers())
            cout << f->getName() << "[ID:" << f->getID() << "] "; // Added ID

        cout << "\n  Following: ";
        for (User* f : user->getFollowing())
            cout << f->getName() << "[ID:" << f->getID() << "] "; // Added ID
        cout << "\n-----------------------------\n";
    }
}
const map<int, User*>& SocialNetwork::getAllUsers() const {
    return usersMap;
}
