#include "main.hpp"

#include <string.h>
#include <stdexcept>
#include <iostream>

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
    mnemonics.update("AD","2");
    
    cout << mnemonics.get("ADD") << endl;
    
    
    
    
    return 0;
}