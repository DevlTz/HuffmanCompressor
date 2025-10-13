#!/usr/bin/env bash
# scripts/compare_compressors.sh
# Usage: scripts/compare_compressors.sh file1 [file2 ...]
set -e
OUT=docs/results.csv
echo "file,original,ours,gzip,zip,7z,ours_ratio,gzip_ratio,zip_ratio,7z_ratio" > $OUT

for f in "$@"; do
  if [ ! -f "$f" ]; then echo "Skipping $f (not a file)"; continue; fi
  orig=$(stat -c%s "$f")
  ./compressor.exe -c -i "$f" -o temp.ours.huf
  ours=$(stat -c%s temp.ours.huf)
  gzip -k -f "$f"
  gzipf="$f.gz"
  gzip_size=$(stat -c%s "$gzipf")
  zip -j temp.zip "$f"
  zip_size=$(stat -c%s temp.zip)
  7z a -bd -y temp.7z "$f" >/dev/null 2>&1
  seven_size=$(stat -c%s temp.7z)
  ours_ratio=$(awk "BEGIN{print 1 - ($ours/$orig)}")
  gzip_ratio=$(awk "BEGIN{print 1 - ($gzip_size/$orig)}")
  zip_ratio=$(awk "BEGIN{print 1 - ($zip_size/$orig)}")
  seven_ratio=$(awk "BEGIN{print 1 - ($seven_size/$orig)}")
  echo "$(basename "$f"),$orig,$ours,$gzip_size,$zip_size,$seven_size,$ours_ratio,$gzip_ratio,$zip_ratio,$seven_ratio" >> $OUT
  rm -f temp.ours.huf temp.zip temp.7z "$gzipf"
done

echo "Results in $OUT"
