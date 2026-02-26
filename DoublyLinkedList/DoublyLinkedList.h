#pragma once

#include <memory>
#include <stdexcept>
#include <iostream>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev; // Para evitar referencia cíclica

        Node(const T& value) : data(value), next(nullptr), prev() {}
    };

    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    std::size_t size;

    void copy(const DoublyLinkedList& other) {
        head = nullptr;
        tail = nullptr;
        size = 0;

        std::shared_ptr<Node> current = other.head;
        while (current != nullptr) {
            pushBack(current->data);
            current = current->next;
        }
    }

public:

    // Constructor por defecto
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Constructor de copia
    DoublyLinkedList(const DoublyLinkedList& other)
        : head(nullptr), tail(nullptr), size(0) {
        copy(other);
    }

    // Asignación = (Deep copy)
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            clear();
            copy(other);
        }

        return *this;
    }

    // Destructor
    ~DoublyLinkedList() {
        clear();
    }

    // Comprueba si la lista está vacía
    bool empty() const {
        return head == nullptr;
    }

    // Decuelve el tamaño de la lista
    std::size_t getSize() const {
        return size;
    }

    // Elimina todos los elementos de la lista
    void clear() {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    // Añade un elemento al principio
    void pushFront(const T& value) {
        std::shared_ptr<Node> newNode = std::make_shared<Node>(value);

        if (empty()) {
            head = tail = newNode;
        }else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }

        ++size;
    }

    // Añade un elemento al final
    void pushBack(const T& value) {
        std::shared_ptr<Node> newNode = std::make_shared<Node>(value);

        if (empty()) {
            head = tail = newNode;
        }else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }

        ++size;
    }

    // Añade un elemento en un índice especificado
    void insert(const std::size_t index, const T& value) {
        if (index > size) {
            throw std::out_of_range("index out of range");
        }

        if (index == 0) {
            pushFront(value);
            return;
        }

        if (index == size) {
            pushBack(value);
            return;
        }

        std::shared_ptr<Node> current = head;
        for (std::size_t i= 0; i < index; ++i) {
            current = current->next;
        }

        std::shared_ptr<Node> previous = current->prev.lock();
        std::shared_ptr<Node> newNode = std::make_shared<Node>(value);

        newNode->next = current;
        newNode->prev = previous;

        previous->next = newNode;
        current->prev = newNode;

        ++size;
    }

    // Elimina la primera aparición de un elemento
    bool remove(const T& value) {
        if (empty()) {
            return false;
        }

        std::shared_ptr<Node> current = head;
        while (current != nullptr && current->data != value) {
            current = current->next;
        }

        if (current == nullptr) {
            return false;
        }

        // Eliminar si está en la cabeza
        if (current == head) {
            head = head->next;
            if (head != nullptr) {
                head->prev.reset();
            }else {
                tail = nullptr;
            }
        }else if (current == tail) {
            // Eliminar la cola
            tail = tail->prev.lock();
            tail->next = nullptr;
        }else {
            // Eliminar un nodo intermedio
            std::shared_ptr<Node> previous = current->prev.lock();
            std::shared_ptr<Node> nextNode = current->next;

            previous->next = nextNode;
            nextNode->prev = previous;
        }

        --size;
        return true;
    }

    // Elimina un elemento en un índice especificado
    void removeAt(std::size_t index) {
        if (index >= size) {
            throw std::out_of_range("index out of range");
        }

        std::shared_ptr<Node> current = head;
        for (std::size_t i= 0; i < index; ++i) {
            current = current->next;
        }

        // Eliminar cabeza
        if (current == head) {
            head = head->next;

            if (head != nullptr) {
                head->prev.reset();
            }else {
                tail = nullptr;
            }
        }else if (current == tail) {
            // Eliminar cola
            tail = tail->prev.lock();
            tail->next = nullptr;
        }else {
            // Eliminar un nodo intermedio
            std::shared_ptr<Node> previous = current->prev.lock();
            std::shared_ptr<Node> nextNode = current->next;

            previous->next = nextNode;
            nextNode->prev = previous;
        }

        --size;
    }

    // Devuelve un elemento en una posición especificada
    T& at(const std::size_t index) {
        if (index >= size) {
            throw std::out_of_range("index out of range");
        }

        std::shared_ptr<Node> current;

        if (index < size / 2) {
            current = head;
            for (std::size_t i= 0; i < index; ++i) {
                current = current->next;
            }
        }else {
            current = tail;
            for (std::size_t i = size; i > index; --i) {
                current = current->prev.lock();
            }
        }

        return current->data;
    }

    void printForward() const {
        if (empty()) {
            std::cout << "[Empty list]\n";
            return;
        }

        std::shared_ptr<Node> current = head;
        std::cout << "Head -> ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " <-> ";
            }
            current = current->next;
        }
        std::cout << " <- Tail\n";
    }

    void printBackward() const {
        if (empty()) {
            std::cout << "[Empty list]\n";
            return;
        }

        std::shared_ptr<Node> current = tail;
        std::cout << "Tail -> ";
        while (current != nullptr) {
            std::cout << current->data;

            std::shared_ptr<Node> previous = current->prev.lock();
            if (previous != nullptr) {
                std::cout << " <-> ";
            }

            current = previous;
        }
        std::cout << " <- Head\n";
    }
};