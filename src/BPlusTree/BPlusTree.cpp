#include <BPlusTree.hpp>

BPlusTree::BPlusTree(int degree) {
    if (root != nullptr) {
        return;
    }
    root.reset(new BPlusNode(3, true));
    // if (newentry != nullptr) {
    //     root = root->makenewroot(val, newentry);
    // }
}

// void BPlusTree::Insert(int key, int value) {}