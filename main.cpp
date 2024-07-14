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

// pre-processamento : Converter strings 
string paraMinuscula(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}


// pre-processamento : Remove espaços desnecessários
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
// pre-processamento : lida com os casos de numeros em hexa/comando equ
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

void montador(string file_path, string outputFilePath){
    Table<string, string> symbols;
    Table<int, string> codeGenerated;
    Table<int, string> pendencies;
    Table<int, string> real;
    Table<string, string> definitions;
    Table<string,int> used;

    ifstream file(file_path); // TODO: fazer IOStream
    ofstream outputFile(outputFilePath);

        if (!outputFile.is_open()) {
        cerr << "Erro ao abrir o arquivo de saída" << endl;
        return;
    }

    string line;
    int pc = 0;
    int line_counter = 0;
    int begin_flag = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            // std::cout << "linha " << line << endl;
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
            // if begin_ == true
            if(macros.get(word) != nullptr) {
                if(word=="SPACE"){
                    codeGenerated.add(pc,"00");real.add(pc,ABSOLUTE);pc++;
                    if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments 1");
                }else if(word=="CONST"){
                    if(!(iss>>word)) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too few arguments");
                    codeGenerated.add(pc,string(2-word.length(),'0') + word);real.add(pc,ABSOLUTE);pc++;
                    if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments 2");
                }
            }else{
                string* opcode = mnemonics.get(word);
                // caso PUBLIC, EXTERN, BEGIN, END
                if(opcode == nullptr) {
                    if (paraMinuscula(word) == "begin" || paraMinuscula(word) == "extern" || paraMinuscula(word) == "public") {
                        begin_flag=1;
                        continue;
                    }else if (paraMinuscula(word) == "end") {
                        break; 
                    }         
                    else {
                        throw invalid_argument("Erro na linha "+to_string(line_counter)+": Unknown mnemonic " + word);
                    }
                } // begin_=1
                
                codeGenerated.add(pc,*opcode);real.add(pc,ABSOLUTE);pc++;
                if(word=="STOP"){
                    
                    if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments 3");
                }
                else{
                    if(word=="COPY"){ 
                        if (!(iss >> word)) throw invalid_argument("Erro na linha " + to_string(line_counter) + ": Too few arguments");
                        size_t c_index = word.find(',');
                        if (c_index == string::npos) throw invalid_argument("Erro na linha " + to_string(line_counter) + ": Missing comma in COPY instruction");
                        pendencies.add(pc, word.substr(0, c_index));real.add(pc,RELATIVE);pc++;
                        pendencies.add(pc, word.substr(c_index + 1));real.add(pc,RELATIVE);pc++;
                        if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments 4");
                    }else{
                        if(!(iss>>word)) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too few arguments");
                        pendencies.add(pc, word);real.add(pc,RELATIVE);pc++;
                        
                        if(iss>>word) throw invalid_argument("Erro na linha "+to_string(line_counter)+": Too many arguments 5");
                    }
                }
            }
        }
        file.close();
    for(const auto &[pc, label] : *pendencies.getData()){
        // cout << pc << ":" << label << endl;
        codeGenerated.add(
            pc,
            *symbols.get(label)
        );
    }
    }else {
        cerr << "Não foi possível abrir o arquivo." << endl;
    }

    if (begin_flag) {
        for(const auto &[label_symbol, adress_symbol] : *symbols.getData()){
            // unica falha disso pode ser em casos de codigo com space como unica linha de codigo, tipo:
            // BEGIN MOD1
            // SPACE X <--- definicao em linha zero, nao vai ser considerada
            // END
            if (adress_symbol != "00") definitions.add(label_symbol, adress_symbol);
            else{
                for(const auto &[adress, label] : *pendencies.getData()) if (label_symbol == label) used.add(label, adress);
            }  
        }
        
        outputFile <<"DEF"<<endl;
        definitions.show(true, outputFile);
        outputFile <<"USO"<<endl;
        used.show(true, outputFile);
        outputFile <<"REAL"<<endl;
        real.show(false, outputFile);
        outputFile << "CODIGO" << endl; 
        codeGenerated.show(false, outputFile);

    } else {
        // std::cout<<"-----Symbols------"<<endl;
        // symbols.show();
        // std::cout<<"-----Pendencies------"<<endl;
        // pendencies.show();
        outputFile <<"-----Code------"<<endl;
        codeGenerated.show(false);
        outputFile <<"------Real-----"<<endl;
        real.show(false);   
    }

    file.close();
    outputFile.close();

}

