#include <string>
#include <iostream>
#include <random>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <TableManager.hpp>

TableManager::TableManager(std::string path) : path_(path) {
    // init
    table_file_ = open(path_.c_str(), O_RDWR | O_APPEND);
    if (-1 == table_file_) {
        std::cout << "File not Found, Creating New Table" << std::endl;
        table_file_ =
            open(path_.c_str(), O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        // createTable();
        // return CLStatus(-1, -1);
    }
}

TableManager::~TableManager() {
    if (table_file_ != 0) {
        close(table_file_);
    }
}

CLStatus TableManager::CreateTable() {
    std::mt19937 rand_generator{std::random_device{}()};  // ret long
    for (int index = 0; index < 1; index++) {
        std::cout << rand_generator() << std::endl;
    }
    std::string head = "ID";
    // std::string head;
    for (int index = 0; index < 100; index++) {
        head += ",attr" + std::to_string(index + 1);
    }
    head += "\n";
    // std::cout << head << std::endl;
    CLStatus ret = AppendEntry(head);
    if (!ret.IsSuccess()) {
        return ret;
    }

    for (int row = 0; row < 10000; row++) {
        std::string entry = std::to_string(row + 1);
        for (int col = 0; col < 100; col++) {
            entry += "," + std::to_string(rand_generator());
        }
        entry += "\n";
        // std::cout << entry << std::endl;
        CLStatus ret = AppendEntry(entry);
        if (!ret.IsSuccess()) {
            return ret;
        }
    }
    return CLStatus(0, 0);
}

CLStatus TableManager::AppendEntry(std::string entry) {
    ssize_t ret = write(table_file_, entry.c_str(), strlen(entry.c_str()));
    if (ret == -1) {
        close(table_file_);
        return CLStatus(-1, errno);
    }
    return CLStatus(0, 0);
}