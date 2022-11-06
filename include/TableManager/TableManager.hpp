#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <CLStatus.h>
#include <BPlusTree.hpp>

class TableManager {
 public:
    // TableManager() = delete;
    TableManager(std::string path);
    ~TableManager();

    static constexpr int ATTRIBUTE_LENGTH = 10;
    static constexpr int INIT_ROW_NUMBER = 10;
    static constexpr int TREE_DEGREE = 3;

    CLStatus CreateTable();
    CLStatus AppendEntry(std::string entry);
    CLStatus BuildIndex(const int attr_index);
    bool OpenIndexFile(const int attr_index);
    bool DumpIndexToFile(const int attr_index);
    bool SearchFromFile(const int attr_index,
                        const long lower,
                        const long upper);
    void WriteFileCallback(BPlusNode::NodePtr node, std::ofstream *file);

 private:
    std::string path_;
    int table_file_;
    std::ifstream input_index_file_;
    std::ofstream output_index_file_;
    std::shared_ptr<BPlusTree> tree_;
};
