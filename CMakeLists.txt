# Versão mínima do CMake requerida
cmake_minimum_required(VERSION 3.10)

# Nome do projeto e linguagem utilizada
project(AmazonLogistics VERSION 1.0 LANGUAGES CXX)

# Define o padrão do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 1. Avisa o compilador para procurar os ficheiros .hpp na pasta "include"
include_directories(include)

# 2. Aponta o caminho correto do main.cpp (que está dentro de "src")
add_executable(amazon_logistics src/main.cpp)

# Opcional: Adiciona flags de compilação
target_compile_options(amazon_logistics PRIVATE -Wall -Wextra)