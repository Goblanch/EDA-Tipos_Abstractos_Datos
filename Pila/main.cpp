#include <iostream>
#include "GenericStack.h"

// EJEMPLO DE USO DE PILA

int main() {

    // Creamos una pila de enteros
    GenericStack<int> stack;

    // Añadimos unos datos
    stack.push(1);
    stack.push(10);
    stack.push(5);
    stack.push(67);

    std::cout << "########## PILA ORIGINAL ##########\n\n";
    stack.print();

    // Constructor de copia
    GenericStack<int> stack2(stack);
    std::cout << "########## PILA CONSTRUCTOR DE COPIA ##########\n\n";
    stack2.print();

    // Operador =
    GenericStack<int> stack3;
    stack3 = stack;
    std::cout << "########## PILA OPERADOR = ##########\n\n";
    stack3.print();

    std::cout << "\nEJECUTADA SENTENCIA POP SOBRE PRIMERA PILA\n\n";
    stack.pop();
    std::cout << "Nueva cima original tras pop: " << stack.top() << std::endl;
    std::cout << "Cima copia (independiente): " << stack2.top() << std::endl;

    return 0;
}