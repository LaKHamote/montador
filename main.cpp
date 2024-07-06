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

unordered_map<string, string> equTable; // Tabela para armazenar valores de EQU

// Converter strings 
string paraMinuscula(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}


// Remove espaços desnecessários
string removeEspaco(const string& line) {
    string result;
    bool inSpace = false;
    bool copyFound = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];

        if (isspace(c)) {
            if (!inSpace) {
                // Adiciona um espaço apenas se não estiver dentro de espaços consecutivos e se a linha nao comecar com espacos
                if (i != 0) {
                    result += ' ';
                }
                inSpace = true;
            }
        } else {
            inSpace = false;
            result += c;

            // Verifica se encontrou a palavra-chave "COPY"
            if (i + 4 <= line.length() && line.substr(i, 4) == "copy") {
                copyFound = true;
            }

            // Remove espaço após a vírgula se estiver dentro de uma instrução "COPY X, X"
            if (copyFound && c == ',' && i + 1 < line.length() && line[i + 1] == ' ') {
                ++i; // Avança para pular o espaço
                inSpace = true;
            }
        }
    }

    return result;
}

string encontraDiretiva(const vector<string>& v, const string& directive) {
    auto pos = find(v.begin(), v.end(), directive);

    if (pos != v.end()) {
        int index = distance(v.begin(), pos);

        if (index >= 1 && index + 1 < v.size() && directive == "equ") {
            equTable[v[index - 1]] = v[index + 1];
            return "";
        } else {
            // Processa const
            stringstream result;
            for (const auto& word : v) {
                // Converte palavra hexadecimal para decimal, se aplicável
                if (word.substr(0, 2) == "0x" || word.substr(0, 3) == "-0x") {
                    try {
                        int hexValue = stoi(word, nullptr, 16);
                        result << hexValue << " ";
                    } catch (const std::invalid_argument& e) {
                        cerr << "Erro ao converter hexadecimal para decimal: " << e.what() << endl;
                    }
                } else {
                    result << word << " ";
                }
            }
            return result.str();
        }

        

    }
    return "nao se aplica";
}


void preprocessa(string inputFilePath, string outputFilePath) {
    ifstream inputFile(inputFilePath);
    ofstream outputFile(outputFilePath);

    if (!inputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada" << endl;
        return;
    }

    if (!outputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de saída" << endl;
        return;
    }

    string line;

    int previous_label=0;
    string previous_line = "";

    // Processar o restante do arquivo
    while (getline(inputFile, line)) {
        line = paraMinuscula(line);
        line = removeEspaco(line);

        if (previous_label == 1) {
            line.insert(0, previous_line);
            line.insert(previous_line.length(), " ");

            previous_label = 0;
        }

        // Caso de rotulos
        if (line[line.length() - 1] == ':') {
            previous_label=1;
            previous_line=line;
            line = "";
        }

        int start, end;
        start = end = 0;
        vector<string> v;

        char dl = ' ';

        while ((start = line.find_first_not_of(dl, end)) != string::npos) {

            end = line.find(dl, start);
  
            v.push_back(line.substr(start, end - start));
        }

        // popula o map com equ, para depois
        if (encontraDiretiva(v, "equ") != "nao se aplica") {
            line="";
        }
        // troca os valores das constantes de hexa para decimal
        if (encontraDiretiva(v, "const") != "nao se aplica") {
            line = encontraDiretiva(v, "const");
        }

        // Remover comentários
        size_t commentPos = line.find(';');
        if (commentPos != string::npos) {
            line = line.substr(0, commentPos);
        }

        // Processar diretivas IF
        if (line.find("if") != string::npos) {
            
            istringstream iss(line);
            string command, condition;
            iss >> command >> condition;
            condition += ':';
            if (equTable.find(condition) == equTable.end() || equTable[condition] == "0") {
                getline(inputFile, line);  // Ignorar a próxima linha se a condição for falsa
                continue;
            }
            line = "";
        }

        // Formatar linha final
        line = removeEspaco(line);

        if (line != "") {
            outputFile << line << endl;
        }
    }

    inputFile.close();
    outputFile.close();
}

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
    std::cout<<"-----Symbols------"<<endl;
    symbols.show();
    std::cout<<"-----Pendencies------"<<endl;
    pendencies.show();
    std::cout<<"-----Code------"<<endl;
    codeGenerated.show();
    std::cout<<"------Real-----"<<endl;
    real.show();
}



int main() {

    // TODO:  pre-processamento
    preprocessa("../exemplos/ex4.txt", "../myfile.pre");
    //montador("../exemplos/ex1.txt");
    // montador("../exemplos/ex2.txt");
    // montador("../ex3.txt"); 
    
    
    return 0;
}