#pragma once

#include <vector>
#include <memory>
#include <CLStatus.h>

class BPlusNode : public std::enable_shared_from_this<BPlusNode> {
 public:
    using NodePtr = std::shared_ptr<BPlusNode>;

    std::vector<int> keys_;
    std::vector<NodePtr> child_ptrs_;
    int degree_;

    bool is_leaf_;
    NodePtr prev_node_;
    NodePtr next_node_;

    BPlusNode() = delete;
    BPlusNode(int node_size, bool is_leaf);
    CLStatus Insert(int key, int val, NodePtr &newentry);
    NodePtr UpdateRoot(int key, NodePtr &newentry);
    void Traverse();
    CLStatus Search();
    CLStatus Release();

    //  private:
};
