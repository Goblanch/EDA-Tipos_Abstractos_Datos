#pragma once

#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include "Node.h"

template <typename T>
class BinaryTree {
private:
    std::shared_ptr<Node<T>> root;

    /// Crea una copia profunda del subarbol con root node.
    /// Si el node es nulo, devuelve un nullptr, en otro caso, se hace una llamada
    /// recursiva por cada subárbol (izquierdo y derecho) para obtener todas las
    /// referencias de los nodos de los subárboles.
    /// @param node raíz del subarbol a copiar
    /// @return
    std::shared_ptr<Node<T>> clone(const std::shared_ptr<Node<T>>& node) const {
        if (node == nullptr) {
            return nullptr;
        }

        std::shared_ptr<Node<T>> newNode = std::make_shared<Node<T>>(node->getData());
        newNode->setLeft(clone(node->getLeft()));
        newNode->setRight(clone(node->getRight()));
        return newNode;
    }

    /// Recorrido en orden (izquierda - raíz - derecha) del subárbol con raíz
    /// en node.
    /// Visita el subárbol izquierdo, procesa el nodo actual y luego,
    /// el derecho
    /// @param node subárbol con raíz en node
    void inOrder(const std::shared_ptr<Node<T>>& node) const {
        if (node != nullptr) {
            inOrder(node->getLeft());
            node->proccessNode();
            inOrder(node->getRight());
        }
    }

    /// Recorre en orden (Raíz - izquierda - derecha) el subárbol con raíz
    /// en node.
    /// Procesa el nodo actual, visita el árbol izquierdo y luego el derecho.
    /// @param node subárbol con raíz en node
    void preOrder(const std::shared_ptr<Node<T>>& node) const {
        if (node != nullptr) {
            node->proccessNode();
            preOrder(node->getLeft());
            preOrder(node->getRight());
        }
    }

    /// Recorre el árbol en orden (izquierda - derecha - raíz) el subárbol con raíz
    /// en node.
    /// Visita el subárbol izquierdo, luego el derecho y finalmente la raíz.
    /// @param node subárbol con raíz en node
    void postOrder(const std::shared_ptr<Node<T>>& node) const {
        if (node != nullptr) {
            postOrder(node->getLeft());
            postOrder(node->getRight());
            node->proccessNode();
        }
    }

    /// Devuelve el número de nodos del subárbol marcado como node.
    /// @param node subárbol con raíz en node.
    /// @return tamaño del subárbol
    std::size_t size_(const std::shared_ptr<Node<T>>& node) const {
        if (node == nullptr) {
            return 0;
        }
        // 1 de la raíz.
        return 1 + size_(node->getLeft()) + size_(node->getRight());
    }

    /// Devuelve la altura del subárbol marcado como node.
    /// height(node) = 1 + (max(height(left), height(right))
    /// @param node tamaño del subárbol
    /// @return altura del subárbol
    std::size_t height_(const std::shared_ptr<Node<T>>& node) const {
        if (node == nullptr) {
            return 0;
        }

        std::size_t leftHeight = height_(node->getLeft());
        std::size_t rightHeight = height_(node->getRight());

        // Suma 1 de la raíz + la mayor altura entre cada lado del árbol.
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

public:
    // Constructor por defecto
    BinaryTree() : root(nullptr) {}

    // Constructor extendido: crea un árbol con un solo nodo con
    // la información de data
    BinaryTree(const T& data) : root(std::make_shared<Node<T>>(data)) {}

    // Constructor de copia
    BinaryTree(const BinaryTree& other) : root(clone(other.root)) {}

    // Sobrecarga de =
    BinaryTree& operator=(const BinaryTree& other) {
        if (this != &other) {
            root = clone(other.root);
        }
        return *this;
    }

    // Destructor
    ~BinaryTree() = default;

    // Comprueba si el árbol está vacío
    bool empty() const {
        return root == nullptr;
    }

    // Devuelve el una referencia del root data.
    const T& getRootData() const {
        if (empty()) {
            throw std::underflow_error("Tree is empty");
        }

        return root->getData();
    }

    // Devuelve un árbol binario que representa el árbol izquierdo.
    BinaryTree getLeftSubtree() const {
        if (empty()) {
            throw std::underflow_error("Tree is empty");
        }

        BinaryTree subTree;
        // Me devuelve el primero porque ese ya tiene un nodo que
        // apunta al resto del árbol y puedo recorrerlo.
        subTree.root = root.getLeft();
        return subTree;
    }

    // Devuelve un árbol binario que representa el árbol izquierdo
    BinaryTree getRightSubtree() const {
        if (empty()) {
            throw std::underflow_error("Tree is empty");
        }

        BinaryTree subTree;
        subTree.root = root.getRight();
        return subTree;
    }

    // Añade un árbol (leftTree) como subárbol izquierdo de root
    void addLedt(BinaryTree& leftTree) {
        if (empty()) {
            throw std::underflow_error("Tree is empty");
        }

        root->setLeft(leftTree.root);
    }

    // Añade un árbol (rightTree) como subárbol derecho de root
    void addRight(BinaryTree& rightTree) {
        if (empty()) {
            throw std::underflow_error("Tree is empty");
        }

        root->setRight(rightTree.root);
    }

    // Junta dos subárboles (izq y derecho) en uno con root conteniendo data
    void buildTree(const BinaryTree& leftTree, const BinaryTree& rightTree, const T& data) {
        root = std::make_shared<Node<T>>(data);
        root->setLeft(leftTree.root);
        root->setRight(rightTree.root);
    }

    // Devuelve el tamaño del árbol
    std::size_t size() const {
        return size_(root);
    }

    // Devuelve la altura del árbol
    std::size_t height() const {
        return height_(root);
    }

    // Imprime el árbol in order
    void traverseInOrder() const {
        inOrder(root);
        std::cout << std::endl;
    }

    // Imprime el árbol in pre order
    void traversePreOrder() const {
        preOrder(root);
        std::cout << std::endl;
    }

    // Imprime el árbol in post order
    void traversePostOrder() const {
        postOrder(root);
        std::cout << std::endl;
    }

    // Imprime el árbol en amplitud
    void traverseLevelOrder() const {
        if (empty()) {
            std::cout << "[Empty tree]\n";
            return;
        }

        std::queue<std::shared_ptr<Node<T>>> q;
        q.push(root);

        while (!q.empty()) {
            std::shared_ptr<Node<T>> current = q.front();
            q.pop();

            current->proccessNode();

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