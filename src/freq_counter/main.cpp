#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include "../../include/huffman/huffman_tree.h"
#include <set>


//Function to help in case the loop find a char in the file
std::string charToString(char c) {
    return std::string(1, c);
}

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "Usage: freq_counter <file1> [file2...] [-o out.freq]\n";
        return 1;
    }

    std::map<std::string, uint64_t> freqs;
    std::set<std::string> keyWords = {
        "int", "double", "while", "return", "include", "new", "class", "public",
        "private", "protected", "if", "else", "for", "switch", "case", "break",
        "continue", "struct", "enum", "namespace", "using", "const", "char",
        "void", "unsigned", "long", "short", "bool", "true", "false", "nullptr", "Node", "Interface"
        //Adicioanr mais, tendo em vista que o professor pediu que seja compressor de várias linguagens
    };

    std::vector<std::string> inputFiles;
    std::string outputFile = "";

   
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-o") {

            if (i + 1 < argc) {
                outputFile = argv[i + 1];
                i++;
            } else {
                std::cerr << "Erro: must have a file after -o \n";
                return 1;
            }
        } 
        else {
            inputFiles.push_back(arg);
        }
    }

    if (inputFiles.empty()) {
        std::cerr << "Error: Input file not found \n";
        return 1;
    }

    for (const std::string& fname : inputFiles) {
        std::ifstream in(fname); 
        if (!in) {
            std::cerr << "Error: File didnt open" << fname << "\n";
            continue;
        }
        char c;
        
        // O loop principal lê um caractere de cada vez
        while (in.get(c)) {
            
            //3 condições diferentes, para identificar as possibilidades existentes de simbolos

            // Identificar o inicio de alguma palavra reservada ou variável 
            if (std::isalpha(c) || c == '_') {
                std::string word;
                word.push_back(c);

                //in.peek() identifica o proximo char sem consumir
                while (in.peek() != EOF && (std::isalnum(in.peek()) || in.peek() == '_')) {
                    word.push_back(in.get());
                }

                if (keyWords.count(word)) {
                
                    freqs[word]++;
                } else {

                    for (char ch : word) {
                        freqs[charToString(ch)]++;
                    }
                }
            }

            //Identificar o inicio de espaços, como \n, espaço em branco

            else if (std::isspace(c)) {
                freqs[charToString(c)]++;
            }
            else {              
                freqs[charToString(c)]++;
            }
        }
    }

    std::ostream* out_stream = &std::cout;
    std::ofstream out_file;

    if (!outputFile.empty()) {
        out_file.open(outputFile);
        if (!out_file) {
            std::cerr << "Could not create output file" << outputFile << "\n";
            return 1;
        }
        out_stream = &out_file;
    }

    // Formato do map simbolo:fequencia
    for (const auto& pair : freqs) {
        (*out_stream) << pair.first << ":" << pair.second << "\n";
    }

    if (out_file.is_open()) {
        out_file.close();
        std::cout << "Frequency table saved in" << outputFile << "\n";
    }

    return 0;
}