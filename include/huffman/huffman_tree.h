#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <cstdint>
#include <memory>
#include <vector>
#include <map>
#include "../../include/huffman/bit_stream.h"

// struct HNode {
//     uint8_t symbol;
//     uint64_t freq;
//     std::shared_ptr<HNode> left, right;
//     bool isLeaf() const { return !left && !right; }
// };

struct HNode {
    std::string symbol; 
    uint64_t freq;
    std::shared_ptr<HNode> left, right;
    //  só é "folha"  se tiver um símbolo && não tiver filhos.
    bool isLeaf() const { return !symbol.empty() && !left && !right; } 
};

class HuffmanTree {
public:
    HuffmanTree();
    void build(const std::map<std::string, uint64_t> &freqs); // freqs size 256
    std::map<std::string, std::vector<bool>> getCodes() const;
    std::shared_ptr<HNode> getRoot() const;
    void serialize(std::ostream &out) const;
    void deserialize(std::istream &in);

private:
    std::shared_ptr<HNode> root_;
    void buildCodes(std::shared_ptr<HNode> node, std::vector<bool> &path, std::map<std::string, std::vector<bool>> &out) const;
    void serialize_helper(std::shared_ptr<HNode> node, BitOutputStream& bit_out) const;
    std::shared_ptr<HNode> deserialize_helper(BitInputStream& bit_in);
};
#endif

