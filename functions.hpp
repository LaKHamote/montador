#include <map>
#include <string>

using namespace std;

class Table{
    private:
        map<string, string> data;
    public:
        Table() {};
        Table(const map<string, string> &baseData) : data(baseData) {};
        string get(const string &key);
        void add(const string &key, string value, bool copy=false);
        void update(const string &key, string value);
        void show();
        map<string, string> getData() { return data; };
};