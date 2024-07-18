#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <vector>

using namespace std;

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

#endif 