void lerArquivo(ifstream& file, Table<string, int>& def, Table<string, int>& uso, vector<int>& codigo) {
    string linha;
    string secao;

    while (getline(file, linha)) {
        if (linha == "DEF" || linha == "USO" || linha == "REAL" || linha == "CODIGO") {
            secao = linha;
        } else {
            if (secao == "DEF") {
                string chave;
                int valor;
                istringstream iss(linha);
                iss >> chave >> valor;
                def.add(chave, valor);
            } else if (secao == "USO") {
                string chave;
                int valor;
                istringstream iss(linha);
                iss >> chave >> valor;
                uso.add(chave, valor);
            } else if (secao == "CODIGO") {
                int valor;
                istringstream iss(linha);
                while (iss >> valor) {
                    codigo.push_back(valor);
                }
            }
        }
    }
}

void ligador(const string& nomeArquivo1, const string& nomeArquivo2, string outputFilePath){
    // gerar o executavel
    Table<string, int> def1, def2;
    Table<string, int> uso1, uso2;
    vector<int> codigo1, codigo2;
    string linha;
    string secao;
    ifstream file1(nomeArquivo1);
    ifstream file2(nomeArquivo2);
    ofstream outputFile(outputFilePath);

    if (!file1 || !file2) {
        cerr << "Erro ao abrir o arquivos " << nomeArquivo1 << endl;
        return;
    }
    lerArquivo(file1, def1, uso1, codigo1);
    lerArquivo(file2, def2, uso2, codigo2);

    // cout << "def" << endl;
    // def.show();
    // cout << "uso" << endl;
    // uso.show();
    // cout << "codigo" << endl;

    int fator_de_correcao = (codigo1.size());

    // trocar valores no codigo 1 que sao usados do codigo 2
    for(const auto &[label, adress] : *uso1.getData()) {
        if (def2.get(label) == nullptr) codigo1[adress] = 0+fator_de_correcao;
        else codigo1[adress] = *def2.get(label)+fator_de_correcao; 
    }

    // aumentar todo endereço relativo do codigo 2 para += fator de correcaos
    // caso tenha copy, aumenta os dois # relativos
    // caso tenho um mneumonico (opcode 0), para de realizar a soma
    int on=0;
    int opcode=0;
    for (auto& item : codigo2) {
        
        if (on && item != 0) {
            if (opcode == 9) on=0;
            if (opcode == 0) break;            
            item += fator_de_correcao;
        }
        opcode=item;
        on = !on;
    }

    // alterar endereços relativos do codigo 1 que estao no codigo 2
    for(const auto &[label, adress] : *uso2.getData()) {
        if (def1.get(label) == nullptr) codigo2[adress] = fator_de_correcao;
        else codigo2[adress] = *def1.get(label); 
    }


    //cout << "CODIGO 1:" << endl;
    for (const auto& item : codigo1) {
        outputFile << item << " ";
    }
    //cout << endl;
    //    cout << "CODIGO 2:" << endl;
    for (const auto& item : codigo2) {
        outputFile << item << " ";
    }
    //cout <<endl;
    outputFile.close();
    file1.close();
    file2.close();
}



int main() {

    // TODO:  pre-processamento
    //preprocessa("../exemplos/ex4.txt", "../myfile.pre");
    //montador("../exemplos/ex5.txt", "../prog1.obj");
    ligador("../prog1.obj", "../prog2.obj", "../prog1.e");
    // montador("../exemplos/ex2.txt");
    // montador("../ex3.txt"); 
    
    
    return 0;
}