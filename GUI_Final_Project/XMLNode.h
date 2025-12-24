#pragma once
#ifndef XMLNODE_H
#define XMLNODE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class XMLNode {
private:
	string name;
	string content;
	vector<XMLNode*> children;
	XMLNode* parent;
public:
	// Constructors and Destructor
	XMLNode();
	XMLNode(const string& name);
	XMLNode(const string& name, const string& content);
	~XMLNode();

	// Getters
	string getName() const;
	string getContent() const;
	const vector<XMLNode*> getChildren() const;
	XMLNode* getParent() const;

	// Setters
	void setName(const string& name);
	void setContent(const string& content);
	void setParent(XMLNode* parent);

	// Child management
	void addChild(XMLNode* child);
	void removeChild(XMLNode* child);
	void removeChildByIndex(size_t index);
	void removeAllChildren();

	// Utility
	bool hasChildren() const;
	bool hasText() const;
	bool isEmpty() const;
	void print(int depth = 0) const;

	// String Conversion

	string toString(int depth = 0) const;
};


#endif // XMLNODE_H
