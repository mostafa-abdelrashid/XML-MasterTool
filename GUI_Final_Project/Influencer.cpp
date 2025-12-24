#include "Influencer.h"

void getInfluencers(const map<int, User*>& usersMap) {
    if (usersMap.empty())  return;

    int maxFollowers = -1;
    vector<User*> influencers;

    for (auto const& [id, user] : usersMap) {
        int current = user->getFollowersCount();
        if (current > maxFollowers) { // In case there is only one influencer
            maxFollowers = current;
            influencers.clear();
            influencers.push_back(user);
        }
        else if (current == maxFollowers) { //In case there are more than one influencer
            influencers.push_back(user);
        }
    }

    for (User* u : influencers) {
        cout << "Name: " << u->getName() << ", ID: " << to_string(u->getID()) << endl;
    }
}

void most_influencer() {
    cout << "\n--- Influencer ---\n";
    getInfluencers(g_network.getAllUsers());
}