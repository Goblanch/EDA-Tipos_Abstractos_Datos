#include <iostream>
#include "GenericQueue.h"

int main() {
    GenericQueue<int> q;
    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);

    std::cout << "########## COLA ORIGINAL ##########\n";
    q.print(); // Front -> 10 | 20 | 30 <- Back
    std::cout << "Elemento delante: " << q.front() << "\n";

    // Copy constructor
    GenericQueue<int> q2(q);
    std::cout << "########## COLA COPIADA ##########\n";
    q2.print();

    // Assignment operator
    GenericQueue<int> q3;
    q3.enqueue(99);
    q3 = q;
    std::cout << "########## COLA OPERADOR = ##########\n";
    q3.print();

    q.dequeue();
    std::cout << "Despues de dequeue, cola original:\n";
    q.print();

    std::cout << "Cola copiada se mantiene intacta:\n";
    q2.print();

    return 0;
}