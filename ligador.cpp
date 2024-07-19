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

string alterarExtensaoParaE(const string& nomeArquivo) {
    size_t pos = nomeArquivo.rfind(".obj");
    if (pos != string::npos) {
        return nomeArquivo.substr(0, pos) + ".e";
    }
    return nomeArquivo + ".e";
}


// detalhe IMPORTANTE:
// o local onde voce chamou essa função (seu/caminho/...../montador/bin) vai ser onde o out vai sair.
// exemplo:
// .\ligador.exe ../out/prog5.obj ../out/prog6.obj ---> gera -----> ../out/prog6.e

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Uso: ./ligador <arquivo1> <arquivo2>" << endl;
        return 1;
    }

    string nomeArquivo1 = argv[1];
    string nomeArquivo2 = argv[2];
    string outputFilePath = alterarExtensaoParaE(nomeArquivo2);

    ligador(nomeArquivo1, nomeArquivo2, outputFilePath);

    return 0;
}
