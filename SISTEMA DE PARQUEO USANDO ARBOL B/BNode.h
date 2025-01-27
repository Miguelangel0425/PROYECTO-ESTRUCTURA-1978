#pragma once
#include <vector>
#include <functional>

template<typename T>
class BNode {
public:
    std::vector<T> keys;
    std::vector<BNode<T>*> children;
    bool isLeaf;
    
    BNode(bool leaf = true) : isLeaf(leaf) {}
    ~BNode() {
        for(auto child : children) {
            delete child;
        }
    }
};