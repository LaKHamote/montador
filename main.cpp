#include "main.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <vector>

using namespace std;

#define ABSOLUTE "0" 
#define RELATIVE "1" 

Table<string, string> mnemonics({
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
Table<string, string> macros({
    {"SPACE","00"},
    {"CONST","00"}
});

unordered_map<string, string> equTable; // mapa para armazenar valores de EQU


int main() {

    // TODO:  pre-processamento
    //preprocessa("../exemplos/ex4.txt", "../myfile.pre");
    //montador("../exemplos/ex5.txt", "../prog1.obj");
    ligador("../prog1.obj", "../prog2.obj", "../prog1.e");
    // montador("../exemplos/ex2.txt");
    // montador("../ex3.txt"); 
    
    
    return 0;
}