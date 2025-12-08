#ifndef TREE_H
#define TREE_H

#include <vector>
#include <iostream>
#include <string>

// =====================
// Node Structure
// =====================
template<typename T>
struct Node {
    T value{};                       // The data stored in the node
    std::vector<Node<T>*> childs;    // List of children

    Node(T val);                     // Constructor
    void addChild(Node<T>* child);   // Add child to node
};

// Node IMPLEMENTATION
template<typename T>
Node<T>::Node(T val) : value(val) {}

template<typename T>
void Node<T>::addChild(Node<T>* child) {
    childs.push_back(child);
}


// =====================
// Tree Class
// =====================
template<typename T>
class Tree {
public:
    Tree();                                 // Empty tree
    Tree(Node<T>* rootNode);                // Construct from root
    Tree(const Tree<T>& other);             // Deep copy constructor
    ~Tree();                                // Destructor

    void printPreorder();                   // Print tree preorder

private:
    Node<T>* root{};                        // Root node of the tree

    Node<T>* copyNode(Node<T>* node);       // Deep copy helper
    void deleteNode(Node<T>* node);         // Free memory recursively
    void printPreorderHelper(Node<T>* node);// Recursive print
};


// ============
// Tree IMPLEMENTATION
// ============

template<typename T>
Tree<T>::Tree() : root(nullptr) {}

template<typename T>
Tree<T>::Tree(Node<T>* rootNode) : root(rootNode) {}

template<typename T>
Tree<T>::Tree(const Tree<T>& other) {
    root = copyNode(other.root);            // Deep copy root
}

template<typename T>
Tree<T>::~Tree() {
    deleteNode(root);                       // Delete full tree
}

template<typename T>
void Tree<T>::printPreorder() {
    printPreorderHelper(root);
    std::cout << "\n";
}

template<typename T>
Node<T>* Tree<T>::copyNode(Node<T>* node) {
    if (!node) return nullptr;

    Node<T>* newNode = new Node<T>(node->value);  // Copy node value

    // Recursively copy children
    for (auto child : node->childs)
        newNode->childs.push_back(copyNode(child));

    return newNode;
}

template<typename T>
void Tree<T>::deleteNode(Node<T>* node) {
    if (!node) return;

    for (auto child : node->childs)
        deleteNode(child);

    delete node;                              // Delete current node
}

template<typename T>
void Tree<T>::printPreorderHelper(Node<T>* node) {
    if (!node) return;

    // If leaf, print value only
    if (node->childs.empty())
        std::cout << node->value << " ";
    else
        std::cout << "<" << node->value << "> ";

    // Print all children
    for (auto child : node->childs)
        printPreorderHelper(child);
}

#endif // TREE_H
