#!/usr/bin/env bash
# scripts/full_test.sh
# Usage: scripts/full_test.sh file1 [file2 ...]

FREQ_DIR="examples/tabelas_freq"
COMPRESSED_DIR="examples/huff_generate"
DECOMPRESSED_DIR="examples/huff_generate/decompressed_code"

mkdir -p "$FREQ_DIR"
mkdir -p "$COMPRESSED_DIR"
mkdir -p "$DECOMPRESSED_DIR"

for f in "$@"; do
  if [ ! -f "$f" ]; then
    echo "WARNING: Skipping '$f' (not a file)"
    continue
  fi

  echo "Processing file: $f"

  BASENAME=$(basename "$f")
  FREQ_FILE="${FREQ_DIR}/${BASENAME}.freq"
  COMPRESSED_FILE="${COMPRESSED_DIR}/${BASENAME}.huf"
  DECOMPRESSED_FILE="${DECOMPRESSED_DIR}/${BASENAME}"

  echo "  -> Generating frequency table"
  ./contador_frequencia.exe "$f" -o "$FREQ_FILE"

  echo "  -> Compressing file"
  ./compressor.exe -c -f "$FREQ_FILE" -i "$f" -o "$COMPRESSED_FILE"

  echo "  -> Decompressing file"
  ./compressor.exe -d -i "$COMPRESSED_FILE" -o "$DECOMPRESSED_FILE"

  echo "  -> Verifying integrity (Roundy-Trip Test)"

  if diff -q "$f" "$DECOMPRESSED_FILE"; then
    echo "  [SUCCESS] Restored file is identical to the original"
  else
    echo "  [FAILURE] Restored file is DIFFERENT from the original"
  fi

done

echo "Frequency result files in: $FREQ_DIR"
echo "Compressed files in: $COMPRESSED_DIR"
echo "Restored/Decompressed files in: $DECOMPRESSED_DIR"


