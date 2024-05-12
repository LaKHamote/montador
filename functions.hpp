#ifndef TABLE_HPP
#define TABLE_HPP

#include <iostream>
#include <map>
#include <string>
#include <stdexcept>

using namespace std;

template<class keyType, class valueType>
class Table {
    private:
        map<keyType, valueType> data;

    public:
        Table() {};
        Table(const map<keyType, valueType> &baseData) : data(baseData) {};

        map<keyType, valueType>* getData() { return &data; };

        valueType* get(const keyType &key) {
            auto it = this->data.find(key);
            if (it == this->data.end()) {
                return nullptr;
            }
            return &(it->second);
        }

        void add(const keyType &key, valueType value) {
            if (this->get(key) == nullptr) {
                data[key] = value;
            } else {
                throw invalid_argument(key + " was already declared");
            }
        }

        void update(const keyType &key, valueType value) {
            if (this->get(key) != nullptr) {
                data[key] = value;
            } else {
                throw invalid_argument(key + " was never declared");
            }
        }

        void show() {
            for (const auto &elem : this->data) {
                cout << elem.first << " : " << elem.second << endl;
            }
        }
};

#endif 