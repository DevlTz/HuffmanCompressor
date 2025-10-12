#include <iostream>
#include "../../include/compressor/compressor.h"

using namespace std;

static void printUsage() {
    cerr << "Usage:\n"
         << "  compressor -c [-f freq_table] -i <input> -o <output>\n"
         << "  compressor -d -i <input> -o <output>\n";
}

int main(int argc, char** argv) {
    if(argc < 2) { printUsage(); return 1; }

    bool doCompress = false, doDecompress = false;
    string freqFile, inputFile, outputFile;

    for(int i=1;i<argc;i++){
        string a = argv[i];
        if(a == "-c") doCompress = true;
        else if(a == "-d") doDecompress = true;
        else if(a == "-f" && i+1<argc) { freqFile = argv[++i]; }
        else if(a == "-i" && i+1<argc) { inputFile = argv[++i]; }
        else if(a == "-o" && i+1<argc) { outputFile = argv[++i]; }
        else {
            cerr << "Unknown arg: " << a << "\n";
            printUsage();
            return 1;
        }
    }

    try {
        if(doCompress && doDecompress) { cerr << "Cannot both compress and decompress.\n"; return 1; }
        if(doCompress) {
            if(inputFile.empty() || outputFile.empty()) { printUsage(); return 1; }
            compressor::compress_file(inputFile, outputFile, freqFile);
            cout << "Compressed " << inputFile << " -> " << outputFile << endl;
        } else if(doDecompress) {
            if(inputFile.empty() || outputFile.empty()) { printUsage(); return 1; }
            compressor::decompress_file(inputFile, outputFile);
            cout << "Decompressed " << inputFile << " -> " << outputFile << endl;
        } else {
            printUsage();
            return 1;
        }
    } catch(const exception &ex) {
        cerr << "Error: " << ex.what() << "\n";
        return 2;
    }

    return 0;
}
