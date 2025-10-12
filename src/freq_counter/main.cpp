#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include "../../include/huffman/huffman_tree.h"
#include <set>


#include <string> // Ensure this is at the top

// Função para escapar caracteres especiais
std::string escape_chars(const std::string& s) {
    std::string escaped;
    for (char c : s) {
        if (c == '\n') {
            escaped += "\\n";
        } else if (c == '\r') {
            escaped += "\\r";
        } else if (c == '\t') {
            escaped += "\\t";
        } else if (c == '\\') {
            escaped += "\\\\";
        } else {
            escaped += c;
        }
    }
    return escaped;
}

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
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
    "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
    "class", "compl", "concept", "const", "consteval", "constexpr", "constinit",
    "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype",
    "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
    "explicit", "export", "extern", "false", "float", "for", "friend", "goto",
    "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept",
    "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private",
    "protected", "public", "register", "reinterpret_cast", "requires", "return",
    "short", "signed", "sizeof", "static", "static_assert", "static_cast",
    "struct", "switch", "template", "this", "thread_local", "throw", "true",
    "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
    "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq",
    // Identificadores com significado especial
    "final", "override", "import", "module",
    // Diretivas de pré-processador (muito comuns em .cpp/.h)
    "define", "elif", "elifdef", "elifndef", "embed", "endif", "error",
    "ifdef", "ifndef", "include", "line", "pragma", "undef", "warning",
    // Tipos e classes comuns da (STL)
    "std", "string", "vector", "map", "set", "iostream", "fstream", "sstream",
    "memory", "shared_ptr", "unique_ptr", "make_shared", "make_unique",
    "cout", "cin", "cerr", "endl"
}; //Fonte das palavras reservadas: https://en.cppreference.com/w/cpp/keywords.html e arquivos .h de antigos projetos


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
    (*out_stream) << escape_chars(pair.first) << ":" << pair.second << "\n";
}

    if (out_file.is_open()) {
        out_file.close();
        std::cout << "Frequency table saved in" << outputFile << "\n";
    }

    return 0;
}