#pragma once

#include <iostream>
#include <memory>

/*
    Este template de Nodo se usa en las implementaciones de los árboles.
*/

template <typename T>
class Node {
private:
    T data_;
    std::shared_ptr<Node<T>> left;
    std::shared_ptr<Node<T>> right;

public:
    Node(const T& data): data_(data), left(nullptr), right(nullptr) {}

    const T& getData() const {
        return data_;
    }

    void setData(const T& newData) {
        data_ = newData;
    }

    std::shared_ptr<Node<T>> getLeft() const {
        return left;
    }

    void setLeft(std::shared_ptr<Node<T>> newLeft) {
        left = newLeft;
    }

    std::shared_ptr<Node<T>> getRight() const {
        return right;
    }

    void setRight(std::shared_ptr<Node<T>> newRight) {
        right = newRight;
    }

    void processNode() const {
        std::cout << data_ << " ";
    }
};