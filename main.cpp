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
        {"SPACE","xx"},
        {"CONST","xx"}
    }); //...
    Table symbols; // set in run time 
    Table codeGenerated;
    Table dependencies;


    ifstream file("../ex1.txt"); // TODO: fazer IOStream
    string line;
    int pc = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << endl;
            istringstream iss(line); // iss>>word gets the next word
            string word;
            if(!(iss>>word)) continue; // no words in the current line
            if(word[word.length()-1] == ':') { // LABEL FOUND AS THE FIRST WORD
                symbols.add(word.substr(0,word.length()-1),to_string(pc)); // store the label with the current pc
                if(!(iss>>word)) continue; // label in empty line
            }
            if(macros.get(word) != "") {
                if(word=="SPACE"){
                    codeGenerated.add(to_string(pc),"xx"); pc++; // macro takes 1 memory space TODO: resolve CONST
                    if(iss>>word) throw invalid_argument("Too many arguments");
                }else if(word=="CONST"){
                    if(!(iss>>word)) throw invalid_argument("Too few arguments");
                    codeGenerated.add(to_string(pc),word); pc++; // macro takes 1 memory space TODO: resolve CONST
                    if(iss>>word) throw invalid_argument("Too many arguments");
                }
            }else{
                string opcode = mnemonics.get(word);
                if(opcode == "") throw invalid_argument("Unknown mnemonic");
                codeGenerated.add(to_string(pc),opcode);
                if(word=="STOP"){
                    if(iss>>word) throw invalid_argument("Too many arguments");
                    pc+=1;
                }
                else{
                    if(!(iss>>word)) throw invalid_argument("Too few arguments");
                    if(word=="COPY"){ // ????????????
                        dependencies.add(to_string(pc), word);
                        if(!(iss>>word)) throw invalid_argument("Too few arguments");
                        dependencies.add(to_string(pc), word);
                        pc+=3;

                    }else{
                        dependencies.add(to_string(pc), word);
                        if(iss>>word) throw invalid_argument("Too many arguments");
                        pc+=2;
                    }
                }
            }
            cout<<"-----------"<<endl;
        }
        file.close();
    for(const auto &[pc, label] : dependencies.getData()){
        codeGenerated.update(
            pc,
            codeGenerated.get(pc) + symbols.get(label));
    }
    }else {
        cerr << "Não foi possível abrir o arquivo." << endl;
    }
    symbols.show();
    cout<<"-----------"<<endl;
    codeGenerated.show();
    cout<<"-----------"<<endl;
    dependencies.show();
    
    
    return 0;
}


// while (iss >> word) {
            //     cout << word << endl;
            //     if(position==0 && word[word.length() -1] == ':') { // LABEL FOUND AS THE FIRST WORD
            //         symbols.add(word.substr(0,word.length()-1),"current_line"); // store the label with the current line -> TODO
            //     }else{
            //         codeGenerated.add(to_string(pc),mnemonics.get(word));
            //         pc++;
            //     }
            //     position++;

            // }
            // cout << "----------" << endl;