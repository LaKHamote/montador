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
    });
    Table symbols;
    Table codeGenerated;
    Table dependencies;


    ifstream file("../ex1.txt"); // TODO: IOStream
    string line;
    int pc = 0;
    int line_counter = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            // cout << line << endl;
            line_counter++;
            istringstream iss(line);                                                 // iss>>word gets the next word
            string word;
            if(!(iss>>word)) continue;                                               // no words in the current line
            if(word[word.length()-1] == ':') {                                       // LABEL FOUND AS THE FIRST WORD
                string string_pc = string(pc<10,'0') + to_string(pc); 
                symbols.add(                                                         // store the label with the current pc
                    word.substr(0,word.length()-1),
                    string(pc<10,'0') + to_string(pc)); 
                if(!(iss>>word)) continue;                                           // label in empty line
            }
            if(macros.get(word) != "") {
                if(word=="SPACE"){
                    codeGenerated.add(to_string(pc),"xx");
                    if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments");
                }else if(word=="CONST"){
                    if(!(iss>>word)) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too few arguments");
                    codeGenerated.add(to_string(pc),string(2-word.length(),'0') + word);
                    if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments");
                }
                pc++; // macros take 1 memory space
            }else{
                string opcode = mnemonics.get(word);
                if(opcode == "") throw invalid_argument("Erro na linha "+to_string(line_counter)+": Unknown mnemonic ->" + word);
                codeGenerated.add(to_string(pc),opcode);
                if(word=="STOP"){
                    if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments");
                    pc+=1;
                }
                else{
                    string command = word;
                    if(!(iss>>word)) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too few arguments");

                    if(command=="COPY"){
                        dependencies.add(to_string(pc), word.substr(0, word.length()-1));           // ommit the comma at the end of the word
                        if(!(iss>>word)) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too few arguments");
                        dependencies.add(to_string(pc), word, true);                                // allow two values at the line where the "copy" command is
                        
                        
                        pc+=3;

                    }else{
                        dependencies.add(to_string(pc), word);
                        if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments");
                        pc+=2;
                    }
                }
            }
            cout<<"-----------"<<endl;
        }
        file.close();
    for(const auto &[pc, label] : dependencies.getData()){
        istringstream isss(label);
        string label1;
        string label2; 
        isss>>label1;

        if (isss>>label2) {                                                     // if more than one label in labels, iterate through each one
            codeGenerated.update(
            pc,
            codeGenerated.get(pc) + symbols.get(label1) + symbols.get(label2)); 

        }else{                                                                  // else, proceed with 1 label       
            codeGenerated.update(
                pc,
                codeGenerated.get(pc) + symbols.get(label1));
        }  
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