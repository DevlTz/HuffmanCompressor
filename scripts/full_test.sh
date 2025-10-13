#!/usr/bin/env bash
# scripts/full_test.sh
# Usage: scripts/full_test.sh file1 [file2 ...]


FREQ_DIR="examples/tabelas_freq"
COMPRESSED_DIR="examples/huff_generate"
DECOMPRESSED_DIR="examples/huff_generate/decompressed_code"

# Cria os diretórios se eles não existirem
mkdir -p "$FREQ_DIR"
mkdir -p "$COMPRESSED_DIR"
mkdir -p "$DECOMPRESSED_DIR"


for f in "$@"; do
  if [ ! -f "$f" ]; then
    echo "AVISO: Ignorando '$f' (não é um arquivo)"
    continue
  fi

  echo "Processando arquivo: $f"

  BASENAME=$(basename "$f")
  FREQ_FILE="${FREQ_DIR}/${BASENAME}.freq"
  COMPRESSED_FILE="${COMPRESSED_DIR}/${BASENAME}.huf"
  DECOMPRESSED_FILE="${DECOMPRESSED_DIR}/${BASENAME}"

  # --- ETAPA 1: Gerar Tabela de Frequência ---
  echo "  -> Gerando tabela de frequência..."
  ./contador_frequencia.exe "$f" -o "$FREQ_FILE"

  # --- ETAPA 2: Comprimir o Arquivo ---
  echo "  -> Comprimindo arquivo..."
  ./compressor.exe -c -f "$FREQ_FILE" -i "$f" -o "$COMPRESSED_FILE"

  # --- ETAPA 3: Descomprimir o Arquivo ---
  echo "  -> Descomprimindo arquivo..."
  ./compressor.exe -d -i "$COMPRESSED_FILE" -o "$DECOMPRESSED_FILE"

  # --- ETAPA 4: Verificar a Integridade (Teste Round-Trip) ---
  echo "  -> Verificando integridade..."
  if diff -q "$f" "$DECOMPRESSED_FILE"; then
    echo "  [SUCESSO] O arquivo restaurado é idêntico ao original."
  else
    echo "  [FALHA] O arquivo restaurado é DIFERENTE do original!"
  fi
  
done

echo "Frequency result files in: $FREQ_DIR"
echo "Compressed files in: $COMPRESSED_DIR"
echo "Restaured/Decompressed files in: $DECOMPRESSED_DIR"