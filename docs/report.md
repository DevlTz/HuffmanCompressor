# Report (modelo) — Experimentos e métricas

- Metodologia:
  - arquivos testados: lista de arquivos em `examples/small_texts`
  - medidas: tamanho original, tamanho comprimido, taxa (1 - compressed/original), tempo de compressão (opcional)
  - comparadores: gzip -k, zip, 7z

- Tabela de resultados (CSV) em docs/results.csv

- Observações:
  - Huffman ótimo para dados com distribuição de frequência muito desigual.
  - Formatos como 7z usam dicionários e modelos mais avançados (LZMA), por isso normalmente obtêm taxas melhores em arquivos maiores.
