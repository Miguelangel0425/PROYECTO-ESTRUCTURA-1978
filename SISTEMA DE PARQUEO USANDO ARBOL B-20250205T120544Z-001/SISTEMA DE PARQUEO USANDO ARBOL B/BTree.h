#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <algorithm> // Asegurar que std::sort está disponible
#include "BNode.h"

template <typename T>
class BTree {
private:
    BNode<T>* root;
    int minDegree;

    void splitChild(BNode<T>* parent, int index, BNode<T>* child);
    void insertNonFull(BNode<T>* node, const T& key);
    BNode<T>* searchNode(BNode<T>* node, const T& key) const;
    void removeNode(BNode<T>* node, const T& key);
    void traverseNode(BNode<T>* node, std::function<void(const T&)> processKey) const;
     void display(BNode<T>* node, int depth, int indent) const {
        if (!node) return;

        std::string baseIndent(indent, ' ');
        
        // Imprime el nodo actual
        std::cout << baseIndent;
        std::cout << "[";
        for (size_t i = 0; i < node->keys.size(); i++) {
            if (i > 0) std::cout << "|";
            std::cout << node->keys[i].spaceNumber;
        }
        std::cout << "]\n";

        // Si tiene hijos, dibuja las líneas de conexión
        if (!node->isLeaf && !node->children.empty()) {
            // Dibuja líneas de conexión
            std::cout << baseIndent;
            int spacing = 4; // Espacio entre conectores
            for (size_t i = 0; i < node->children.size(); i++) {
                if (i == 0) std::cout << "/";
                else if (i == node->children.size() - 1) std::cout << "\\";
                else std::cout << "|";
                std::cout << std::string(spacing, ' ');
            }
            std::cout << "\n";

            // Procesa recursivamente los hijos
            int childIndentStep = 8; // Ajuste de indentación para hijos
            int baseChildIndent = indent - ((node->children.size() - 1) * childIndentStep / 2);
            
            for (size_t i = 0; i < node->children.size(); i++) {
                int childIndent = baseChildIndent + (i * childIndentStep);
                display(node->children[i], depth + 1, childIndent);
            }
        }
        };
public:
    BTree(int degree) : root(nullptr), minDegree(degree) {};
    void traverse(std::function<void(const T&)> processKey) const;
    void insert(const T& key);
    bool search(const T& key) const;
    void remove(const T& key);
    void display() const;
};

// Implementación de funciones fuera de la clase

template <typename T>
void BTree<T>::splitChild(BNode<T>* parent, int index, BNode<T>* child) {
    int t = minDegree;
    BNode<T>* newChild = new BNode<T>(child->isLeaf);

    for (int j = 0; j < t - 1; j++) {
        newChild->keys.push_back(child->keys[j + t]);
    }

    if (!child->isLeaf) {
        for (int j = 0; j < t; j++) {
            newChild->children.push_back(child->children[j + t]);
        }
    }

    child->keys.resize(t - 1);
    child->children.resize(child->isLeaf ? 0 : t);

    parent->children.insert(parent->children.begin() + index + 1, newChild);
    parent->keys.insert(parent->keys.begin() + index, child->keys[t - 1]);
}

template <typename T>
void BTree<T>::insertNonFull(BNode<T>* node, const T& key) {
    int i = node->keys.size() - 1;

    if (node->isLeaf) {
        node->keys.push_back(key);
        std::sort(node->keys.begin(), node->keys.end());
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        if (node->children[i]->keys.size() == 2 * minDegree - 1) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

template <typename T>
BNode<T>* BTree<T>::searchNode(BNode<T>* node, const T& key) const {
    if (!node) return nullptr;
    int i = 0;
    while (i < node->keys.size() && key > node->keys[i]) {
        i++;
    }
    if (i < node->keys.size() && node->keys[i] == key) {
        return node;
    }
    if (node->isLeaf) return nullptr;
    return searchNode(node->children[i], key);
}

template <typename T>
void BTree<T>::removeNode(BNode<T>* node, const T& key) {
    int idx = 0;
    while (idx < node->keys.size() && node->keys[idx] < key) {
        idx++;
    }
    if (idx < node->keys.size() && node->keys[idx] == key) {
        if (node->isLeaf) {
            node->keys.erase(node->keys.begin() + idx);
        } else {
            // Implementar eliminación en nodos internos
        }
    } else {
        if (node->isLeaf) return;
        removeNode(node->children[idx], key);
    }
}

template <typename T>
    void BTree<T>::display() const {
        if (root) {
            std::cout << "\n";  // Añade un salto de línea antes de empezar
            display(root, 0, 40);  // Comienza con una indentación base de 40
            std::cout << "\n";  // Añade un salto de línea al final
        } else {
            std::cout << "Arbol vacio\n";
        }
    }

template <typename T>
void BTree<T>::traverse(std::function<void(const T&)> processKey) const {
    traverseNode(root, processKey);
}

template <typename T>
void BTree<T>::traverseNode(BNode<T>* node, std::function<void(const T&)> processKey) const {
    if (node) {
        for (size_t i = 0; i < node->keys.size(); i++) {
            if (!node->isLeaf) {
                traverseNode(node->children[i], processKey);
            }
            processKey(node->keys[i]);
        }
        if (!node->isLeaf) {
            traverseNode(node->children[node->keys.size()], processKey);
        }
    }
}

template <typename T>
void BTree<T>::insert(const T& key) {
    if (!root) {
        root = new BNode<T>(true);
        root->keys.push_back(key);
    } else {
        if (root->keys.size() == 2 * minDegree - 1) {
            BNode<T>* newRoot = new BNode<T>(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0, root);
            root = newRoot;
        }
        insertNonFull(root, key);
    }
}

template <typename T>
void BTree<T>::remove(const T& key) {
    if (!root) return;
    removeNode(root, key);
    if (root->keys.empty()) {
        BNode<T>* oldRoot = root;
        if (root->isLeaf) {
            root = nullptr;
        } else {
            root = root->children[0];
        }
        delete oldRoot;
    }
}

template <typename T>
bool BTree<T>::search(const T& key) const {
    return searchNode(root, key) != nullptr;
}
