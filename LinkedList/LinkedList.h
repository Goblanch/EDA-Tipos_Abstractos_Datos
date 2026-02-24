#pragma once
#include <stdexcept>
#include <iostream>

template <typename T>
class LinkedList {
public:
    // Constructor por defecto
    LinkedList() : head_{nullptr}, tail_{nullptr}, size_{0} {}

    // Constructor de copia
    LinkedList(const LinkedList& other) : head_(nullptr), tail_(nullptr), size_ {0} {
        copy(other);
    }

    // Operador =
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            copy(other);
        }

        return *this;
    }

    // Destructor
    ~LinkedList() {
        clear();
    }

    // Comprueba si está vacío
    bool empty() const {
        return head_ == nullptr;
    }

    // Devuelve el número de elementos
    std::size_t size() const {
        return size_;
    }

    // Elimina todos los elementos
    void clear() {
        while (head_ != nullptr) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
        tail_ = nullptr;
        size_ = 0;
    }

    // Inserta un elemento al principio
    void pushFront(const T& value) {
        Node* newNode = new Node(value, head_);
        head_ = newNode;

        if (tail_ == nullptr) {
            tail_ = newNode;
        }

        ++size_;
    }

    // Inserta un elemento al final
    void pushBack(const  T& value) {
        Node* newNode = new Node(value);

        if (empty()) {
            head_ = newNode;
            tail_ = newNode;
        }else {
            tail_->next = newNode;
            tail_ = newNode;
        }

        ++size_;
    }

    //Inserta en un índice específico
    void insert(std::size_t index, const T& value) {
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

        // Buscamos el nodo en posición index - 1 (el previo)
        Node* prev = head_;
        for (size_t i = 0; i < index - 1; ++i) {
            prev = prev->next;
            // Ahora prev apunta al nodo anterior del índice donde queremos insertar
        }

        Node* newNode = new Node(value, prev->next);
        prev->next = newNode;
        ++size_;
    }

    // Elimina un elemento por primera aparición
    bool remove(const T& value) {
        if (empty()) {
            return false;
        }

        // Comprobamos si está en head
        if (head_->data == value) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
            --size_;

            if (head_ == nullptr) {
                tail_ = nullptr;
            }

            return true;
        }

        // Buscamos primera aparición. OJO buscamos quedarnos con el nodo anterior
        // por eso current->next->data != value
        Node* current = head_;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }

        // Si no se ha encontrado
        if (current->next == nullptr) {
            return false;
        }

        Node* temp = current->next;
        current->next = temp->next;

        if (temp == tail_) {
            tail_ = current;
        }

        delete temp;
        --size_;
        return true;
    }

    // Elimina un elemento en una posición específica
    void removeAt(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
            --size_;

            if (head_ == nullptr) {
                tail_ = nullptr;
            }

            return;
        }

        Node* prev = head_;
        for (size_t i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }

        Node* temp = prev->next;
        prev->next = temp->next;

        if (temp == tail_) {
            tail_ = temp;
        }

        delete temp;
        --size_;
    }

    // Accede a un elemento en la posición
    T& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }

        return current->data;
    }

    void print() const {
        if (empty()) {
            std::cout << "[Empty list]\n";
            return;
        }

        Node* current = head_;
        std::cout << "Head -> ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << " -> nullptr\n";
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

    void copy(const LinkedList& other) {
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;

        Node* current = other.head_;
        while (current != nullptr) {
            pushBack(current->data);
            current = current->next;
        }
    }
};
