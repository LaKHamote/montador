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
                cout << ("Erro semântico: Rotulo redefinido => "+key)<<endl;
            }
        }

        void update(const keyType &key, valueType value) {
            if (this->get(key) != nullptr) {
                data[key] = value;
            } else {
                cout << (key + " was never declared\n");
            }
        }
        // se quiser imprimir no terminal, só nao colocar o output=seuOutputFile
        void show(bool index=true, ostream& output = cout) {
            for (const auto &elem : this->data) {
                if (index) output  << elem.first << " " << elem.second << endl;
                else {
                    output  << elem.second << " ";
                }
            }
            if (!index) output  << endl;
        }
};

#endif 