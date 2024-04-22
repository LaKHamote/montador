#include <map>

using namespace std;

class Table{
    private:
        map<string, string> data;
    public:
        Table() {};
        Table(const map<std::string, std::string> &baseData) : data(baseData) {};
        string get(const std::string &key);
        void add(const string &key, string value);
        void update(const string &key, string value);
};