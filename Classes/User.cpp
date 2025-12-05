#include "User.h"

int User::next_available_id = 1;

User::User() {
	id = next_available_id++;
	name = "";
	posts = {};
	followers_id = {};
	followers = {};
}

User::User(string name) {
	id = next_available_id++;
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

void User::setName(string name) {
	this->name = name;
}

void User::addPost(const Post& newPost) {
	posts.push_back(newPost);
}

void User::addFollowerID(int follower_id) {
	followers_id.push_back(follower_id);
}

void User::addFollower(User* newFollower_ptr) {
	if (newFollower_ptr != nullptr) {
		followers.push_back(newFollower_ptr);
	}
}