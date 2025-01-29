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
        for(int j = 0; j < minDegree - 1; j++){
            newNode->keys[j] = child->keys[j + minDegree];
        
        }
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
        if (!node) return;

        // Encuentra el índice donde la clave debería estar
        int idx = 0;
        while (idx < node->keys.size() && node->keys[idx] < key) 
            idx++;

        // Si la clave está en este nodo
        if (idx < node->keys.size() && node->keys[idx] == key) {
            if (node->isLeaf) {
                // Caso 1: Si es una hoja, simplemente elimina la clave
                node->keys.erase(node->keys.begin() + idx);
            } else {
                // Caso 2: Si no es una hoja, reemplaza con el predecesor o sucesor
                if (node->children[idx]->keys.size() >= minDegree) {
                    // Usa el predecesor
                    T pred = getPred(node, idx);
                    node->keys[idx] = pred;
                    remove(node->children[idx], pred);
                } else if (node->children[idx + 1]->keys.size() >= minDegree) {
                    // Usa el sucesor
                    T succ = getSucc(node, idx);
                    node->keys[idx] = succ;
                    remove(node->children[idx + 1], succ);
                } else {
                    // Combina los hijos y elimina la clave
                    merge(node, idx);
                    remove(node->children[idx], key);
                }
            }
        } else {
            if (node->isLeaf) {
                return; // La clave no está en el árbol
            }

            // Determina si la clave está en el último hijo
            bool flag = (idx == node->keys.size());

            // Si el hijo tiene menos del mínimo de claves, rellenarlo
            if (node->children[idx]->keys.size() < minDegree) {
                fill(node, idx);
            }

            // Si el último hijo se ha combinado, necesitamos ir al hijo anterior
            if (flag && idx > node->keys.size()) {
                remove(node->children[idx - 1], key);
            } else {
                remove(node->children[idx], key);
            }
        }
    }

    T getPred(BNode<T>* node, int idx) {
        BNode<T>* curr = node->children[idx];
        while (!curr->isLeaf) {
            curr = curr->children[curr->keys.size()];
        }
        return curr->keys[curr->keys.size() - 1];
    }

    T getSucc(BNode<T>* node, int idx) {
        BNode<T>* curr = node->children[idx + 1];
        while (!curr->isLeaf) {
            curr = curr->children[0];
        }
        return curr->keys[0];
    }

    void fill(BNode<T>* node, int idx) {
        if (idx != 0 && node->children[idx - 1]->keys.size() >= minDegree) {
            borrowFromPrev(node, idx);
        } else if (idx != node->keys.size() && node->children[idx + 1]->keys.size() >= minDegree) {
            borrowFromNext(node, idx);
        } else {
            if (idx != node->keys.size()) {
                merge(node, idx);
            } else {
                merge(node, idx - 1);
            }
        }
    }

    void borrowFromPrev(BNode<T>* node, int idx) {
        BNode<T>* child = node->children[idx];
        BNode<T>* sibling = node->children[idx - 1];

        child->keys.insert(child->keys.begin(), node->keys[idx - 1]);
        node->keys[idx - 1] = sibling->keys.back();
        sibling->keys.pop_back();

        if (!child->isLeaf) {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }
    }

    void borrowFromNext(BNode<T>* node, int idx) {
        BNode<T>* child = node->children[idx];
        BNode<T>* sibling = node->children[idx + 1];

        child->keys.push_back(node->keys[idx]);
        node->keys[idx] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());

        if (!child->isLeaf) {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }
    }

    void merge(BNode<T>* node, int idx) {
        BNode<T>* child = node->children[idx];
        BNode<T>* sibling = node->children[idx + 1];

        child->keys.push_back(node->keys[idx]);
        
        for (const T& key : sibling->keys) {
            child->keys.push_back(key);
        }

        if (!child->isLeaf) {
            for (auto* ptr : sibling->children) {
                child->children.push_back(ptr);
            }
        }

        node->keys.erase(node->keys.begin() + idx);
        node->children.erase(node->children.begin() + idx + 1);
        delete sibling;
    }
    
public:
    void remove(const T& key) {
        if(root != nullptr) {
            remove(root, key);
        }
    }
};