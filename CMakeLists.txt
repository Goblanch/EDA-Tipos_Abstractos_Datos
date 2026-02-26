cmake_minimum_required(VERSION 3.14)

project(tipos_abstractos_datos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Stack
add_executable(Stack Stack/main.cpp)

# Queue
add_executable(Queue Queue/main.cpp)

# Linkedlist
add_executable(LinkedList LinkedList/main.cpp)

# CircularLinkedList
add_executable(CircularLinkedList CircularLinkedList/main.cpp)

# DoublyLinkedList
add_executable(DoublyLinkedList DoublyLinkedList/main.cpp)