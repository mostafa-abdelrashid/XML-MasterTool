#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include "Post.h"
using namespace std;

class User {

	int id;
	static int next_available_id;
	string name;
	vector<Post> posts;
	vector<int> followers_id;
	vector<User*> followers;

public:
	User();
	User(string name);
	int getID() const;
	string getName() const;
	const vector<Post>& getPosts() const;
	const vector<int>& getFollowersIDs() const;
	const vector<User*>& getFollowers() const;
	void setName(string name);
	void addPost(const Post& newPost);
	void addFollowerID(int follower_id);
	void addFollower(User* newFollower);
	~User();
};

#endif