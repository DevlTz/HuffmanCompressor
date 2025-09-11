#include "../../include/huffman/bit_stream.h"
#include <iostream>

BitOutputStream::BitOutputStream(std::ostream &out): out_(out), buffer_(0), nbits_(0) {}

BitOutputStream::~BitOutputStream(){ flush(); }

void BitOutputStream::writeBit(bool bit){
    buffer_ <<= 1;
    if(bit) buffer_ |= 1;
    nbits_++;
    if(nbits_ == 8) flush();
}

void BitOutputStream::writeBits(uint64_t bits, unsigned int count){
    for(int i = count-1; i>=0; --i){
        bool b = ((bits >> i) & 1);
        writeBit(b);
    }
}

void BitOutputStream::flush(){
    if(nbits_ == 0) return;
    // shift to MSB first order:
    unsigned char outb = buffer_ << (8 - nbits_);
    out_.put(static_cast<char>(outb));
    buffer_ = 0;
    nbits_ = 0;
}

BitInputStream::BitInputStream(std::istream &in): in_(in), buffer_(0), nbits_(0) {}

int BitInputStream::readBit(){
    if(nbits_ == 0){
        int c = in_.get();
        if(c == EOF) return -1;
        buffer_ = static_cast<unsigned char>(c);
        nbits_ = 8;
    }
    int bit = (buffer_ >> (nbits_-1)) & 1;
    --nbits_;
    return bit;
}
