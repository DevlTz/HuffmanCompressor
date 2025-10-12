#pragma once
#include <string>
#include <map>
#include <cstdint>

namespace compressor {

// Compress: se freq_table_path vazio, conta bytes do input.
// Se freq_table_path não for vazio, tenta usar a tabela (suporta tokens multi-char).
void compress_file(const std::string &inputPath,
                   const std::string &outputPath,
                   const std::string &freq_table_path = "");

// Decompress: lê arquivo compactado e escreve output
void decompress_file(const std::string &inputPath,
                     const std::string &outputPath);

} // namespace compressor
