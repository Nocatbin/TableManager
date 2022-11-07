#include <iostream>
#include <queue>
#include <BPlusTree.hpp>

BPlusTree::BPlusTree(int degree) {
    if (root != nullptr) {
        return;
    }
    root.reset(new BPlusNode(degree, true));
}

void BPlusTree::Insert(long key, long value) {
    BPlusNode::NodePtr newentry;
    root->Insert(key, value, newentry);
    if (newentry != nullptr) {
        root = root->UpdateRoot(newentry);
    }
}

void BPlusTree::DebugPrint() {
    std::cout << "printing tree" << std::endl;
    root->Traverse();
}

// labeling nodes with row number in index file
void BPlusTree::GenerateRowNumberFunc(BPlusNode::NodePtr node) {
    static int row_number = 0;
    node->row_number_in_file_ = row_number++;
}

void BPlusTree::LevelPrintFunc(BPlusNode::NodePtr node) {
    auto &keys = node->keys_;
    for (int idx = 0; idx < keys.size(); idx++) {
        std::cout << keys[idx] << " ";
    }
    std::cout << std::endl;
}

// Print & Label row number in index file for each node
void BPlusTree::levelTraverse(TraverseFunc func) {
    std::queue<BPlusNode::NodePtr> node_queue;
    // push root into queue
    if (root != nullptr) {
        node_queue.push(root);
    }
    // judge is queue is empty
    while (!node_queue.empty()) {
        auto &node = node_queue.front();

        // call traverse func for different purpose
        func(node);

        for (int idx = 0; idx < node->child_ptrs_.size(); idx++) {
            node_queue.push(node->child_ptrs_[idx]);
        }
        // pop traversed nodes
        node_queue.pop();
    }
}

int BPlusTree::Degree() { return root->degree_; }
