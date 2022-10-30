#include <iostream>
#include <BPlusNode.hpp>

int main(void) {
    std::cout << "Test" << std::endl;
    BPlusNode::NodePtr tree(new BPlusNode(3, true));
    BPlusNode::NodePtr newentry;
    tree->Insert(1, 1, newentry);
    tree->Insert(2, 1, newentry);
    tree->Insert(3, 1, newentry);
    // tree->Insert(4, 1, newentry);
    // if (newentry != nullptr) {
    //     root = root->UpdateRoot(2, newentry);
    // }
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