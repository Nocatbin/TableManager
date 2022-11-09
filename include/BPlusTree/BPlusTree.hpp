#pragma once
// #include <iostream>
#include <functional>
#include <CLMutex.h>
#include <CLCriticalSection.h>
#include <BPlusNode.hpp>

class BPlusTree {
 public:
    using TraverseFunc = std::function<void(BPlusNode::NodePtr node)>;

    BPlusNode::NodePtr root;

    BPlusTree(int degree);
    void Insert(long key, long value);
    void DebugPrint();

    static void GenerateRowNumberFunc(BPlusNode::NodePtr node);
    static void LevelPrintFunc(BPlusNode::NodePtr node);
    void LevelTraverse(TraverseFunc func);
    int Degree();

 private:
    CLMutex tree_mutex;
};