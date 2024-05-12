#include "main.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

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

void montador(string file_path){
    Table<string, string> symbols;
    Table<int, string> codeGenerated;
    Table<int, string> pendencies;
    Table<int, string> real;

    ifstream file(file_path); // TODO: fazer IOStream
    string line;
    int pc = 0;
    int line_counter = 0;
    if (file.is_open()) {
        // TODO:  pre-processamento
        while (getline(file, line)) {
            // std::cout << line << endl;
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
            if(macros.get(word) != nullptr) {
                if(word=="SPACE"){
                    codeGenerated.add(pc,"00");real.add(pc,ABSOLUTE);pc++;
                    if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments");
                }else if(word=="CONST"){
                    if(!(iss>>word)) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too few arguments");
                    codeGenerated.add(pc,string(2-word.length(),'0') + word);real.add(pc,ABSOLUTE);pc++;
                    if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments");
                }
            }else{
                string* opcode = mnemonics.get(word);
                if(opcode == nullptr) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Unknown mnemonic " + word);
                
                codeGenerated.add(pc,*opcode);real.add(pc,ABSOLUTE);pc++;
                if(word=="STOP"){
                    if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments");
                }
                else{
                    if(word=="COPY"){ // TODO
                        if (!(iss >> word)) throw invalid_argument("Erro na linha " + to_string(line_counter) + ": Too few arguments");
                        size_t c_index = word.find(',');
                        if (c_index == string::npos) throw invalid_argument("Erro na linha " + to_string(line_counter) + ": Missing comma in COPY instruction");
                        pendencies.add(pc, word.substr(0, c_index));real.add(pc,RELATIVE);pc++;
                        pendencies.add(pc, word.substr(c_index + 1));real.add(pc,RELATIVE);pc++;
                        if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments");
                    }else{
                        if(!(iss>>word)) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too few arguments");
                        pendencies.add(pc, word);real.add(pc,RELATIVE);pc++;
                        if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments");
                    }
                }
            }
        }
        file.close();
    for(const auto &[pc, label] : *pendencies.getData()){
        codeGenerated.add(
            pc,
            *symbols.get(label)
        );
    }
    }else {
        cerr << "Não foi possível abrir o arquivo." << endl;
    }
    std::cout<<"-----------"<<endl;
    symbols.show();
    std::cout<<"-----------"<<endl;
    pendencies.show();
    std::cout<<"-----------"<<endl;
    codeGenerated.show();
    std::cout<<"-----------"<<endl;
    real.show();
}



int main() {
    
    montador("../ex1.txt");
    montador("../ex2.txt"); 
    montador("../ex3.txt"); 
    
    
    return 0;
}