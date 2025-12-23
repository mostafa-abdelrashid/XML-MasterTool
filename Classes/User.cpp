#include "User.h"

User::User() {
	id = 0;
	name = "";
	posts = {};
	followers_id = {};
	followers = {};
}

User::User(string name, int id) {
	this->id = id;
	this->name = name;
	posts = {};
	followers_id = {};
	followers = {};
}

User::~User() {}

int User::getID() const {
	return id;
}

string User::getName() const {
	return name;
}

const vector<Post>& User::getPosts() const {
	return posts;
}

const vector<int>& User::getFollowersIDs() const {
	return followers_id;
}
const vector<User*>& User::getFollowers() const {
	return followers;
}

const vector<int>& User::getFollowingIDs() const {
	return following_id;
}
const vector<User*>& User::getFollowing() const { 
	return following;
}

int User::getFollowersCount() const { 
	return followers_id.size();
}

int User::getFollowingCount() const { 
	return following_id.size();
}

void User::setName(string name) {
	this->name = name;
}

void User::setID(int id) {
	this->id = id;
}

void User::addPost(const Post& newPost) {
	posts.push_back(newPost);
}

void User::addFollowerID(int follower_ID) {
	followers_id.push_back(follower_ID);
}

void User::addFollower(User* newFollower_ptr) {
	if (newFollower_ptr != nullptr) {
		followers.push_back(newFollower_ptr);
	}
}

void User::addFollowingID(int following_ID) { 
	following_id.push_back(following_ID);
}

void User::addFollowing(User* newFollowing_ptr) { 
	if (newFollowing_ptr != nullptr) {
		following.push_back(newFollowing_ptr);
	}
}

