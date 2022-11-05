#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <fstream>
#include <functional>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <CLStatus.h>
#include <TableManager.hpp>

TableManager::TableManager(std::string path) : path_(path) {
    // init
    table_file_ = open(path_.c_str(), O_RDWR);
    if (-1 == table_file_) {
        std::cout << "File not Found, Creating New Table" << std::endl;
        table_file_ = open(path_.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        CreateTable();
    }
}

TableManager::~TableManager() {
    if (input_index_file_.is_open()) {
        input_index_file_.close();
    }
    if (output_index_file_.is_open()) {
        output_index_file_.close();
    }
}

CLStatus TableManager::CreateTable() {
    std::mt19937 rand_generator{std::random_device{}()};  // ret long
    for (int row = 0; row < INIT_ROW_NUMBER; row++) {
        std::stringstream entry;
        entry << std::right << std::setfill('0') << std::setw(10) << row;
        for (int col = 0; col < 10; col++) {
            entry << "," << std::right << std::setfill('0') << std::setw(10)
                  << rand_generator();
        }
        entry << std::endl;
        // std::cout << entry.str() << std::endl;
        CLStatus ret = AppendEntry(entry.str());
        if (!ret.IsSuccess()) {
            return ret;
        }
    }
    return CLStatus(0, 0);
}

CLStatus TableManager::AppendEntry(std::string entry) {
    // set cursor to file end
    lseek(table_file_, 0, SEEK_END);
    // write line
    ssize_t ret = write(table_file_, entry.c_str(), strlen(entry.c_str()));
    if (ret == -1) {
        close(table_file_);
        return CLStatus(-1, errno);
    }
    return CLStatus(0, 0);
}

CLStatus TableManager::BuildIndex(int attr_index) {
    char num_ptr[ATTRIBUTE_NUMBER];
    tree_.reset(new BPlusTree(TREE_DEGREE));
    lseek(table_file_, 11 * attr_index, SEEK_SET);
    for (int i = 0; i < INIT_ROW_NUMBER; i++) {
        size_t t = read(table_file_, num_ptr, 10);
        if (t == -1) {
            std::cout << "read err" << std::endl;
        }
        lseek(table_file_, 11 * ATTRIBUTE_NUMBER + 1, SEEK_CUR);  // +1 for /n
        long num = atol(num_ptr);
        std::cout << num << std::endl;
        tree_->Insert(num, i);
    }
    // tree_->DebugPrint();
    return CLStatus(0, 0);
}

bool TableManager::OpenIndexFile(int attr_index) {
    std::string target_file = "attr" + std::to_string(attr_index) + ".txt";
    input_index_file_.open(target_file, std::ios::in);
    if (!input_index_file_) {
        std::cout << "index file not exist! creating new index" << std::endl;
        output_index_file_.open(target_file, std::ios::out | std::ios::app);
        CreateIndexFile(attr_index);
    }
    // output_index_file_.open(target_file, std::ios::out | std::ios::app);
    // CreateIndexFile(attr_index);
    return true;
}

bool TableManager::CreateIndexFile(int attr_index) {
    int col_number = -1;
    // bind ofstream object to input param of the callback function
    tree_->LevelTraverse(std::bind(&TableManager::WriteFileCallback, this,
                                   std::placeholders::_1, &output_index_file_));
    return true;
}

void TableManager::WriteFileCallback(BPlusNode::NodePtr node,
                                     std::ofstream* file) {
    // index file line format
    // is_leaf,key_size,keys,...,child_ptr_row,...
    int key_size = node->keys_.size();
    int child_size = node->child_ptrs_.size();

    // 1 char flag for leaf node
    *file << node->is_leaf_ ? 1 : 0;
    // 2 char key size
    *file << "," << std::setfill('0') << std::setw(2) << key_size;
    // 10 char keys, empty keys are set to -1
    for (int idx = 0; idx <= node->degree_ - 1; idx++) {
        if (idx < key_size) {
            *file << "," << std::setfill('0') << std::setw(10)
                  << node->keys_[idx];
        } else {
            *file << "," << std::setfill('0') << std::setw(10) << -1;
        }
    }
    // 10 char child row, empty childs are set to -1
    for (int idx = 0; idx <= node->degree_ - 1; idx++) {
        if (idx >= child_size || node->is_leaf_) {
            *file << "," << std::setfill('0') << std::setw(10) << -1;
        } else {
            *file << "," << std::setfill('0') << std::setw(10)
                  << node->child_ptrs_[idx]->row_number_in_file_;
        }
    }
    *file << std::endl;
}

std::vector<std::string> stringSplit(const std::string& str, char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}