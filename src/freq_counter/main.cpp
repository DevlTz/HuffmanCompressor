#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include "../../include/huffman/huffman_tree.h"

// Simple freq_counter stub: count bytes and print summary
int main(int argc, char** argv){
    if(argc < 2){
        std::cerr << "Usage: freq_counter <file> [-o out.txt]\n";
        return 1;
    }
    std::vector<uint64_t> freqs(256,0);
    for(int i=1;i<argc;i++){
        std::string fname = argv[i];
        if(fname == "-o"){ ++i; break; }
        std::ifstream in(fname, std::ios::binary);
        if(!in){ std::cerr << "Cannot open " << fname << "\n"; continue; }
        char c;
        while(in.get(c)){
            unsigned char uc = static_cast<unsigned char>(c);
            freqs[uc]++;
        }
    }
    // print non-zero frequencies
    for(size_t i=0;i<freqs.size();++i){
        if(freqs[i]) std::cout << i << " -> " << freqs[i] << "\n";
    }
    return 0;
}
