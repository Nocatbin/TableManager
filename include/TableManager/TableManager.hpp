#pragma once

#include <vector>
#include <string>
#include <memory>
#include <CLStatus.h>

class TableManager {
 public:
    // TableManager() = delete;
    TableManager(std::string path);
    ~TableManager();

    CLStatus CreateTable();
    CLStatus AppendEntry(std::string entry);
    // CLStatus Insert(int key, int val, NodePtr &newentry);
    // NodePtr UpdateRoot(NodePtr &newentry);
    // void Traverse();
    // CLStatus Search();
    // CLStatus Release();

 private:
    std::string path_;
    int table_file_;
};
