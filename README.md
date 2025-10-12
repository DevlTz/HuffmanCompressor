# Huffman Compressor

Repositório de exemplo para um projeto em C++ que implementa compressão e descompressão usando o algoritmo de Huffman.
Este repositório contém:
- `contador_frequencia`: programa que conta frequência de símbolos (bytes) em arquivos.
- `compressor`: compressor/descompressor que usa Huffman.
- Helpers (`BitInputStream`/`BitOutputStream`) para leitura/escrita de bits.
- Scripts para comparar taxa de compressão com gzip/zip/7z.
- `CMake` para facilitar build. (Vamos trabalhar nisso)

--- 

## Estrutura do repositório

```
alterar a arvore de estrutura do repo.
```

---

## Como compilar (CMake)

```bash
Vamos mudar isso ( tem que alterar o cmake para compilar tudo)

enquanto isso...
```
## Como compilar (G++)

```bash

 g++ -std=c++17 -o contador_frequencia src/freq_counter/main.cpp src/lib/huffman_tree.cpp src/lib/bit_stream.cpp src/lib/utils.cpp -I./include -Wall

 g++ -std=c++17 -o compressor src/compressor/main.cpp src/compressor/compressor.cpp src/lib/huffman_tree.cpp src/lib/bit_stream.cpp -I./include -Wall

```


Isso deve gerar os binários `contador_frequencia` e `compressor` (dependendo do conteúdo do CMakeLists).

---

## Uso básico

Contar frequências:
```bash
./contador_frequencia code_test.cpp -o tabela_cpp.freq  
```

Comprimir:
```bash
./compressor -c -f tabela_cpp.freq -i code_test.cpp -o codigo_comprimido.huff     
```

Descomprimir:
```bash
./compressor -d -i codigo_comprimido.huff -o codigo_restaurado.cpp    
```
Caso necessário remover e começar novamente:
```rm -f compressor contador_frequencia tabela_cpp.freq codigo_comprimido.huff codigo_restaurado.cpp ```

Veja a diferença em;
```diff code_test.cpp codigo_restaurado.cpp```


Vale ressaltar que vamos nos programar para alterar e imprimir corretamente na saída que se encontra ```examples/huff_generate/*
Detalhes completos em `docs/`.

---

## Scripts úteis

- `scripts/compare_compressors.sh` — roda compressão com seu `compressor` e também `gzip`, `zip`, `7z`, gerando um CSV com os resultados (original, comprimido, taxa).


---

## Licença

Este repositório usa MIT por padrão (`LICENSE.md`).

---

## Referências e leituras sugeridas

- [Artigo sobre Huffman Coding](https://medium.com/@davidsonbrsilva/compress%C3%A3o-de-dados-pelo-algoritmo-de-huffman-5e04bc437d77#:~:text=O%20c%C3%B3digo%20de%20Huffman%20%C3%A9,que%20isso%20seja%20uma%20regra.)
- [Algoritmo de Huffman](https://youtu.be/BDwtGMhGNc0)
- [Exemplos de BitStream em C++ (leitura de bits)](https://pt.wikipedia.org/wiki/Codifica%C3%A7%C3%A3o_de_Huffman)
