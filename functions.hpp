#include <iostream>
#include <map>
#include <string>
#include <stdexcept>

using namespace std;

template<class keyType>
class Table {
    private:
        map<keyType, string> data;

    public:
        Table() {};
        Table(const map<keyType, string> &baseData) : data(baseData) {};

        map<keyType, string>* getData() { return &data; };

        string* get(const keyType &key) {
            auto it = data.find(key);
            if (it == data.end()) {
                return nullptr;
            }
            return &(it->second);
        }

        void add(const keyType &key, string value) {
            if (this->get(key) == nullptr) {
                data[key] = value;
            } else {
                throw invalid_argument(key + " was already declared");
            }
        }

        void update(const keyType &key, string value) {
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