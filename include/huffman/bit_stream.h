#ifndef HUFFMAN_BIT_STREAM_H
#define HUFFMAN_BIT_STREAM_H

#include <istream>
#include <ostream>
#include <cstdint>

class BitOutputStream {
public:
    explicit BitOutputStream(std::ostream &out);
    ~BitOutputStream();
    void writeBit(bool bit);
    void writeBits(uint64_t bits, unsigned int count); // writes 'count' least-significant bits
    void flush();
private:
    std::ostream &out_;
    unsigned char buffer_;
    int nbits_;
};

class BitInputStream {
public:
    explicit BitInputStream(std::istream &in);
    int readBit(); // returns 0 or 1, or -1 at EOF
private:
    std::istream &in_;
    unsigned char buffer_;
    int nbits_;
};

#endif
