#include <iostream>
#include "CircularLinkedList.h"

int main() {

    CircularLinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    list.print(); // Head -> 10 -> 20 -> 30 -> (back to Head)

    list.pushFront(5);
    list.print(); // Head -> 5 -> 10 -> 20 -> 30 -> (back to Head)

    list.insert(2, 99);
    list.print(); // Head -> 5 -> 10 -> 99 -> 20 -> 30 -> (back to Head)

    list.remove(20);
    list.print(); // Head -> 5 -> 10 -> 99 -> 30 -> (back to Head)

    list.removeAt(0);
    list.print(); // Head -> 10 -> 99 -> 30 -> (back to Head)

    std::cout << "Element at index 1: " << list.at(1) << "\n"; // 99

    // Copy constructor
    CircularLinkedList<int> copyList(list);
    copyList.print();

    // Assignment operator
    CircularLinkedList<int> assignedList;
    assignedList.pushBack(1000);
    assignedList = list;
    assignedList.print();

    return 0;
}