#ifndef POST_H
#define POST_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Post {
	string body;
	vector<string> topics;

public:
	Post();
	Post(const string& postBody, const vector<string>& postTopics);
	string getBody() const;
	const vector<string>& getTopics() const;
	void setBody(const string& newBody);
	void setTopics(const vector<string>& newTopics);
	void addTopic(const string& topicName);
};

#endif