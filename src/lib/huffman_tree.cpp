#include "../../include/huffman/huffman_tree.h"
#include "../../include/huffman/bit_stream.h"
#include <queue>
#include <iostream>

void serialize_helper(std::shared_ptr<HNode> node, BitOutputStream& bit_out) {
    if (node == nullptr) {
        return;
    }
    // Lógica de pré-ordem para visitar a raiz
    if (node->isLeaf()) {
        bit_out.writeBit(true);
        // Escreve o tamanho da string como um único byte, com 8 bits para o tamanho
        uint8_t len = node->symbol.length();
        bit_out.writeBits(len, 8); 
    
        for (char c : node->symbol) {
            bit_out.writeBits(static_cast<uint8_t>(c), 8);
        }
        //Caso nó interno
    } else {
       
        bit_out.writeBit(false);
        // Percorre recursivamente a esquerda e a direita (Assim como o conceito de pré-ordem)
        serialize_helper(node->left, bit_out);
        serialize_helper(node->right, bit_out);
    }
}

std::shared_ptr<HNode> deserialize_helper(BitInputStream& bit_in) {
    int bit = bit_in.readBit();
    if (bit == -1) { 
        return nullptr;
    }

    // Caso nó-folha
    if (bit == 1) { 
        auto node = std::make_shared<HNode>();
        
        uint8_t len = 0;
        for(int i=0; i<8; ++i){
            len = (len << 1) | bit_in.readBit();
        }

        // FAz A leitrua dos caracteres do símbolo
        std::string symbol = "";
        for(int i=0; i<len; ++i){
            uint8_t ch = 0;
            for(int j=0; j<8; ++j){
                ch = (ch << 1) | bit_in.readBit();
            }
            symbol += static_cast<char>(ch);
        }
        node->symbol = symbol;
        return node;
    } 
    // Caso nó interno
    else { 
        auto node = std::make_shared<HNode>();
        node->symbol = ""; // Nós internos não têm símbolo
        node->left = deserialize_helper(bit_in);
        node->right = deserialize_helper(bit_in);
        return node;
    }
}

HuffmanTree::HuffmanTree(): root_(nullptr) {}

void HuffmanTree::build(const std::map<std::string, uint64_t> &freqs){
    struct QNode {
        std::shared_ptr<HNode> node;
        uint64_t freq;
        bool operator<(const QNode& other) const { return freq > other.freq; }
    };

    std::priority_queue<QNode> pq;
    for(const auto& pair : freqs){
        if(pair.second > 0){
            auto n = std::make_shared<HNode>();
            n->symbol = pair.first;
            n->freq = pair.second;
            pq.push(QNode{n, n->freq});
        }
    }

    if(pq.empty()){ return; }

    while(pq.size() > 1){
        auto a = pq.top(); pq.pop();
        auto b = pq.top(); pq.pop();
        auto parent = std::make_shared<HNode>();
        parent->symbol = ""; // Nós internos são vazios para servir apenas como conectores de estruturas
        parent->left = a.node;
        parent->right = b.node;
        parent->freq = a.freq + b.freq;
        pq.push(QNode{parent, parent->freq});
    }
    root_ = pq.top().node;
}

std::map<std::string, std::vector<bool>> HuffmanTree::getCodes() const {
    std::map<std::string, std::vector<bool>> out;
    if(root_) {
        std::vector<bool> path;
        buildCodes(root_, path, out);
    }
    return out;
}


void HuffmanTree::buildCodes(std::shared_ptr<HNode> node, std::vector<bool> &path, std::map<std::string, std::vector<bool>> &out) const {
    if(!node) return;
    if(node->isLeaf()){
        out[node->symbol] = path;
        return;
    }
    path.push_back(false);
    buildCodes(node->left, path, out);
    path.pop_back(); // Desfaz a mudança para o caminho da direita
    
    path.push_back(true);
    buildCodes(node->right, path, out);
    path.pop_back(); // Limpa o caminho ao retornar da recursão
}


void HuffmanTree::serialize(std::ostream &out) const {
    BitOutputStream bit_out(out);
    serialize_helper(root_, bit_out);
    bit_out.flush(); // Garante que qualquer bit restante no buffer seja escrito
}

void HuffmanTree::deserialize(std::istream &in) {
    BitInputStream bit_in(in);
    root_ = deserialize_helper(bit_in);
}

// Util para o descompressor
std::shared_ptr<HNode> HuffmanTree::getRoot() const {
    return root_;
}
