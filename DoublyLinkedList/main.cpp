#include <iostream>
#include "DoublyLinkedList.h"

int main() {

    DoublyLinkedList<int> list;

    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    list.printForward();
    list.printBackward();

    list.pushFront(5);
    list.printForward();

    list.insert(2, 99);
    list.printForward();

    list.remove(20);
    list.printForward();

    list.removeAt(0);
    list.printForward();

    std::cout << "Element at index 1: " << list.at(1) << "\n";

    DoublyLinkedList<int> copyList(list);
    copyList.printForward();

    DoublyLinkedList<int> assignedList;
    assignedList.pushBack(1000);
    assignedList = list;
    assignedList.printForward();


    return 0;
}