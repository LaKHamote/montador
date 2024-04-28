#include <iostream>

#include "./functions.hpp"

string Table::get(const std::string &key){
    // If key does not exist, return ""
    if (data.find(key) == data.end()) {
        return "";
    }
    return data.at(key);
}

void Table::add(const string &key, string value, bool copy) {
    if(this->get(key) == "") {
        data[key] = value;
    }
    else{
        if (copy == true) {                                 // allow two values for dependencies if copy is true
            data[key] = data[key] + " " + value;
        } 
        else{
            throw invalid_argument(key+" was already declared");   
        }
    }
}

void Table::update(const string &key, string value) {
    if(this->get(key) != "") {
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