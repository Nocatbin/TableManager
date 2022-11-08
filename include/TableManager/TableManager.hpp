#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <random>
#include <CLStatus.h>
#include <BPlusTree.hpp>

class TableManager {
 public:
    // TableManager() = delete;
    TableManager(std::string path);
    ~TableManager();

    static constexpr int ATTRIBUTE_NUMBER = 10;
    static constexpr int INIT_ROW_NUMBER = 10;
    static constexpr int TREE_DEGREE = 3;

    CLStatus AppendEntry(std::string entry);

    bool SearchFromFile(const int attr_index,
                        const long lower,
                        const long upper);
    void WriteFileCallback(BPlusNode::NodePtr node, std::ofstream *file);

 private:
    std::string path_;
    int table_file_;
    std::mt19937 rand_generator{std::random_device{}()};  // ret long
    std::ifstream input_index_file_;
    std::ofstream output_index_file_;
    std::shared_ptr<BPlusTree> tree_;

    CLStatus createTable();
    bool openIndexFile(const int attr_index);
    CLStatus buildIndex(const int attr_index);
    bool dumpIndexToFile(const int attr_index);
};
