#include <iostream>
#include <memory>
#include <string>
// #include <string.h>
// #include <BPlusNode.hpp>
#include <BPlusTree.hpp>
#include <TableManager.hpp>

int main(void) {
    std::cout << "Test" << std::endl;

    std::shared_ptr<BPlusTree> tree = std::make_shared<BPlusTree>(3);
    tree->Insert(1, 1);
    tree->Insert(2, 1);
    tree->Insert(3, 1);
    tree->Insert(4, 1);
    tree->Insert(5, 1);
    tree->Insert(6, 1);
    tree->Insert(7, 1);
    // tree->Insert(8, 1);
    // tree->Insert(9, 1);
    // tree->Insert(10, 1);
    // tree->Insert(11, 1);

    // tree->Insert(1000, 1);
    // tree->Insert(888, 1);
    // tree->Insert(9899, 1);
    // tree->Insert(7777, 1);
    // tree->Insert(666, 1);
    // tree->Insert(8484, 1);
    // tree->Insert(8848, 1);
    tree->DebugPrint();
    std::cout << std::endl;
    tree->LevelTraverse(BPlusTree::GenerateRowNumberFunc);

    // BPNodeInfo node{123, 123};
    // char *str = reinterpret_cast<char *>(&node);
    // std::cout << strlen(str) << std::endl;
    // for (int i = 0; i < strlen(str); i++) {
    //     std::cout << std::hex << (int)str[i] << std::endl;
    // }

    TableManager table("table.csv");
    table.BuildIndex(1);
    // table.CreateTable();
    table.OpenIndexFile(1);
    return 0;
}

// #include <iostream>
// #include "CLStatus.h"
// #include "CLLogger.h"

// using namespace std;

// CLStatus f()
// {
// 	return CLStatus(-1, 4);
// }

// class A
// {
// public:
// 	~A()
// 	{
// 		CLLogger::WriteLogMsg("in a", 0);
// 	}
// };

// A a;

// int main()
// {
// 	CLStatus s = f();
// 	if(!s.IsSuccess())
// 		CLLogger::WriteLogMsg("this is an error", s.m_clErrorCode);

// 	return 0;
// }