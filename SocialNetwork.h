#ifndef SOCIALNETWORK_H
#define SOCIALNETWORK_H

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "User.h"
#include "Token.h" 

using namespace std;

class SocialNetwork {
private:
    map<int, User*> usersMap; 

public:
    SocialNetwork();
    ~SocialNetwork();

    void parseUsersFromXML(shared_ptr<XMLNode> root);

    void linkUsers();

    void displayGraph() const;

    const map<int, User*>& getAllUsers() const;
};

#endif

