#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <cstdint>
#include <memory>
#include <vector>
#include <map>

struct HNode {
    uint8_t symbol;
    uint64_t freq;
    std::shared_ptr<HNode> left, right;
    bool isLeaf() const { return !left && !right; }
};

class HuffmanTree {
public:
    HuffmanTree();
    void build(const std::vector<uint64_t> &freqs); // freqs size 256
    std::map<uint8_t, std::vector<bool>> getCodes() const;
    void serialize(std::ostream &out) const;
    void deserialize(std::istream &in);
private:
    std::shared_ptr<HNode> root_;
    void buildCodes(std::shared_ptr<HNode> node, std::vector<bool> &path, std::map<uint8_t, std::vector<bool>> &out) const;
};

#endif
