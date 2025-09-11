# Huffman Compressor

Repositório de exemplo para um projeto em C++ que implementa compressão e descompressão usando o algoritmo de Huffman.
Este repositório contém:
- `freq_counter`: programa que conta frequência de símbolos (bytes) em arquivos.
- `compressor`: compressor/descompressor que usa Huffman.
- Helpers (`BitInputStream`/`BitOutputStream`) para leitura/escrita de bits.
- Scripts para comparar taxa de compressão com gzip/zip/7z.
- `CMake` para facilitar build.

--- 

## Estrutura do repositório

```
huffman-compressor/
├─ .github/
│  └─ workflows/ci.yml
├─ cmake/
├─ docs/
│  ├─ report.md
│  └─ design.md
├─ examples/
│  ├─ samples_cpp/
│  └─ small_texts/
├─ include/
│  └─ huffman/
│     ├─ bit_stream.h
│     └─ huffman_tree.h
├─ src/
│  ├─ freq_counter/
│  │  └─ main.cpp
│  ├─ compressor/
│  │  ├─ main.cpp
│  │  ├─ compressor.cpp
│  │  └─ decompressor.cpp
│  └─ lib/
│     ├─ huffman_tree.cpp
│     ├─ bit_stream.cpp
│     └─ utils.cpp
├─ tests/
│  ├─ unit/
│  └─ integration/
├─ scripts/
│  ├─ compare_compressors.sh
│  └─ generate_dataset.sh
├─ CMakeLists.txt
├─ LICENSE.md
├─ README.md
└─ .gitignore
```

---

## Como compilar (CMake)

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

Isso deve gerar os binários `freq_counter` e `compressor` (dependendo do conteúdo do CMakeLists).

---

## Uso básico

Contar frequências:
```bash
./freq_counter arquivo1.txt arquivo2.cpp -o frequencies.txt
```

Comprimir:
```bash
./compressor -c input.txt -o output.huf -t frequencies.txt
```

Descomprimir:
```bash
./compressor -d output.huf -o recovered.txt
```

---

## Formato sugerido do arquivo `.huf`

- Header:
  - 4 bytes: magic (por exemplo: 'HUF\0')
  - 1 byte: versão
  - 2 bytes: flags/reservado
- 4 ou 8 bytes: tamanho da serialização da árvore (em bytes)
- bytes: serialização da árvore (pré-order com marcação de folha)
- bytes: payload comprimido (bits concatenados, com padding no último byte)

Detalhes completos e trade-offs em `docs/design.md`.

---

## Roteiro (por onde começar)

1. Crie o repositório remoto (GitHub/GitLab) e faça o clone local.
2. Configure `CMakeLists.txt` mínimo.
3. Implemente `freq_counter`:
   - objetivo: contar frequência de cada byte (0-255) e escrever `frequencies.txt`.
4. Implemente `HuffmanTree`:
   - construir a árvore a partir das frequências (priority_queue).
   - gerar tabela de códigos (map<byte, string>).
5. Implemente `BitOutputStream` / `BitInputStream`:
   - escrever/ler bits agrupados em bytes.
6. Implemente `compressor`:
   - escrever header + árvore + payload.
7. Implemente `decompressor`:
   - ler header, reconstruir árvore, decodificar bits.
8. Testes:
   - unitários para contagem, geração de códigos e round-trip de compressão.
9. Experimentos de comparação:
   - rode `scripts/compare_compressors.sh` (exemplo) para comparar com `gzip`/`7z`/`zip`.
   - documente resultados em `docs/report.md`.

---

## Scripts úteis

- `scripts/compare_compressors.sh` — roda compressão com seu `compressor` e também `gzip`, `zip`, `7z`, gerando um CSV com os resultados (original, comprimido, taxa).
- `scripts/generate_dataset.sh` — (opcional) baixa ou copia exemplos para `examples/small_texts/`.

---

## Licença

Este repositório usa MIT por padrão (`LICENSE.md`). Mude se preferir outra licença.

---

## Referências e leituras sugeridas

- Artigos/tutorials sobre Huffman Coding (procure por "Huffman coding C++ example")
- Leituras sobre serialização de árvores pré-order com marcação de folhas
- Exemplos de BitStream em C++ (leitura de bits)

--- 

Se quiser, eu posso:
- Gerar código completo dos stubs (freq_counter, BitStream, HTree) já funcional.
- Criar o arquivo ZIP pronto para download (está sendo gerado agora).

