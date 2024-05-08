#include <iostream>
#include <stdexcept>

#include "./functions.hpp"

string* Table::get(const std::string &key){
    auto it = data.find(key);
    if (it == data.end()) {
        return nullptr;
    }
    return &(it->second);
}

void Table::add(const string &key, string value) {
    if(this->get(key) == nullptr) {
        data[key] = value;
    }
    else{
        throw invalid_argument(key+" was already declared");
    }
}

void Table::update(const string &key, string value) {
    if(this->get(key) != nullptr) {
        data[key] = value;
    }
    else{
        throw invalid_argument(key+" was never declared");
    }
}

void Table::show(){
    for(const auto &elem : this->data){
        cout << elem.first <<" : " << elem.second<< endl;
    }
}