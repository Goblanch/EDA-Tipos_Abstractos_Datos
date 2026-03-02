#pragma once

#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include "Node.h"

template <typename T>
class BinarySearchTree {
private:
    // Puntero a la raíz del árbol
    std::shared_ptr<Node<T>> root_;

    /*
        clone(node)

        Hace una copia profunda del subárbol cuya raíz es 'node'.

        Se usa en:
        - constructor de copia
        - operador de asignación

        Al ser static, no depende del objeto actual.
    */
    static std::shared_ptr<Node<T>> clone(const std::shared_ptr<Node<T>>& node) {
        if (node == nullptr) {
            return nullptr;
        }

        std::shared_ptr<Node<T>> newNode = std::make_shared<Node<T>>(node->getData());
        newNode->setLeft(clone(node->getLeft()));
        newNode->setRight(clone(node->getRight()));
        return newNode;
    }

    /*
        insertRec(node, value)

        Inserta recursivamente un valor en el ABB y devuelve
        la nueva raíz del subárbol.

        Regla del ABB:
        - si value < dato actual, se inserta a la izquierda
        - si value >= dato actual, se inserta a la derecha
    */
    std::shared_ptr<Node<T>> insertRec(std::shared_ptr<Node<T>> node, const T& value) {
        if (node == nullptr) {
            return std::make_shared<Node<T>>(value);
        }

        if (value < node->getData()) {
            node->setLeft(insertRec(node->getLeft(), value));
        } else {
            node->setRight(insertRec(node->getRight(), value));
        }

        return node;
    }

    /*
        searchRec(node, value)

        Busca recursivamente un valor en el ABB.

        Aprovecha la propiedad de orden:
        - si value == dato actual, lo hemos encontrado
        - si value < dato actual, buscamos a la izquierda
        - si value > dato actual, buscamos a la derecha

        Devuelve el nodo encontrado o nullptr si no existe.
    */
    std::shared_ptr<Node<T>> searchRec(const std::shared_ptr<Node<T>>& node, const T& value) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (value == node->getData()) {
            return node;
        }

        if (value < node->getData()) {
            return searchRec(node->getLeft(), value);
        }

