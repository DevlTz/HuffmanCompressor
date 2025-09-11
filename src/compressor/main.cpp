#include <iostream>
#include <fstream>
#include <string>
#include "../../include/huffman/bit_stream.h"
#include "../../include/huffman/huffman_tree.h"

int main(int argc, char** argv){
    if(argc < 2){
        std::cerr << "Usage: compressor -c|-d ...\n";
        return 1;
    }
    // This is a stub. Implement parsing -c (compress) and -d (decompress) and call appropriate functions.
    std::cout << "Compressor stub. Implement command-line parsing and logic.\n";
    return 0;
}
