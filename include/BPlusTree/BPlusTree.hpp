#pragma once

#include <BPlusNode.hpp>

class BPlusTree {
 public:
    BPlusNode::NodePtr root;
    BPlusTree(int degree);
    void Insert(int key, int value);
    void Print();
};