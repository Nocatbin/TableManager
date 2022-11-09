#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <functional>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <CLStatus.h>
#include <CLString.hpp>
#include <CLCriticalSection.h>
#include <TableManager.hpp>

TableManager::TableManager(std::string path) : path_(path) {
    // try opening table_file_
    table_file_ = open(path_.c_str(), O_RDWR);
    if (-1 == table_file_) {
        // open err
        std::cout << "File not Found, Creating New Table" << std::endl;
        // create new file
        table_file_ = open(path_.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        // random create table file
        createTable();
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

// randomly create table file
CLStatus TableManager::createTable() {
    for (int row = 0; row < INIT_ROW_NUMBER; row++) {
        std::string entry = getRandomEntry();
        // dump the entry to file
        CLStatus ret = AppendEntry(entry);
        if (!ret.IsSuccess()) {
            return ret;
        }
    }
    return CLStatus(0, 0);
}

std::string TableManager::getRandomEntry() {
    CLCriticalSection cs(&rand_entry_mutex);
    std::stringstream entry;
    static int row = 0;
    entry << std::right << std::setfill('0') << std::setw(10) << row++;
    for (int col = 0; col < 100; col++) {
        entry << "," << std::right << std::setfill('0') << std::setw(10)
              << rand_generator();
    }
    entry << std::endl;
    return entry.str();
}

// API function for appending entry to table file end
CLStatus TableManager::AppendEntry(std::string entry) {
    CLCriticalSection cs(&table_file_mutex);
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

// API function for appending a random entry to table file end
CLStatus TableManager::AppendEntry() {
    std::string rand_entry = getRandomEntry();
    CLStatus ret = AppendEntry(rand_entry);
    if (!ret.IsSuccess()) {
        std::cout << "FATAL: Append Error!" << std::endl;
    }
    return ret;
}

CLStatus TableManager::buildIndex(const int attr_index) {
    char num_ptr[ATTRIBUTE_NUMBER];
    tree_.reset(new BPlusTree(TREE_DEGREE));
    // set file cursor to the attribute of the first row
    lseek(table_file_, 11 * attr_index, SEEK_SET);
    for (int i = 0; i < INIT_ROW_NUMBER; i++) {
        size_t t = read(table_file_, num_ptr, 10);
        if (t == -1) {
            std::cout << "read err, out of range" << std::endl;
            return CLStatus(-1, -1);
        }
        lseek(table_file_, 11 * ATTRIBUTE_NUMBER + 1, SEEK_CUR);  // +1 for /n
        long key = atol(num_ptr);
        tree_->Insert(key, i);
    }
    // tree_->DebugPrint();
    return CLStatus(0, 0);
}

bool TableManager::OpenIndexFile(const int attr_index) {
    if (attr_index > INIT_ROW_NUMBER) {
        std::cout << "attribute index out of range!" << std::endl;
        return false;
    }
    std::string target_file = "attr" + std::to_string(attr_index) + ".txt";
    if (input_index_file_.is_open()) {
        input_index_file_.close();
    }
    if (output_index_file_.is_open()) {
        output_index_file_.close();
    }
    input_index_file_.open(target_file, std::ios::in);
    if (!input_index_file_) {
        std::cout << "Index File not Exist! Building New Index" << std::endl;
        CLStatus ret = buildIndex(attr_index);
        if (!ret.IsSuccess()) {
            std::cout << "buildIndex Fail! Exiting" << std::endl;
            return false;
        }
        tree_->LevelTraverse(BPlusTree::GenerateRowNumberFunc);
        std::cout << "Dumping Index to File" << std::endl;
        output_index_file_.open(target_file, std::ios::out | std::ios::app);
        if (!output_index_file_.is_open()) {
            std::cout << "Output File Open Err!" << std::endl;
            return false;
        }
        dumpIndexToFile(attr_index);
        input_index_file_.open(target_file, std::ios::in);
        if (!output_index_file_.is_open()) {
            std::cout << "Input Index File Open Err!" << std::endl;
            return false;
        }
    }
    return true;
}

bool TableManager::dumpIndexToFile(const int attr_index) {
    // dump file head
    output_index_file_ << "CYT_BPTREE," << std::setfill('0') << std::setw(2)
                       << tree_->Degree() << "," << std::setw(3) << attr_index
                       << std::endl;
    // bind ofstream object to input param of the callback function
    tree_->LevelTraverse(std::bind(&TableManager::WriteFileCallback, this,
                                   std::placeholders::_1, &output_index_file_));
    return true;
}

// callback function for dumping index to file during level traverse
void TableManager::WriteFileCallback(BPlusNode::NodePtr node,
                                     std::ofstream* file) {
    // index file line format
    // is_leaf,key_size,keys,...,child_ptr_row,...
    int key_size = node->keys_.size();
    int child_size = node->child_ptrs_.size();
    int value_size = node->values_.size();

    // 1 char flag for leaf node
    *file << node->is_leaf_ ? 1 : 0;
    // 2 char key size
    *file << "," << std::setfill('0') << std::setw(2) << key_size;
    // 10 char keys, empty keys are set to -1, max key_size = degree -1
    for (int idx = 0; idx < node->degree_ - 1; idx++) {
        if (idx < key_size) {
            *file << "," << std::setfill('0') << std::setw(10)
                  << node->keys_[idx];
        } else {
            *file << "," << std::setfill('0') << std::setw(10) << -1;
        }
    }
    // 10 char child row, empty childs are set to -1, max child_size = degree
    for (int idx = 0; idx < node->degree_; idx++) {
        if (idx < value_size && node->is_leaf_) {
            *file << "," << std::setfill('0') << std::setw(10)
                  << node->values_[idx];
        } else if (idx < child_size && (!node->is_leaf_)) {
            *file << "," << std::setfill('0') << std::setw(10)
                  << node->child_ptrs_[idx]->row_number_in_file_;
        } else {
            *file << "," << std::setfill('0') << std::setw(10) << -1;
        }
        // if (idx >= child_size && (!node->is_leaf_))
    }
    *file << std::endl;
}

bool TableManager::SearchFromFile(const int attr_index,
                                  const long lower,
                                  const long upper) {
    std::cout << "Search attribute " << attr_index << " " << lower << " - "
              << upper << std::endl;
    if (!OpenIndexFile(attr_index)) {
        return false;
    }

    // parse file head
    std::string file_head;
    input_index_file_ >> file_head;
    auto file_head_split = StringSplit(file_head, ',');
    if (file_head_split[0] != "CYT_BPTREE") {
        std::cout << "Fatal: Index file corrupted!" << std::endl;
        return false;
    }
    if (std::stoi(file_head_split[2]) != attr_index) {
        std::cout << "Fatal: Index attribute not match!" << std::endl;
        return false;
    }
    int degree = std::stoi(file_head_split[1]);
    // 5 = (is_leaf flag, key_size,)
    // keys and child ptr number = (2 * degree - 1)
    // (ATTRIBUTE_LENGTH + 1) = 10 bytes key + , or /n
    int line_length = 5 + (2 * degree - 1) * (ATTRIBUTE_LENGTH + 1);
    // parse index file
    int count = 0;
    std::cout << "found entry key : value" << std::endl;
    while (!input_index_file_.eof()) {
        std::string line;
        input_index_file_ >> line;
        if (input_index_file_.eof()) {
            break;
        }
        auto line_split = StringSplit(line, ',');

        std::vector<long> keys;
        std::vector<long> values;
        // next index row to parse & search
        static int next_index_row = -1;
        // parse single line
        int key_size = std::stoi(line_split[1]);
        bool is_leaf = line_split[0] == "1" ? true : false;
        // deserialize keys
        for (int idx = 0; idx < key_size; idx++) {
            keys.emplace_back(std::stol(line_split[idx + 2]));
        }

        int key_index = 0;
        for (key_index = 0; key_index < key_size; key_index++) {
            if (lower <= keys[key_index]) {
                break;
            }
        }
        if (is_leaf) {
            // deserialize values
            for (int idx = 0; idx < key_size; idx++) {
                values.emplace_back(
                    std::stol(line_split[idx + 2 + degree - 1]));
            }
            for (int idx = key_index; idx < key_size; idx++) {
                if (keys[idx] >= lower && keys[idx] <= upper) {
                    static bool print = true;
                    if (print) {
                        std::cout << keys[idx] << ":" << values[idx]
                                  << std::endl;
                    }

                    if (count == 9) {
                        std::cout << "... ..." << std::endl;
                        print = false;
                    } else if (count >= 9) {
                        print = false;
                    }
                    count++;
                } else {
                    std::cout << "Search done! Count: " << count << std::endl;
                    return true;
                }
            }
            next_index_row += 1;
        } else {
            // nonleaf node, (key_index + 2 + degree - 1)
            // indicates next row to search
            next_index_row = std::stoi(line_split[key_index + 2 + degree - 1]);
        }
        // 17 Bytes file head
        input_index_file_.seekg(17 + line_length * next_index_row,
                                std::ios::beg);
    }
    return true;
}
