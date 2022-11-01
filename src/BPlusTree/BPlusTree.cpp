#include <BPlusTree.hpp>

BPlusTree::BPlusTree(int degree) {
    if (root != nullptr) {
        return;
    }
    root.reset(new BPlusNode(degree, true));
}

void BPlusTree::Insert(int key, int value) {
    BPlusNode::NodePtr newentry;
    root->Insert(key, value, newentry);
    if (newentry != nullptr) {
        root = root->UpdateRoot(newentry);
    }
}

void BPlusTree::Print() { root->Traverse(); }