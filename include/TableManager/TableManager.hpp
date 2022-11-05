#pragma once

#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <CLStatus.h>
#include <BPlusTree.hpp>

// #pragma pack(push, 1)
// struct BPTreeLine {
//     bool is_leaf;
//     uint32_t depth;
//     uint16_t elevation;
//     uint16_t azimuth;
//     uint8_t intensity;
//     uint8_t tag;
// };

// struct BPNodeInfo {
//     long key;
//     int row_index;  // value
// };
// #pragma pack(pop)

class TableManager {
 public:
    // TableManager() = delete;
    TableManager(std::string path);
    ~TableManager();

    static constexpr int ATTRIBUTE_NUMBER = 10;
    static constexpr int INIT_ROW_NUMBER = 10;
    static constexpr int TREE_DEGREE = 3;

    CLStatus CreateTable();
    CLStatus AppendEntry(std::string entry);
    CLStatus BuildIndex(int attr_index);
    bool OpenIndexFile(int attr_index);
    bool CreateIndexFile(int attr_index);
    void WriteFileCallback(BPlusNode::NodePtr node, std::ofstream *file);

 private:
    std::string path_;
    int table_file_;
    std::ifstream input_index_file_;
    std::ofstream output_index_file_;
    std::shared_ptr<BPlusTree> tree_;
};
