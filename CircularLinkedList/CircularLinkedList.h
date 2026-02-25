#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class CircularLinkedList {
public:

    // Constructor por defecto
    CircularLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    // Constructor de copia
    CircularLinkedList(const CircularLinkedList& other)
        : head_(nullptr), tail_(nullptr), size_(0) {
        copy(other);
    }

    // Operador =
    CircularLinkedList& operator=(const CircularLinkedList& other) {
        if (this != &other) {
            clear();
            copy(other);
        }

        return *this;
    }

    // Destructor
    ~CircularLinkedList() {
        clear();
    }

    // Comprueba si la lista está vacía
    bool empty() const {
        return head_ == nullptr;
    }

    // Devuelve el número de elementos
    std::size_t size() {
        return size_;
    }

    // Elimina todos los elementos
    void clear() {
        if (empty()) {
            return;
        }

        Node* current = head_;
        for (std::size_t i = 0; i < size_; ++i) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }

        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    // Insertar al principio
    void pushFront(const T& value) {
        Node* newNode = new Node(value);

        if (empty()) {
            head_ = tail_ = newNode;
            newNode->next = newNode; // Enlace circular
        }else {
            newNode->next = head_; // Enlace circular
            head_ = newNode;
            tail_->next = head_;
        }

        ++size_;
    }

    // Insertar al final
    void pushBack(const T& value) {
        Node* newNode = new Node(value);

        if (empty()) {
            head_ = tail_ = newNode;
            newNode->next = newNode; // Enlace circular
        }else {
            newNode->next = head_;
            tail_->next = newNode;
            tail_ = newNode;
        }

        ++size_;
    }

    // Insertar a en una posición específica
    void insert(size_t index, const T& value) {
        if (index > size_) {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0) {
            pushFront(value);
            return;
        }

        if (index == size_) {
            pushBack(value);
            return;
        }

        Node* current = head_;
        for (std::size_t i = 0; i < index - 1; ++i) {
            current = current->next;
        }

        Node* newNode = new Node(value, current->next);
        current->next = newNode;
        ++size_;
    }

    // Eliminar primera aparición
    bool remove(const T& value) {
        if (empty()) {
            return false;
        }

        // Eliminar head
        if (head_->data == value) {
            // Si solo existe la cabeza
            if (size_ == 1) {
                delete head_;
                head_ = nullptr;
                tail_ = nullptr;
                size_ = 0;
                return true;
            }

            Node* temp = head_;
            head_ = head_->next;
            tail_->next = head_;
            delete temp;
            --size_;
            return true;
        }

        // Buscamos el anterior al que queremos eliminar
        Node* current = head_;
        while (current->next != head_ && current->next->data != value) {
            current = current->next;
        }

        // No encontrado
        if (current->next == head_) {
            return false;
        }

        Node* temp = current->next;
        current->next = temp->next;

        // Si estamos eliminando tail, lo actualizamos
        if (temp == tail_) {
            tail_ = current;
        }

        delete temp;
        --size_;
        return true;
    }

    // Eliminar un elemento en una posición determinada
    void removeAt(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        // Eliminar cabeza
        if (index == 0) {
            if (size_ == 1) {
                delete head_;
                head_ = nullptr;
                tail_ = nullptr;
                size_ = 0;
                return;
            }

            Node* temp = head_;
            head_ = head_->next;
            tail_->next = head_;
            delete temp;
            --size_;
            return;
        }

        Node* current = head_;
        for (size_t i = 0; i < index - 1; ++i) {
            current = current->next;
        }

        Node* temp = current->next;
        current->next = temp->next;

        if (temp == tail_) {
            tail_ = current;
        }

        delete temp;
        --size_;
    }

    // Accede a un elemento en un índice especificado
    T& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = head_;
        for (std::size_t i = 0; i < index; ++i) {
            current = current->next;
        }

        return current->data;
    }

    // Imprime la lista
    void print() const {
        if (empty()) {
            std::cout << "[Empty circular list]\n";
            return;
        }

        Node* current = head_;
        std::cout << "Head -> ";
        for (size_t i = 0; i < size_; ++i) {
            std::cout << current->data << " -> ";
            current = current->next;
        }
        std::cout << "(back to Head)\n";
    }

private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
    };
    Node* head_;
    Node* tail_;
    std::size_t size_;

    void copy(const CircularLinkedList& other) {
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;

        if (other.empty()) {
            return;
        }

        Node* current = other.head_;
        for (std::size_t i = 0; i < other.size_; ++i) {
            pushBack(current->data);
            current = current->next;
        }
    }

};