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
        std::cout << "Insert key: " << key << std::endl;

        if (keys_.size() > degree_ - 1) {
            std::cout << "keys overflow, splitting" << std::endl;
            int t = degree_ / 2;
            newentry.reset(new BPlusNode(degree_, true));
            // val = this->keys_[t + 1];
            std::cout << "keys in newentry:";
            newentry->new_root_key = this->keys_[t];
            for (int idx = t; idx < keys_.size(); idx++) {
                newentry->keys_.push_back(this->keys_[idx]);
                newentry->values_.push_back(this->values_[idx]);
                std::cout << " " << this->keys_[idx];
            }
            std::cout << std::endl;
            this->keys_.resize(t);
            this->values_.resize(t);
            newentry->next_node_ = this->next_node_;
            if (newentry->next_node_ != nullptr) {
                newentry->next_node_->prev_node_ = newentry;
            }
            this->next_node_ = newentry;
            newentry->prev_node_ = shared_from_this();
        }
    } else {
        int index = 0;
        while (index < keys_.size() && key >= keys_[index]) {
            index++;
        }
        child_ptrs_[index]->Insert(key, val, newentry);
        if (newentry != nullptr) {
            keys_.insert(keys_.begin() + index, newentry->new_root_key);
            child_ptrs_.insert(child_ptrs_.begin() + index + 1, newentry);

        } else {
            return CLStatus(0, 0);
        }
        if (keys_.size() > degree_ - 1) {
            int t = degree_ / 2;
            newentry.reset(new BPlusNode(degree_, false));
            // *val = this->keys[t];
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
    std::cout << "updating root" << std::endl;
    if (newentry->new_root_key != -1) {
        root->keys_.emplace_back(newentry->new_root_key);
    } else {
        std::cerr << "Fatal, root key error" << std::endl;
    }
    root->child_ptrs_.emplace_back(shared_from_this());
    root->child_ptrs_.emplace_back(newentry);
    // auto& vec = root->child_ptrs_;
    // for (int idx = 0; idx < vec.size(); idx++) {
    //     for (int y = 0; y < vec[idx]->keys_.size(); y++) {
    //         std::cout << "left tree: " << vec[idx]->keys_[y] << std::endl;
    //     }
    // }

    return root;
}

// std::string BPlusNode::getIndexString() {}

bool BPlusNode::WriteIndexToFile(int fd, int &latest_row) {
    std::stringstream ss;
    ss << is_leaf_ ? 1 : 0;
    ss << "," << keys_.size();
    for (int idx = 0; idx < keys_.size(); idx++) {
        ss << "," << keys_[idx];
    }
    std::string output = ss.str();
    write(fd, output.c_str(), strlen(output.c_str()));
    if (is_leaf_) {
    } else {
    }
}
