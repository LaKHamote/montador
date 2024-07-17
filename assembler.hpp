#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

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



#endif 
