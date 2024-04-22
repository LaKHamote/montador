#include <map>

using namespace std;

class Table{
    private:
        map<string, string> data;
    public:
        void add(const string &key, string value) {data[key] = value;}
        string get(const std::string &key);
};