#include <map>

using namespace std;

class Table{
    private:
        map<string, string> data;
    public:
        Table() {};
        Table(const map<string, string> &baseData) : data(baseData) {};
        string get(const string &key);
        void add(const string &key, string value);
        void update(const string &key, string value);
        void show();
};