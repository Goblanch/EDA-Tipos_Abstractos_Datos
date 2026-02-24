#pragma once
#include <iostream>
#include <stdexcept>

template<typename T>
class GenericStack {
public:
    // Constructor por defecto
    GenericStack() : topNode_(nullptr), size_(0) {}

    // Constructor de copia: útil para hacer modificaciones
    // dentro de la pila (eliminar ciertos datos por ejemplo)
    GenericStack(const GenericStack& other) {
        copy(other);
    }

    // Sobrecarga de operador =
    GenericStack& operator=(const GenericStack& other) {
        if (this != &other) {
            freeStack();
            copy(other);
        }
        return *this;
    }

    // Destructor
    ~GenericStack() {
        freeStack();
    }

    void push(const T& item) {
        topNode_ = new Node(item, topNode_);
        ++size_;
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack is emptuy");
        }

        Node* aux = topNode_;
        topNode_ = topNode_->next;
        delete aux;
        --size_;
    }

    T& top(){
        if (empty()) {
            throw std::underflow_error("Stack is empty");
        }
        return topNode_->data;
    }

    bool empty() {
        return topNode_ == nullptr;
    }

    size_t size() const {
        return size_;
    }

    void print() {
        if (empty()) {
            std::cout << "[Empty Stack]\n";
            return;
        }

        Node* current = topNode_;
        std::cout << "Cima -> ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " | ";
            }
            current = current->next;
        }
        std::cout << " <- Fondo\n";
    }

private:
    // Nodo que se almacena en la pila (genérico)
    struct Node {
        T data;
        Node* next;

        Node(const T& value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
    };

    Node* topNode_;
    size_t size_;

    void freeStack() {
        while (topNode_ != nullptr) {
            Node* aux = topNode_;
            topNode_ = topNode_->next;
            delete aux;
        }
        size_ = 0;
    }

    void copy(const GenericStack& other) {
        if (other.topNode_ == nullptr) {
            topNode_ = nullptr;
            size_ = 0;
            return;
        }

        // Para mantener el orden original, copiamos primero en un array temporal
        Node* current = other.topNode_;
        T* items = new T[other.size_];
        size_t i = 0;

        while (current != nullptr) {
            items[i++] = current->data;
            current = current->next;
        }

        topNode_ = nullptr;
        size_ = 0;

        // Insertamos desde el final para respetar el orden de la pila
        for (size_t j = other.size_; j > 0; --j) {
            push(items[j - 1]);
        }

        delete[] items;
    }
};