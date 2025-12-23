#include "ActiveUser.h"

void getMostActive(const map<int, User*>& usersMap) {
    if (usersMap.empty()) return ;

    int maxFollowing = -1;
    vector<User*> activeUsers;

    for (auto const& [id, user] : usersMap) {
        int current = user->getFollowing();
        if (current > maxFollowing) { // In case there is only one most active user
            maxFollowing = current;
            activeUsers.clear();
            activeUsers.push_back(user);
        }
        else if (current == maxFollowing) { //In case there are more than one active user
            activeUsers.push_back(user);
        }
    }

    for (User* u : activeUsers) {
        cout << "Name: " << u->getName() << ", ID: " << to_string(u->getID()) << endl;
    }
}

void most_activeUser() {
    cout << "\n--- Most Active User ---\n";
    getMostActive(g_network.getAllUsers());
}

