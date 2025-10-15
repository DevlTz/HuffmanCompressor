#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <cstdint>
#include <cstring>

#include "../../include/compressor/compressor.h"
#include "../../include/huffman/huffman_tree.h"
#include "../../include/huffman/bit_stream.h"

using namespace std;

namespace compressor {

// =================================================================================
// FUNÇÕES AUXILIARES
// =================================================================================

// Lê um arquivo inteiro para uma string.
static string readFileToString(const string &path) {
    ifstream in(path, ios::binary);
    if (!in) throw runtime_error("Nao foi possivel abrir o arquivo de entrada: " + path);
    ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Conta a frequência de cada byte individualmente.
static map<string, uint64_t> countByteFreqs(const string &data) {
    map<string, uint64_t> freqs;
    for (unsigned char c : data) {
        string s(1, static_cast<char>(c));
        freqs[s]++;
    }
    return freqs;
}

// Converte sequências de escape (ex: "\\n") para seus caracteres reais (ex: '\n').
static string unescape_chars(const std::string& s) {
    std::string unescaped;
    for (size_t i = 0; i < s.length(); ++i) {
        if (s[i] == '\\' && i + 1 < s.length()) {
            i++; // Pula a barra invertida
            if (s[i] == 'n') {
                unescaped += '\n';
            } else if (s[i] == 'r') {
                unescaped += '\r';
            } else if (s[i] == 't') {
                unescaped += '\t';
            } else if (s[i] == '\\') {
                unescaped += '\\';
            }
        } else {
            unescaped += s[i];
        }
    }
    return unescaped;
}

// Lê o arquivo de tabela de frequência ("simbolo:contagem").
static map<string, uint64_t> readFreqTable(const string &path) {
    map<string, uint64_t> freqs;
    if (path.empty()) return freqs;
    ifstream in(path, ios::binary);
    if (!in) throw runtime_error("Nao foi possivel abrir o arquivo de tabela de frequencia: " + path);
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        size_t pos = line.rfind(':');
        if (pos == string::npos) continue;

        string key = line.substr(0, pos);
        string val = line.substr(pos + 1);
        
        try {
            uint64_t v = stoull(val);
            freqs[unescape_chars(key)] = v;
        } catch (...) {
            // Ignora linhas com formato inválido
        }
    }
    return freqs;
}

// Quebra o texto de entrada em tokens (símbolos) usando a abordagem "greedy".
static vector<string> tokenize_greedy(const string &text, const vector<string> &symbols_sorted) {
    vector<string> out;
    size_t i = 0;
    const size_t n = text.size();
    while (i < n) {
        bool matched = false;
        for (const string &sym : symbols_sorted) {
            size_t L = sym.size();
            if (L == 0) continue;
            if (i + L <= n && memcmp(text.data() + i, sym.data(), L) == 0) {
                out.push_back(sym);
                i += L;
                matched = true;
                break;
            }
        }
        if (!matched) {
            out.emplace_back(string(1, text[i]));
            ++i;
        }
    }
    return out;
}


// =================================================================================
// FUNÇÕES PRINCIPAIS
// =================================================================================

// Função principal para comprimir um arquivo.
void compress_file(const string &inputPath, const string &outputPath, const string &freq_table_path) {
    string data = readFileToString(inputPath);
    map<string, uint64_t> freqs = readFreqTable(freq_table_path);
    vector<string> tokens;

    // Se uma tabela de frequência foi fornecida, usa a tokenização inteligente.
    if (!freqs.empty()) {
        vector<string> symbols;
        for (const auto &p : freqs) {
            symbols.push_back(p.first);
        }
        sort(symbols.begin(), symbols.end(), [](const string &a, const string &b) {
            if (a.size() != b.size()) return a.size() > b.size();
            return a < b;
        });
        
        tokens = tokenize_greedy(data, symbols);
    } else {
        // Senão, faz a compressão simples por byte.
        freqs = countByteFreqs(data);
        for (unsigned char c : data) {
            tokens.emplace_back(string(1, static_cast<char>(c)));
        }
    }

    // Constrói a Árvore de Huffman.
    HuffmanTree tree;
    tree.build(freqs);

    ofstream out(outputPath, ios::binary);
    if (!out) throw runtime_error("Nao foi possivel abrir o arquivo de saida: " + outputPath);
    
    uint64_t token_count = tokens.size();
    out.write(reinterpret_cast<const char*>(&token_count), sizeof(token_count));
  
    // Salva a árvore no início do arquivo.
    tree.serialize(out);

    // Escreve os dados comprimidos bit a bit.
    BitOutputStream bout(out);
    auto codes = tree.getCodes();

    for (const auto &tok : tokens) {
        auto it = codes.find(tok);
        if (it == codes.end()) {
            throw runtime_error("Nao foi encontrado codigo para o token durante a compressao. Token: '" + tok + "'");
        }
        for (bool b : it->second) {
            bout.writeBit(b);
        }
    }
    bout.flush();
    out.close();

    cout << "Compressed " << inputPath << " -> " << outputPath << endl;
}

// Função principal para descomprimir um arquivo.
    void decompress_file(const string &inputPath, const string &outputPath) {
        ifstream in(inputPath, ios::binary);
        if (!in) throw runtime_error("Nao foi possivel abrir o arquivo comprimido: " + inputPath);


        uint64_t tokens_to_decode = 0;
        in.read(reinterpret_cast<char*>(&tokens_to_decode), sizeof(tokens_to_decode));
          if (in.gcount() != sizeof(tokens_to_decode)) {
              // Cria um arquivo vazio, pois não há nada para decodificar.
              ofstream out(outputPath, ios::binary);
              out.close();
              return;
          }
        // Lê a árvore do início do arquivo.
        HuffmanTree tree;
        tree.deserialize(in);
        
        auto root = tree.getRoot();
        if (!root) { return; }

        ofstream out(outputPath, ios::binary);
        if (!out) throw runtime_error("Nao foi possivel abrir o arquivo de saida: " + outputPath);

        // Lê os bits e percorre a árvore para decodificar.
        BitInputStream bin(in);
        auto node = root;
        for (uint64_t i = 0; i < tokens_to_decode; ++i) {
            node = root; // Reinicia a busca na árvore para cada novo token
            while (node && !node->isLeaf()) {
                int b = bin.readBit();
                if (b == -1) {
                    // Fim inesperado do arquivo, algo deu errado.
                    // Paramos aqui para evitar corrupção.
                    node = nullptr; // Sinaliza erro para quebrar o loop externo
                    break;
                }
                node = (b == 0) ? node->left : node->right;
            }

            if (node && node->isLeaf()) {
                out << node->symbol;
            } else {
                // Se chegamos aqui, o arquivo está corrompido ou o stream terminou.
                break;
            }
        }

        out.close();
        in.close();

    cout << "Decompressed " << inputPath << " -> " << outputPath << endl;
}

} // namespace compressor
