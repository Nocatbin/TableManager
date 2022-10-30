#include <vector>
#include <iostream>
#include <memory>
#include <CLStatus.h>
#include <BPlusNode.hpp>

BPlusNode::BPlusNode(int degree, bool is_leaf)
    : degree_(degree),
      is_leaf_(is_leaf),
      next_node_(nullptr),
      prev_node_(nullptr) {}

CLStatus BPlusNode::Release() {
    // if (child_ptrs_.size() == 0){

    // }
    // for (int index = 0; index < child_ptrs_.size(); index++) {
    //     child_ptrs_[index]->Release();
    // }
}

CLStatus BPlusNode::Insert(int key, int val, NodePtr newentry) {
    if (is_leaf_) {
        // 第一个大于等于key
        std::vector<int>::iterator iter =
            std::lower_bound(keys_.begin(), keys_.end(), key);
        keys_.insert(iter, key);
        std::cout << "Insert key: " << key << std::endl;

        if (keys_.size() > degree_ - 1) {
            std::cout << "keys overflow, splitting" << std::endl;
            int t = degree_ / 2;
            newentry.reset(new BPlusNode(degree_, true));
            // val = this->keys_[t + 1];
            std::cout << "keys in newentry:";
            for (int idx = t; idx < keys_.size(); idx++) {
                newentry->keys_.push_back(this->keys_[idx]);
                std::cout << " " << this->keys_[idx];
            }
            std::cout << std::endl;
            this->keys_.resize(t + 1);
            newentry->next_node_ = this->next_node_;
            if (newentry->next_node_ != nullptr) {
                newentry->next_node_->prev_node_ = newentry;
            }
            this->next_node_ = newentry;
            // newentry->prev_node_ = this;
        }
    } else {
        // int index = 0;
        // while (index < keys_.size() && key >= keys_[index]) {
        //     index++;
        // }
        // // child_ptrs_[index]->Insert(key, val, newentry);
        // if (newentry == nullptr) {
        //     return;
        // }
        // // enter(newentry, *val, i);
        // if (keys_.size() <= degree_) {
        //     newentry = NULL;
        // } else {
        //     split(val, newentry);
        // }
    }
}

// BPlusNode::NodePtr BPlusNode::UpdateRoot(int key, NodePtr newentry) {
//     BTreeNode *root = new BTreeNode(leaf_size, non_leaf_size, false);
//     root->keys.push_back(val);
//     root->C.push_back(this);
//     root->C.push_back(newentry);
//     return root;
// }
CLStatus BPlusNode::Search() {}
