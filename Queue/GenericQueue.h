#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class GenericQueue {
public:
    // Constructor por defecto
    GenericQueue() : front_(nullptr), back_(nullptr), size_(0) {}

    // Constructor de copia
    GenericQueue(GenericQueue& other) : front_(nullptr), back_(nullptr), size_(0) {
        copy(other);
    }

    // Operador =
    GenericQueue& operator=(GenericQueue& other) {
        if (this != &other) {
            clear();
            copy(other);
        }

        return *this;
    }

    // Destructor
    ~GenericQueue() {
        clear();
    }

    // Añadir elemento al final
    void enqueue(const T& value) {
        Node* newNode = new Node(value);

        if (empty()) {
            front_ = back_ = newNode;
        }else {
            back_->next = newNode;
            back_ = newNode;
        }

        ++size_;
    }

    // Eliminar un elemento del principio
    void dequeue() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }

        Node* temp = front_;
        front_ = front_->next;
        delete temp;
        --size_;

        if (front_ == nullptr) {
            back_ = nullptr;
        }
    }

    // Acceder al elemento de delante
    T& front() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        return front_->data;
    }

    // Comprueba si la cola está vacía
    bool empty() {
        return front_ == nullptr;
    }

    // Devuelve el tamaño
    std::size_t size() const {
        return size_;
    }

    void print() {
        if (empty()) {
            std::cout << "[Empty Queue]\n";
            return;
        }

        Node* current = front_;
        std::cout << "Front -> ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " | ";
            }
            current = current->next;
        }
        std::cout << " <- Back\n";
    }

private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
    };

    Node* front_;
    Node* back_;
    std::size_t size_;

    void clear() {
        while (front_ != nullptr) {
            Node* temp = front_;
            front_ = front_->next;
            delete temp;
        }
        back_ = nullptr;
        size_ = 0;
    }

    void copy(GenericQueue& other) {
        front_ = nullptr;
        back_ = nullptr;
        size_ = 0;

        Node* current = other.front_;
        while (current != nullptr) {
            enqueue(current->data);
            current = current->next;
        }
    }
};