        return searchRec(node->getRight(), value);
    }

    /*
        findMax(node)

        Devuelve el valor máximo de un subárbol.

        En un ABB, el máximo siempre está en el nodo
        más a la derecha.
    */
    T findMax(const std::shared_ptr<Node<T>>& node) const {
        if (node == nullptr) {
            throw std::underflow_error("Subárbol vacío");
        }

        std::shared_ptr<Node<T>> current = node;
        while (current->getRight() != nullptr) {
            current = current->getRight();
        }

        return current->getData();
    }

    /*
        removeRec(node, value)

        Elimina recursivamente un valor del ABB y devuelve
        la nueva raíz del subárbol.

        Casos:
        1. Nodo no encontrado -> no se modifica nada
        2. Nodo hoja -> desaparece
        3. Nodo con un hijo -> se sustituye por su hijo
        4. Nodo con dos hijos -> se sustituye por el mayor
           de los menores (máximo del subárbol izquierdo)
    */
    std::shared_ptr<Node<T>> removeRec(std::shared_ptr<Node<T>> node, const T& value) {
        if (node == nullptr) {
            return nullptr;
        }

        if (value < node->getData()) {
            node->setLeft(removeRec(node->getLeft(), value));
        } else if (value > node->getData()) {
            node->setRight(removeRec(node->getRight(), value));
        } else {
            // Caso: no tiene hijo izquierdo
            if (node->getLeft() == nullptr) {
                return node->getRight();
            }

            // Caso: no tiene hijo derecho
            if (node->getRight() == nullptr) {
                return node->getLeft();
            }

            // Caso: tiene dos hijos
            T predecessorValue = findMax(node->getLeft());
            node->setData(predecessorValue);
            node->setLeft(removeRec(node->getLeft(), predecessorValue));
        }

        return node;
    }

    /*
        inOrder(node)

        Recorre el árbol en inorden:
        izquierda -> raíz -> derecha

        En un ABB, este recorrido imprime los valores
        ordenados de menor a mayor.
    */
    void inOrder(const std::shared_ptr<Node<T>>& node) const {
        if (node != nullptr) {
            inOrder(node->getLeft());
            node->processNode();
            inOrder(node->getRight());
        }
    }

    /*
        preOrder(node)

        Recorre el árbol en preorden:
        raíz -> izquierda -> derecha
    */
    void preOrder(const std::shared_ptr<Node<T>>& node) const {
        if (node != nullptr) {
            node->processNode();
            preOrder(node->getLeft());
            preOrder(node->getRight());
        }
    }

    /*
        postOrder(node)

        Recorre el árbol en postorden:
        izquierda -> derecha -> raíz
    */
    void postOrder(const std::shared_ptr<Node<T>>& node) const {
        if (node != nullptr) {
            postOrder(node->getLeft());
            postOrder(node->getRight());
            node->processNode();
        }
    }

    /*
        sizeRec(node)

        Devuelve el número de nodos del subárbol cuya raíz es 'node'.
    */
    std::size_t sizeRec(const std::shared_ptr<Node<T>>& node) const {
        if (node == nullptr) {
            return 0;
        }

        return 1 + sizeRec(node->getLeft()) + sizeRec(node->getRight());
    }

    /*
        heightRec(node)

        Devuelve la altura del subárbol cuya raíz es 'node'.

        Convención:
        - árbol vacío -> altura 0
        - árbol con un único nodo -> altura 1
    */
    std::size_t heightRec(const std::shared_ptr<Node<T>>& node) const {
        if (node == nullptr) {
            return 0;
        }

        std::size_t leftHeight = heightRec(node->getLeft());
        std::size_t rightHeight = heightRec(node->getRight());

        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

public:
    /*
        Constructor por defecto.

        Crea un ABB vacío.
    */
    BinarySearchTree() : root_(nullptr) {}

    /*
        Constructor con dato.

        Crea un ABB con una única raíz.
    */
    BinarySearchTree(const T& data) : root_(std::make_shared<Node<T>>(data)) {}

    /*
        Constructor de copia.

        Hace una copia profunda del árbol.
    */
    BinarySearchTree(const BinarySearchTree& other) : root_(clone(other.root_)) {}

    /*
        Operador de asignación.

        Sustituye el contenido actual por una copia profunda del otro árbol.
    */
    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            root_ = clone(other.root_);
        }
        return *this;
    }

    /*
        Destructor.

        No es necesario liberar memoria manualmente porque usamos shared_ptr.
    */
    ~BinarySearchTree() = default;

    /*
        empty()

        Devuelve true si el árbol está vacío.
    */
    bool empty() const {
        return root_ == nullptr;
    }

    /*
        getRootData()

        Devuelve el dato de la raíz.
        Lanza excepción si el árbol está vacío.
    */
    const T& getRootData() const {
        if (empty()) {
            throw std::underflow_error("El árbol está vacío");
        }

        return root_->getData();
    }

    /*
        insert(value)

        Inserta un valor en el ABB.
    */
    void insert(const T& value) {
        root_ = insertRec(root_, value);
    }

    /*
        contains(value)

        Devuelve true si el valor está en el árbol.
    */
    bool contains(const T& value) const {
        return searchRec(root_, value) != nullptr;
    }

    /*
        remove(value)

        Elimina un valor del árbol si existe.
    */
    void remove(const T& value) {
        root_ = removeRec(root_, value);
    }

    /*
        size()

        Devuelve el número total de nodos del árbol.
    */
    std::size_t size() const {
        return sizeRec(root_);
    }

    /*
        height()

        Devuelve la altura total del árbol.
    */
    std::size_t height() const {
        return heightRec(root_);
    }

    /*
        traverseInOrder()

        Imprime el recorrido en inorden.
        En un ABB, este recorrido sale ordenado.
    */
    void traverseInOrder() const {
        inOrder(root_);
        std::cout << "\n";
    }

    /*
        traversePreOrder()

        Imprime el recorrido en preorden.
    */
    void traversePreOrder() const {
        preOrder(root_);
        std::cout << "\n";
    }

    /*
        traversePostOrder()

        Imprime el recorrido en postorden.
    */
    void traversePostOrder() const {
        postOrder(root_);
        std::cout << "\n";
    }

    /*
        traverseLevelOrder()

        Recorre el árbol por niveles usando una cola.
    */
    void traverseLevelOrder() const {
        if (empty()) {
            std::cout << "[Árbol vacío]\n";
            return;
        }

        std::queue<std::shared_ptr<Node<T>>> q;
        q.push(root_);

        while (!q.empty()) {
            std::shared_ptr<Node<T>> current = q.front();
            q.pop();

            current->processNode();

            if (current->getLeft() != nullptr) {
                q.push(current->getLeft());
            }

            if (current->getRight() != nullptr) {
                q.push(current->getRight());
            }
        }

        std::cout << "\n";
    }
};