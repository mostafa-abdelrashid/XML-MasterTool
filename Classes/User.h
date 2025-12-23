#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include "Post.h"
using namespace std;

class User {

	int id;
	string name;
	vector<Post> posts;
	vector<int> followers_id;
	vector<User*> followers;
	vector<int> following_id;
	vector<User*> following;

public:
	User();
	User(string name, int id);
	int getID() const;
	string getName() const;
	const vector<Post>& getPosts() const;
	const vector<int>& getFollowersIDs() const;
	const vector<User*>& getFollowers() const;
	const vector<int>& getFollowingIDs() const;
	const vector<User*>& getFollowing() const;
	int getFollowersCount() const;
	int getFollowingCount() const;
	void setName(string name);
	void setID(int id);
	void addPost(const Post& newPost);
	void addFollowerID(int follower_ID);
	void addFollower(User* newFollower);
	void addFollowingID(int following_ID);
	void addFollowing(User* newFollowing);
	~User();
};

#endif

