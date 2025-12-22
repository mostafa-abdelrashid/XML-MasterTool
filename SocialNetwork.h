#ifndef SOCIALNETWORK_H
#define SOCIALNETWORK_H

#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Classes\User.h"
#include "Token.h"
#include "XMLNode.h"


using namespace std;

class SocialNetwork {
private:
    map<int, User*> usersMap;

public:
    SocialNetwork();
    ~SocialNetwork();

    void parseUsersFromXML(XMLNode* root);

    void linkUsers();

    void displayGraph() const;

    const map<int, User*>& getAllUsers() const;
};

//added function to build followers graph
std::unordered_map<int, std::vector<int>>
buildFollowersGraph(XMLNode* root);
void Graph();
extern SocialNetwork g_network;

#endif

