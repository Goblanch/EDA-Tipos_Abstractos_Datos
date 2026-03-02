#include <iostream>
#include "BinarySearchTree.h"

int main() {
    BinarySearchTree<int> tree;

    tree.insert(18);
    tree.insert(5);
    tree.insert(25);
    tree.insert(1);
    tree.insert(20);
    tree.insert(46);
    tree.insert(19);
    tree.insert(22);

    std::cout << "Raíz: " << tree.getRootData() << "\n";
    std::cout << "Tamaño: " << tree.size() << "\n";
    std::cout << "Altura: " << tree.height() << "\n\n";

    std::cout << "Inorden: ";
    tree.traverseInOrder();

    std::cout << "Preorden: ";
    tree.traversePreOrder();

    std::cout << "Postorden: ";
    tree.traversePostOrder();

    std::cout << "Por niveles: ";
    tree.traverseLevelOrder();

    std::cout << "\nBuscar 20: " << (tree.contains(20) ? "sí" : "no") << "\n";
    std::cout << "Buscar 99: " << (tree.contains(99) ? "sí" : "no") << "\n";

    std::cout << "\nEliminar 25...\n";
    tree.remove(25);

    std::cout << "Inorden tras eliminar 25: ";
    tree.traverseInOrder();

    std::cout << "\nPrueba constructor de copia:\n";
    BinarySearchTree<int> copyTree(tree);
    copyTree.traverseInOrder();

    std::cout << "\nPrueba operador = :\n";
    BinarySearchTree<int> assignedTree;
    assignedTree = tree;
    assignedTree.traversePreOrder();

    return 0;
}