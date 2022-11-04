#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <fstream>
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
    if (index_file_.is_open()) {
        index_file_.close();
    }
    if (output_index_.is_open()) {
        output_index_.close();
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
    tree_->Print();
    return CLStatus(0, 0);
}

bool TableManager::OpenIndexFile(std::string attr) {
    std::string target_file = "attr" + attr + ".txt";
    index_file_.open(target_file, std::ios::in);
    if (!index_file_) {
        std::cout << "index file not exist! creating new index" << std::endl;
        output_index_.open(target_file, std::ios::out | std::ios::app);
        CreateIndexFile(attr);
    }
    output_index_.open(target_file, std::ios::out | std::ios::app);
    CreateIndexFile(attr);
    return true;
}

bool TableManager::CreateIndexFile(std::string attr) {
    int col_index = 0;
    // for (col_index = 0; col_index)]
    char *str;
    ssize_t size = read(table_file_, str, 8);
    std::cout << "read size : " << size << std::endl;
    return true;
}