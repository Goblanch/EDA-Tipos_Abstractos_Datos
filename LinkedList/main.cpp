#include <iostream>

#include "LinkedList.h"

int main() {
    LinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    list.print(); // Head -> 10 -> 20 -> 30 -> nullptr

    list.pushFront(5);
    list.print(); // Head -> 5 -> 10 -> 20 -> 30 -> nullptr

    list.insert(2, 99);
    list.print(); // Head -> 5 -> 10 -> 99 -> 20 -> 30 -> nullptr

    list.remove(20);
    list.print(); // Head -> 5 -> 10 -> 99 -> 30 -> nullptr

    list.removeAt(0);
    list.print(); // Head -> 10 -> 99 -> 30 -> nullptr

    std::cout << "Element at index 1: " << list.at(1) << "\n"; // 99

    // Copy constructor
    LinkedList<int> copyList(list);
    copyList.print();

    // Assignment operator
    LinkedList<int> assignedList;
    assignedList.pushBack(1000);
    assignedList = list;
    assignedList.print();

    return 0;
}
