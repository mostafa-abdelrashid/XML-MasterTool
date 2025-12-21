#ifndef SOCIAL_NETWORK_H
#define SOCIAL_NETWORK_H

#include <vector>
#include <string>
#include <map>
#include <memory>
#include "User.h"
#include "Token.h"

using namespace std;

class SocialNetwork {
private:
    // Map helps find a User pointer quickly using their unique ID
    map<int, User*> usersMap;

public:
    SocialNetwork();
    ~SocialNetwork();

    // Phase 1
    void parseUsersFromXML(shared_ptr<XMLNode> root);

    // Phase 2
    void linkUsers();

    // Utility to see the results
    void displayGraph() const;

    // Get all users for analysis
    const map<int, User*>& getAllUsers() const;
};

#endif
