#pragma once

#include <BPlusNode.hpp>

class BPlusTree {
 public:
    BPlusNode::NodePtr root;
    BPlusTree(int degree);
    void Insert(long key, long value);
    void Print();
};