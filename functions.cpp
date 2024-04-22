#include "./functions.hpp"


string Table::get(const std::string &key){
    // If key does not exist, return ""
    if (data.find(key) == data.end()) {
        return "";
    }
    return data.at(key);
}