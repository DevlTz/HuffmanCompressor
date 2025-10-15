# Huffman Compressor

Repositório de exemplo para um projeto em C++ que implementa compressão e descompressão usando o algoritmo de Huffman.
Este repositório contém:
- `contador_frequencia`: programa que conta frequência de símbolos (bytes) em arquivos.
- `compressor`: compressor/descompressor que usa Huffman.
- Helpers (`BitInputStream`/`BitOutputStream`) para leitura/escrita de bits.
- Scripts para comparar taxa de compressão com gzip/zip/7z e automatizar testes.
- `CMake` para facilitar build. (Vamos trabalhar nisso)
---

## Visão geral

O projeto contém duas ferramentas principais:

- **freq_counter** (contador de frequência): analisa ficheiros e gera uma tabela de frequências de símbolos/tokens.
- **compressor**: cria a árvore de Huffman a partir da tabela de frequência, comprime ficheiros e também descomprime ficheiros `.huff`.

Também há helpers para leitura/escrita de bits (`BitInputStream` / `BitOutputStream`) e scripts para comparar taxas de compressão.

---

## Estrutura do repositório

Abaixo está a árvore do repositório extraída do zip fornecido:

```
HuffmanCompressor/
└── HuffmanCompressor-main
    ├── .gitignore
    ├── CMakeLists.txt
    ├── LICENSE
    ├── README.md
    ├── docs
    │   ├── compressor_md
    │   │   ├── design.md
    │   │   └── report.md
    │   ├── report_work
    │   │   ├── EDB2_14_10_25.pdf
    │   │   ├── relatorio.md
    │   │   └── relatorio.tex
    │   └── results.csv
    ├── examples
    │   ├── codes_test
    │   │   ├── medium_code.java
    │   │   ├── medium_code.js
    │   │   ├── readme.md
    │   │   ├── simple_code.cpp
    │   │   ├── simple_code.py
    │   │   └── simple_code.ts
    │   ├── huff_generate
    │   │   └── decompressed_code
    │   └── tabelas_freq
    │       └── readme.md
    ├── include
    │   ├── compressor
    │   │   └── compressor.h
    │   └── huffman
    │       ├── bit_stream.h
    │       └── huffman_tree.h
    ├── scripts
    │   ├── compare_compressors.sh
    │   └── full_test.sh
    └── src
        ├── compressor
        │   ├── compressor.cpp
        │   └── main.cpp
        ├── freq_counter
        │   └── main.cpp
        └── lib
            ├── bit_stream.cpp
            ├── huffman_tree.cpp
            └── utils.cpp
```



---

## Requisitos

- Compilador C++ com suporte a **C++17** (`g++`, `clang++`).
- **CMake** (3.10+ recomendado).
- **make** (ou equivalente).
- (Opcional) `gzip`, `zip`, `7z` para comparação de taxas (scripts).

---

## Compilação com CMake (recomendado)

1. Acesse a raiz do repositório:


2. Crie e entre na pasta de build:
```bash
mkdir -p build
cd build
```

3. Configure com CMake e compile:
```bash
cmake ..
make 
```

Se o `CMakeLists.txt` estiver devidamente configurado os executáveis (`freq_counter`, `compressor` ou nomes em português semelhantes) aparecerão em `build/` ou em `build/bin/`.

---

## Compilação direta com g++ (alternativa)

Se preferir compilar manualmente (útil para debugging rápido), exemplos de comandos:

```bash

g++ -std=c++17 -o contador_frequencia src/freq_counter/main.cpp src/lib/huffman_tree.cpp src/lib/bit_stream.cpp src/lib/utils.cpp -I./include -Wall

g++ -std=c++17 -o compressor src/compressor/main.cpp src/compressor/compressor.cpp src/lib/huffman_tree.cpp src/lib/bit_stream.cpp -I./include -Wall
```
---

## Uso básico

### Gerar tabela de frequência
```bash
# Exemplo
./contador_frequencia /examples/codes_test/simple_code.cpp -o simple_cpp.freq
```

### Comprimir
```bash
./compressor -c -f simple_cpp.freq -i /examples/codes_test/simple_code.cpp -o /huff_generate/compressed_code/simple_code.huff
```

### Descomprimir
```bash
./compressor -d -i /huff_generate/compressed_code/simple_code.huff -o /huff_generate/decompressed_code/simple_code.cpp
```

### Verificar integridade
```bash
diff /examples/codes_test/simple_code.cpp /huff_generate/decompressed_code/simple_code.cpp
# Sem saída => ficheiros idênticos
```

---

## Testes automatizados (`scripts/full_test.sh`)

O repositório contém um script de testes chamado `scripts/full_test.sh` que automatiza:

1. Compilação do projecto;
2. Compressão e descompressão de todos os ficheiros de exemplo;
3. Verificação (diff) entre originais e ficheiros restaurados;
4. (Opcional) Geração de relatórios/CSV comparando com gzip/zip/7z, se houver scripts complementares.

### Como usar
```bash
# Dê permissão de execução (uma vez)
chmod +x ./scripts/full_test.sh

# Execute
./scripts/full_test.sh
```

O script imprimirá resultados por ficheiro e um resumo final. Se desejar, abra o script para ver as opções internas (por exemplo, caminhos de entrada/saída, etc.).

---

## Scripts úteis

- `scripts/full_test.sh` — teste automatizado (compila e valida todo o fluxo).
- `scripts/compare_compressors.sh` — compara taxas de compressão do `compressor` com `gzip`, `zip` e `7z` e produz um CSV.

---

## Formato `.huff` e segurança da descompressão

O formato de saída `.huff` inclui:
- a árvore de Huffman serializada;
- o número total de símbolos/token;
- os dados comprimidos (bits).

Ter a árvore e a contagem de símbolos permite descomprimir sem ambiguidade e evita problemas com padding de bits.

---

## Boas práticas e dicas

- Gere a tabela de frequência com os mesmos tokens/estratégia que o compressor espera (tokenização consistente).
- Quando fizer testes em massa, use `scripts/full_test.sh` para economizar tempo.
- Se alterar o formato da tabela de frequência, atualize também o `compressor` e os scripts que consumem essa tabela.
- Para debug de compressão, gere um pequeno ficheiro de teste e examine a árvore gerada (adicionar prints temporários no código pode ajudar).

---

## Autores

- Kauã do Vale Ferreira
- Caio de Medeiros Trindade

---


## Referências e leituras feitas aqui

- [Artigo sobre Huffman Coding](https://medium.com/@davidsonbrsilva/compress%C3%A3o-de-dados-pelo-algoritmo-de-huffman-5e04bc437d77#:~:text=O%20c%C3%B3digo%20de%20Huffman%20%C3%A9,que%20isso%20seja%20uma%20regra.)
- [Algoritmo de Huffman](https://youtu.be/BDwtGMhGNc0)
- [Exemplos de BitStream em C++ (leitura de bits)](https://pt.wikipedia.org/wiki/Codifica%C3%A7%C3%A3o_de_Huffman)


---
## Licença

Distribuído sob a Licença MIT. Veja o ficheiro `LICENSE` para detalhes.

---
