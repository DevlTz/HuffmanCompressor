cmake_minimum_required(VERSION 3.10)
project(huffman-compressor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(freq_counter src/freq_counter/main.cpp src/lib/utils.cpp)
add_executable(compressor src/compressor/main.cpp src/lib/huffman_tree.cpp src/lib/bit_stream.cpp src/lib/utils.cpp)

target_include_directories(freq_counter PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(compressor PRIVATE ${CMAKE_SOURCE_DIR}/include)
