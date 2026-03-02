# EDA – Tipos Abstractos de Datos en C++

## Descripción general

Este repositorio recopila implementaciones de **Tipos Abstractos de Datos (TAD)** escritas en **C++ moderno** (C++17) haciendo uso de `template` para que cada estructura sea genérica y reutilizable con cualquier tipo de dato.

El objetivo es didáctico: servir como material de estudio para la asignatura de **Estructuras de Datos y Algoritmos (EDA)** a nivel de Ingeniería Informática. Cada TAD está documentado con la explicación de su implementación real, incluyendo la gestión manual de memoria con punteros.

---

## Estructura del repositorio

```
EDA-Tipos_Abstractos_Datos/
├── CMakeLists.txt          ← Compilación conjunta con CMake
├── README.md               ← Este fichero
│
├── Common/
│   └── Node.h              ← Nodo genérico con shared_ptr (usado por los árboles)
│
├── Stack/
│   ├── GenericStack.h      ← Implementación de la pila con template
│   ├── main.cpp            ← Ejemplo de uso
│   └── README.md           ← Documentación de la pila
│
├── Queue/
│   ├── GenericQueue.h      ← Implementación de la cola con template
│   ├── main.cpp            ← Ejemplo de uso
│   └── README.md           ← Documentación de la cola
│
├── LinkedList/
│   ├── LinkedList.h        ← Implementación de la lista enlazada con template
│   ├── main.cpp            ← Ejemplo de uso
│   └── README.md           ← Documentación de la lista enlazada
│
├── DoublyLinkedList/
│   ├── DoublyLinkedList.h  ← Implementación de la lista doblemente enlazada con template
│   ├── main.cpp            ← Ejemplo de uso
│   └── README.md           ← Documentación de la lista doblemente enlazada
│
├── CircularLinkedList/
│   ├── CircularLinkedList.h ← Implementación de la lista enlazada circular con template
│   ├── main.cpp             ← Ejemplo de uso
│   └── README.md            ← Documentación de la lista enlazada circular
│
├── BinaryTree/
│   ├── BinaryTree.h        ← Implementación del árbol binario con template
│   ├── main.cpp            ← Ejemplo de uso
│   └── README.md           ← Documentación del árbol binario
│
└── BinarySearchTree/
    ├── BinarySearchTree.h  ← Implementación del árbol binario de búsqueda con template
    ├── main.cpp            ← Ejemplo de uso
    └── README.md           ← Documentación del árbol binario de búsqueda
```

---

## Compilación con CMake

Desde la raíz del repositorio:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Esto generará un ejecutable independiente por cada TAD.

---

## Ejecución

Después de compilar, los binarios se encuentran dentro de `build/`. Ejemplo:

```bash
./build/Stack
./build/Queue
./build/LinkedList
./build/BinaryTree
./build/BinarySearchTree
```

En Windows (con MSVC o MinGW) los ejecutables estarán en `build\Debug\` o `build\Release\` según la configuración.

---

## TADs implementados

| TAD | Fichero de cabecera | Política |
|-----|---------------------|----------|
| [Pila (Stack)](./Stack/) | `GenericStack.h` | LIFO |
| [Cola (Queue)](./Queue/) | `GenericQueue.h` | FIFO |
| [Lista Enlazada Simple (LinkedList)](./LinkedList/) | `LinkedList.h` | Acceso por índice |
| [Lista Doblemente Enlazada (DoublyLinkedList)](./DoublyLinkedList/) | `DoublyLinkedList.h` | Acceso por índice, recorrido bidireccional |
| [Lista Enlazada Circular (CircularLinkedList)](./CircularLinkedList/) | `CircularLinkedList.h` | Acceso por índice, ciclo cerrado |
| [Árbol Binario (BinaryTree)](./BinaryTree/) | `BinaryTree.h` | Estructura jerárquica, 4 recorridos |
| [Árbol Binario de Búsqueda (BinarySearchTree)](./BinarySearchTree/) | `BinarySearchTree.h` | Búsqueda, inserción y eliminación O(log n) |

---

## Notas

- Cada implementación incluye un fichero `main.cpp` con un ejemplo de uso ejecutable.
- Todas las estructuras gestionan la memoria de forma manual (sin `std::unique_ptr` ni contenedores de la STL internamente), con el fin de que el estudiante entienda el ciclo de vida de los nodos y los punteros.
- Se han implementado **constructor de copia** y **operador de asignación** para garantizar la copia profunda (*deep copy*) y evitar que dos objetos compartan los mismos nodos en memoria.