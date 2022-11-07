#include <iostream>
#include <memory>

#include <CLString.hpp>
#include <BPlusTree.hpp>
#include <TableManager.hpp>

// unit test for create table file randomly
void CreateTableTest() { TableManager table("test.csv"); }

// unit test for search entry from file
void SearchFromFileTest() {
    TableManager table("test.csv");
    table.SearchFromFile(1, 650390930, 1435302762);
    table.SearchFromFile(1, 650390930, 1435302760);
    table.SearchFromFile(1, 650390932, 1435302760);
    table.SearchFromFile(2, 2000000000, 3000000000);
}

void BPTreeInsertTest() {
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
}

void PrintMenu() {
    std::cout << "-------------------TableManager-------------------"
              << std::endl;
    std::cout << "1. Open existing table or create a new one" << std::endl;
    std::cout << "2. Multi thread append entry test on/off" << std::endl;
    std::cout << "3. Search entrys between lower and higher" << std::endl;
    std::cout << "--------------------------------------------------"
              << std::endl;
    std::cout << "Enter your choice: " << std::endl;
}

int main(void) {
    std::shared_ptr<TableManager> table;
    PrintMenu();
    int choice;
    std::cin >> choice;
    while (choice != -1) {
        if (choice == 1) {
            std::cout << "Enter table file name:" << std::endl;
            std::string path;
            std::cin >> path;
            if (table != nullptr) {
                std::cout << "WARN: table already opened, opening new table!"
                          << std::endl;
            }
            table.reset(new TableManager(path));
            std::cout << "Table opened or created!" << std::endl;
        } else if (choice == 2) {
            static bool thread1_alive = false;
            static bool thread2_alive = false;
            int x;
            std::cin >> x;
            // tree->insert(x);
        } else if (choice == 3) {
            std::cout << "Enter attribute index & lower & upper :" << std::endl;
            int index = -1;
            long lower, upper;
            std::cin >> index >> lower >> upper;
            if (table == nullptr) {
                std::cout << "FATAL: table not opened, exiting!" << std::endl;
                std::cout << "Enter your choice: " << std::endl;
                std::cin >> choice;
                continue;
            }
            table->SearchFromFile(index, lower, upper);
        } else {
            std::cout << "ERROR: Invalid choice, please enter again!"
                      << std::endl;
        }
        std::cout << "Enter your choice: " << std::endl;
        std::cin >> choice;
    }

    return 0;
}

// int main()
// {
// 	CLStatus s = f();
// 	if(!s.IsSuccess())
// 		CLLogger::WriteLogMsg("this is an error", s.m_clErrorCode);
// 	return 0;
// }