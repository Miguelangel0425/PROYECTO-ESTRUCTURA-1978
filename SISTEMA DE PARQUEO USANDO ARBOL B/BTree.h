#pragma once
#include "BNode.h"
#include <functional>
#include <iostream>

template<typename T>
class BTree {
private:
    BNode<T>* root;
    int minDegree; // Minimum degree (t)
    
    void splitChild(BNode<T>* parent, int index, BNode<T>* child) {
        BNode<T>* newNode = new BNode<T>(child->isLeaf);
        newNode->keys.resize(minDegree - 1);
        
        // Copy the last (t-1) keys of child to newNode
        for(int j = 0; j < minDegree - 1; j++)
            newNode->keys[j] = child->keys[j + minDegree];
        
        // Copy the last t children of child to newNode
        if(!child->isLeaf) {
            newNode->children.resize(minDegree);
            for(int j = 0; j < minDegree; j++)
                newNode->children[j] = child->children[j + minDegree];
        }
        
        // Reduce the number of keys in child
        child->keys.resize(minDegree - 1);
        
        // Create space for new child
        parent->children.insert(parent->children.begin() + index + 1, newNode);
        
        // Move one key from child to parent
        parent->keys.insert(parent->keys.begin() + index, child->keys[minDegree - 1]);
    }
    
    void insertNonFull(BNode<T>* node, const T& key) {
        int i = node->keys.size() - 1;
        
        if(node->isLeaf) {
            // Find location of new key and move all greater keys ahead
            while(i >= 0 && key < node->keys[i]) {
                i--;
            }
            node->keys.insert(node->keys.begin() + i + 1, key);
        }
        else {
            // Find the child which is going to have the new key
            while(i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;
            
            if(node->children[i]->keys.size() == 2 * minDegree - 1) {
                splitChild(node, i, node->children[i]);
                if(key > node->keys[i])
                    i++;
            }
            insertNonFull(node->children[i], key);
        }
    }
    
    BNode<T>* search(BNode<T>* node, const T& key) const {
        int i = 0;
        while(i < node->keys.size() && key > node->keys[i])
            i++;
            
        if(i < node->keys.size() && key == node->keys[i])
            return node;
            
        if(node->isLeaf)
            return nullptr;
            
        return search(node->children[i], key);
    }
    
public:
    BTree(int degree) : root(nullptr), minDegree(degree) {}
    
    void insert(const T& key) {
        if(root == nullptr) {
            root = new BNode<T>(true);
            root->keys.push_back(key);
        }
        else {
            if(root->keys.size() == 2 * minDegree - 1) {
                BNode<T>* newRoot = new BNode<T>(false);
                newRoot->children.push_back(root);
                splitChild(newRoot, 0, root);
                root = newRoot;
                insertNonFull(root, key);
            }
            else {
                insertNonFull(root, key);
            }
        }
    }
    
    bool search(const T& key) const {
        return search(root, key) != nullptr;
    }
    
    void traverse(std::function<void(const T&)> processKey) const {
        if(root != nullptr) {
            traverseNode(root, processKey);
        }
    }
    
private:
    void traverseNode(BNode<T>* node, std::function<void(const T&)> processKey) const {
        int i;
        for(i = 0; i < node->keys.size(); i++) {
            if(!node->isLeaf)
                traverseNode(node->children[i], processKey);
            processKey(node->keys[i]);
        }
        
        if(!node->isLeaf)
            traverseNode(node->children[i], processKey);
    }
    
    void remove(BNode<T>* node, const T& key) {
        // Implementation of remove operation
        // This is a complex operation that requires careful handling of multiple cases
        // For now, we'll leave it as a TODO
    }
    
public:
    void remove(const T& key) {
        if(root != nullptr) {
            remove(root, key);
        }
    }
};