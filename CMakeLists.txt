cmake_minimum_required(VERSION 3.14)

project(tipos_abstractos_datos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Pila
add_executable(pila Stack/main.cpp)