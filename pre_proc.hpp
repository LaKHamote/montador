#ifndef PRE_PROC_HPP
#define PRE_PROC_HPP

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <vector>

#include "./utils.hpp"


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


#endif 