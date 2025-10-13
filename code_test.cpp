#include <iostream>
#include <vector>

// Um loop simples para testar palavras-chave
int main() {
    std::vector<int> vec;
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }

    if (!vec.empty()) {
        std::cout << "O vetor nao esta vazio!" << std::endl;
    }
    return 0;
}