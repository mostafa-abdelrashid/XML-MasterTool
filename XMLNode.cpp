#include "XMLNode.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string escapeXML(const string& input) {
    string output;
    for (char c : input) {
        switch (c) {
        case '&': output += "&amp;"; break;
        case '<': output += "&lt;"; break;
        case '>': output += "&gt;"; break;
        case '\"': output += "&quot;"; break;
        case '\'': output += "&apos;"; break;
        default: output += c; break;
        }
    }
    return output;
}

XMLNode::XMLNode() : name(""), content(""), parent(nullptr) {}
XMLNode::XMLNode(const string& name) : name(name), content(""), parent(nullptr) {}
XMLNode::XMLNode(const string& name, const string& content) : name(name), content(content), parent(nullptr) {}
XMLNode::~XMLNode() { removeAllChildren(); }

string XMLNode::getName() const { return name; }
string XMLNode::getContent() const { return content; }
const vector<XMLNode*> XMLNode::getChildren() const { return children; }
XMLNode* XMLNode::getParent() const { return parent; }

void XMLNode::setName(const string& name) { this->name = name; }
void XMLNode::setContent(const string& content) { this->content = content; }
void XMLNode::setParent(XMLNode* parent) { this->parent = parent; }

void XMLNode::addChild(XMLNode* child) {
    if (child) {
        child->setParent(this);
        children.push_back(child);
    }
}

void XMLNode::removeChild(XMLNode* child) {
    auto it = find(children.begin(), children.end(), child);
    if (it != children.end()) {
        (*it)->setParent(nullptr);
        children.erase(it);
    }
}

void XMLNode::removeChildByIndex(size_t index) {
    if (index < children.size()) {
        children[index]->setParent(nullptr);
        children.erase(children.begin() + index);
    }
}

void XMLNode::removeAllChildren() {
    for (XMLNode* child : children) delete child;
    children.clear();
}

bool XMLNode::hasChildren() const { return !children.empty(); }
bool XMLNode::hasText() const { return !content.empty(); }
bool XMLNode::isEmpty() const { return !hasChildren() && !hasText(); }

void XMLNode::print(int depth) const {
    string indent(depth * 2, ' ');
    cout << indent << "<" << name << ">";

    if (hasText()) {
        cout << escapeXML(content);
        if (hasChildren()) {
            cout << endl;
            for (const XMLNode* child : children) child->print(depth + 1);
            cout << indent;
        }
    }
    else if (hasChildren()) {
        cout << endl;
        for (const XMLNode* child : children) child->print(depth + 1);
        cout << indent;
    }

    cout << "</" << name << ">" << endl;
}

string XMLNode::toString(int depth) const {
    string output;
    string indent(depth * 2, ' ');
    output += indent + "<" + name + ">";

    if (hasText()) {
        output += escapeXML(content);
        if (hasChildren()) {
            output += "\n";
            for (auto child : children) output += child->toString(depth + 1);
            output += indent;
        }
    }
    else if (hasChildren()) {
        output += "\n";
        for (const XMLNode* child : children) output += child->toString(depth + 1);
        output += indent;
    }

    output += "</" + name + ">\n";
    return output;
}
