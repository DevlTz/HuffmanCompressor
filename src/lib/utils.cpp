#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// utils: simple helpers (e.g., read entire file into vector)
std::vector<char> readFileBytes(const std::string &path){
    std::ifstream in(path, std::ios::binary);
    if(!in) return {};
    std::vector<char> data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return data;
}
