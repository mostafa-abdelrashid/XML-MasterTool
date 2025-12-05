#include "Post.h"

Post::Post() {
	body = "";
	topics = {};
}

Post::Post(const string& postBody, const vector<string>& postTopics) {
	body = postBody;
	topics = postTopics;
}

string Post::getBody() const {
	return body;
}

const vector<string>& Post::getTopics() const {
	return topics;
}

void Post::setBody(const string& newBody) {
	body = newBody;
}

void Post::setTopics(const vector<string>& newTopics) {
	topics = newTopics;
}

void Post::addTopic(const string& topicName) {
	topics.push_back(topicName);
}