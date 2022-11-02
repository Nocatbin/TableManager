#include <iostream>
#include <memory>
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
    // if (newentry != nullptr) {
    //     tree = tree->UpdateRoot(newentry);
    // }
    // tree->Insert(4, 1, newentry);
    // if (newentry != nullptr) {
    //     tree = tree->UpdateRoot(newentry);
    // }
    tree->Print();
    // TableManager table("table.csv");
    // table.CreateTable();
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