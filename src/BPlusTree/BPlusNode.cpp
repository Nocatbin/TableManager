#include <vector>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <queue>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <CLStatus.h>
#include <BPlusNode.hpp>

BPlusNode::BPlusNode(int degree, bool is_leaf)
    : degree_(degree),
      is_leaf_(is_leaf),
      next_node_(nullptr),
      prev_node_(nullptr) {}

CLStatus BPlusNode::Insert(long key, long val, NodePtr &newentry) {
    if (is_leaf_) {
        // 第一个大于等于key的index
        int iter_index = 0;
        for (iter_index = 0; iter_index < keys_.size(); iter_index++) {
            if (keys_[iter_index] >= key) {
                break;
            }
        }
        keys_.insert(keys_.begin() + iter_index, key);
        values_.insert(values_.begin() + iter_index, val);
        // std::cout << "Insert key: " << key << std::endl;

        // keys overflow, splitting
        if (keys_.size() > degree_ - 1) {
            // std::cout << "keys overflow, splitting" << std::endl;
            int t = degree_ / 2;
            newentry.reset(new BPlusNode(degree_, true));
            // std::cout << "keys in newentry:";
            // new root key for adding a key to its father node
            newentry->new_root_key = this->keys_[t];
            // append half key-value to newentry node
            for (int idx = t; idx < keys_.size(); idx++) {
                newentry->keys_.push_back(this->keys_[idx]);
                newentry->values_.push_back(this->values_[idx]);
                // std::cout << " " << this->keys_[idx];
            }
            // std::cout << std::endl;
            // resize origin half key-value
            this->keys_.resize(t);
            this->values_.resize(t);
            // link table for leaf node, using shared_ptrs
            newentry->next_node_ = this->next_node_;
            if (newentry->next_node_ != nullptr) {
                newentry->next_node_->prev_node_ = newentry;
            }
            this->next_node_ = newentry;
            newentry->prev_node_ = shared_from_this();
        }
    } else {
        // nonleaf node case
        int index = 0;
        // find first key <= keys
        while (index < keys_.size() && key >= keys_[index]) {
            index++;
        }
        // recursive call Insert until leaf node
        child_ptrs_[index]->Insert(key, val, newentry);
        // newentry node will update if any node had splitted
        if (newentry != nullptr) {
            keys_.insert(keys_.begin() + index, newentry->new_root_key);
            child_ptrs_.insert(child_ptrs_.begin() + index + 1, newentry);
        } else {
            return CLStatus(0, 0);
        }
        // nonleaf node overflow after insert, splitting
        if (keys_.size() > degree_ - 1) {
            int t = degree_ / 2;
            newentry.reset(new BPlusNode(degree_, false));
            newentry->new_root_key = this->keys_[t];
            for (int i = t + 1; i < keys_.size(); i++) {
                newentry->keys_.push_back(this->keys_[i]);
            }
            this->keys_.resize(t);
            for (int i = t + 1; i < child_ptrs_.size(); i++) {
                newentry->child_ptrs_.push_back(this->child_ptrs_[i]);
            }
            this->child_ptrs_.resize(t + 1);
        } else {
            newentry.reset();
        }
    }
}

void BPlusNode::Traverse() {
    for (int idx = 0; idx < keys_.size(); idx++) {
        std::cout << " " << keys_[idx];
    }
    std::cout << std::endl;
    if (is_leaf_) {
        std::cout << "Leaf Printed!" << std::endl;
        return;
    } else {
        for (int idx = 0; idx < child_ptrs_.size(); idx++) {
            child_ptrs_[idx]->Traverse();
        }
    }
}

BPlusNode::NodePtr BPlusNode::UpdateRoot(NodePtr &newentry) {
    BPlusNode::NodePtr root = std::make_shared<BPlusNode>(degree_, false);
    // std::cout << "updating root" << std::endl;
    if (newentry->new_root_key != -1) {
        root->keys_.emplace_back(newentry->new_root_key);
    } else {
        std::cerr << "Fatal, root key error" << std::endl;
    }
    root->child_ptrs_.emplace_back(shared_from_this());
    root->child_ptrs_.emplace_back(newentry);
    return root;
}
