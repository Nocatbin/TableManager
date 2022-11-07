#pragma once
#include <vector>
#include <string>
#include <sstream>

static std::vector<std::string> StringSplit(const std::string& str,
                                            char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.emplace_back(item);
        }
    }
    return elems;
}
