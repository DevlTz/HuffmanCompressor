#include "../../include/huffman/huffman_tree.h"
#include <queue>
#include <iostream>

HuffmanTree::HuffmanTree(): root_(nullptr) {}

void HuffmanTree::build(const std::vector<uint64_t> &freqs){
    struct QNode {
        std::shared_ptr<HNode> node;
        uint64_t freq;
        bool operator<(QNode const& other) const { return freq > other.freq; } // for min-heap
    };
    std::priority_queue<QNode> pq;
    for(int i=0;i<256;i++){
        if(freqs[i]>0){
            auto n = std::make_shared<HNode>();
            n->symbol = static_cast<uint8_t>(i);
            n->freq = freqs[i];
            pq.push(QNode{n, freqs[i]});
        }
    }
    if(pq.empty()){
        return;
    }
    while(pq.size() > 1){
        auto a = pq.top(); pq.pop();
        auto b = pq.top(); pq.pop();
        auto parent = std::make_shared<HNode>();
        parent->left = a.node;
        parent->right = b.node;
        parent->freq = a.freq + b.freq;
        pq.push(QNode{parent, parent->freq});
    }
    root_ = pq.top().node;
}

std::map<uint8_t, std::vector<bool>> HuffmanTree::getCodes() const {
    std::map<uint8_t, std::vector<bool>> out;
    if(root_) {
        std::vector<bool> path;
        const_cast<HuffmanTree*>(this)->buildCodes(root_, path, out);
    }
    return out;
}

void HuffmanTree::buildCodes(std::shared_ptr<HNode> node, std::vector<bool> &path, std::map<uint8_t, std::vector<bool>> &out) const {
    if(!node) return;
    if(node->isLeaf()){
        out[node->symbol] = path;
        return;
    }
    path.push_back(false);
    buildCodes(node->left, path, out);
    path.back() = true;
    buildCodes(node->right, path, out);
    path.pop_back();
}

void HuffmanTree::serialize(std::ostream &out) const {
    // stub: implement pre-order serialization with bits
    out << ""; // placeholder
}

void HuffmanTree::deserialize(std::istream &in) {
    // stub: implement de-serialization
}
