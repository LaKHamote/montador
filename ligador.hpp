#ifndef LIGADOR_HPP
#define LIGADOR_HPP

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



#endif 