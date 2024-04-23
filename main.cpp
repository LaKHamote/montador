#include "main.hpp"

#include <string.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
    Table mnemonics({
        {"ADD", "01"},
        {"SUB", "02"},
        {"MUL", "03"},
        {"DIV", "04"},
        {"JMP", "05"},
        {"JMPN", "06"},
        {"JMPP", "07"},
        {"JMPZ", "08"},
        {"COPY", "09"},
        {"LOAD", "10"},
        {"STORE", "11"},
        {"INPUT", "12"},
        {"OUTPUT", "13"},
        {"STOP", "14"}
    });
    Table macros({
        {"SPACE",""},
        {"CONST",""}
    }); //...
    Table labels; // set in run time 
    Table codeGenerated;


    ifstream file("../ex1.txt"); // TODO: fazer IOStream
    string line;
    int pc = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << endl;
            istringstream iss(line);
            string word;            
            int position = 0; // only the first word can be a label
            while (iss >> word) {
                cout << word << endl;
                if(position==0 && word[word.length() -1] == ':') { // LABEL FOUND AS THE FIRST WORD
                    labels.add(word.substr(0,word.length()-1),"current_line"); // store the label with the current line -> TODO
                }else{
                    codeGenerated.add(to_string(pc),mnemonics.get(word));
                    pc++;
                }
                position++;

            }
            cout << "----------" << endl;
            pc++; //TODO depende de qual instrução foi lida
        }
        file.close();
    } else {
        cerr << "Não foi possível abrir o arquivo." << endl;
    }


    labels.show();
    cout<<"-----------"<<endl;
    codeGenerated.show();
    
    
    return 0;
}