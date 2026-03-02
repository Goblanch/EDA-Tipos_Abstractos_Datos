#include <iostream>
#include "BinaryTree.h"

int main() {
    BinaryTree<int> tree;

    BinaryTree<int> node4(4);
    BinaryTree<int> node5(5);
    BinaryTree<int> node6(6);
    BinaryTree<int> node7(7);

    BinaryTree<int> node2;
    node2.buildTree(node4, node5, 2);

    BinaryTree<int> node3;
    node3.buildTree(node6, node7, 3);

    tree.buildTree(node2, node3, 1);

    std::cout << "Root: " << tree.getRootData() << "\n";
    std::cout << "Size: " << tree.size() << "\n";
    std::cout << "Height: " << tree.height() << "\n\n";

    std::cout << "In-order: ";
    tree.traverseInOrder();

    std::cout << "Pre-order: ";
    tree.traversePreOrder();

    std::cout << "Post-order: ";
    tree.traversePostOrder();

    std::cout << "Level-order: ";
    tree.traverseLevelOrder();

    std::cout << "\nCopy constructor test:\n";
    BinaryTree<int> copyTree(tree);
    copyTree.traverseInOrder();

    std::cout << "\nAssignment operator test:\n";
    BinaryTree<int> assignedTree;
    assignedTree = tree;
    assignedTree.traversePreOrder();

    return 0;
}