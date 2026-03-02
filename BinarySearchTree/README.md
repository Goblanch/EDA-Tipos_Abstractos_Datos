# Árbol Binario de Búsqueda (BinarySearchTree) en C++ con `template`

## Descripción

Un **árbol binario de búsqueda** (ABB, o *Binary Search Tree*, BST) es un árbol binario que cumple el siguiente **invariante de orden** en cada nodo:

> Todos los valores del **subárbol izquierdo** son **menores** que el nodo actual, y todos los valores del **subárbol derecho** son **mayores o iguales**.

Esta propiedad permite realizar búsquedas, inserciones y eliminaciones en **O(log n)** en un árbol equilibrado, aprovechando que en cada paso descartamos la mitad del árbol.

Esta implementación está en `BinarySearchTree.h`, es completamente genérica con `template<typename T>`, y usa `std::shared_ptr<Node<T>>` para la gestión automática de memoria.

---

## Conceptos clave

### El invariante del ABB

Para cualquier nodo con valor `X`:

```
      X
     / \
  < X  >= X
```

Ejemplo con los valores insertados en `main.cpp` (`18, 5, 25, 1, 20, 46, 19, 22`):

```
           18
          /  \
         5    25
        /    /  \
       1    20   46
           /  \
          19   22
```

- Los nodos a la izquierda de `18` son `5`, `1` → todos < 18 ✓
- Los nodos a la derecha de `18` son `25`, `20`, `46`, `19`, `22` → todos >= 18 ✓
- Lo mismo se cumple en cada subárbol recursivamente.

---

## Estructura interna

### `Node<T>` (en `Common/Node.h`)

Los nodos son instancias de la misma clase `Node<T>` usada en `BinaryTree`, gestionados con `std::shared_ptr`:

```cpp
template <typename T>
class Node {
    T data_;
    std::shared_ptr<Node<T>> left;
    std::shared_ptr<Node<T>> right;
    // ...
};
```

### Atributos de `BinarySearchTree`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `root_` | `std::shared_ptr<Node<T>>` | Puntero a la raíz del ABB |

---

## Métodos implementados

### Públicos

| Método | Descripción |
|--------|-------------|
| `BinarySearchTree()` | Constructor por defecto. Crea un ABB vacío. |
| `BinarySearchTree(const T& data)` | Constructor con dato. Crea un ABB con un único nodo raíz. |
| `BinarySearchTree(const BinarySearchTree& other)` | Constructor de copia. Crea una copia profunda. |
| `operator=(const BinarySearchTree& other)` | Operador de asignación. Copia profunda del árbol. |
| `~BinarySearchTree()` | Destructor por defecto (memoria gestionada por `shared_ptr`). |
| `empty()` | Devuelve `true` si el ABB está vacío. |
| `getRootData()` | Devuelve el dato de la raíz. Lanza `std::underflow_error` si está vacío. |
| `insert(const T& value)` | Inserta un valor respetando el invariante del ABB. |
| `contains(const T& value)` | Devuelve `true` si el valor está en el árbol. |
| `remove(const T& value)` | Elimina un valor del árbol si existe. |
| `size()` | Devuelve el número total de nodos. |
| `height()` | Devuelve la altura del árbol. |
| `traverseInOrder()` | Imprime en inorden → **resultado siempre ordenado de menor a mayor**. |
| `traversePreOrder()` | Imprime en preorden (raíz – izq – der). |
| `traversePostOrder()` | Imprime en postorden (izq – der – raíz). |
| `traverseLevelOrder()` | Imprime por niveles usando una cola. |

### Privados

| Método | Descripción |
|--------|-------------|
| `clone(node)` | `static`. Copia profunda de un subárbol. Usado en constructor de copia y `operator=`. |
| `insertRec(node, value)` | Inserción recursiva respetando el orden del ABB. |
| `searchRec(node, value)` | Búsqueda recursiva binaria en el árbol. |
| `findMax(node)` | Devuelve el valor máximo de un subárbol (el nodo más a la derecha). |
| `removeRec(node, value)` | Eliminación recursiva con los 4 casos posibles. |
| `inOrder(node)` | Implementación recursiva del recorrido en inorden. |
| `preOrder(node)` | Implementación recursiva del recorrido en preorden. |
| `postOrder(node)` | Implementación recursiva del recorrido en postorden. |
| `sizeRec(node)` | Cuenta recursivamente los nodos del subárbol. |
| `heightRec(node)` | Calcula recursivamente la altura del subárbol. |

---

## Inserción – `insertRec(node, value)`

La inserción aprovecha el invariante del ABB para encontrar el lugar correcto sin comparar con todos los nodos.

```cpp
std::shared_ptr<Node<T>> insertRec(std::shared_ptr<Node<T>> node, const T& value) {
    if (node == nullptr) {
        return std::make_shared<Node<T>>(value);  // Caso base: posición encontrada
    }

    if (value < node->getData()) {
        node->setLeft(insertRec(node->getLeft(), value));   // Ir a la izquierda
    } else {
        node->setRight(insertRec(node->getRight(), value)); // Ir a la derecha
    }

    return node;  // Devuelve el nodo actual (sin cambios si no es el lugar de inserción)
}
```

### Traza de `insert(19)` sobre el árbol con raíz `18`

```
insertRec(18, 19)  → 19 >= 18, ir a la derecha
  insertRec(25, 19)  → 19 < 25, ir a la izquierda
    insertRec(20, 19)  → 19 < 20, ir a la izquierda
      insertRec(nullptr, 19)  → ¡aquí se crea el nodo!
      devuelve nuevo Node(19)
    20->setLeft(Node(19))
    devuelve 20
  25->setLeft(20)
  devuelve 25
18->setRight(25)
devuelve 18
```

Resultado:

```
           18
          /  \
         5    25
        /    /  \
       1    20   46
           /
          19     ← recién insertado
```

---

## Búsqueda – `searchRec(node, value)`

La búsqueda binaria en el ABB sigue la misma lógica que la búsqueda binaria en un array ordenado: en cada paso descartamos la mitad del árbol.

```cpp
std::shared_ptr<Node<T>> searchRec(const std::shared_ptr<Node<T>>& node, const T& value) const {
    if (node == nullptr)           return nullptr;  // No encontrado
    if (value == node->getData())  return node;     // Encontrado

    if (value < node->getData()) {
        return searchRec(node->getLeft(), value);   // Buscar a la izquierda
    }
    return searchRec(node->getRight(), value);      // Buscar a la derecha
}
```

### Traza de `contains(19)` (árbol con raíz `18`)

```
searchRec(18, 19)  → 19 != 18, 19 > 18 → ir a la derecha
  searchRec(25, 19)  → 19 != 25, 19 < 25 → ir a la izquierda
    searchRec(20, 19)  → 19 != 20, 19 < 20 → ir a la izquierda
      searchRec(19, 19)  → 19 == 19 → ¡encontrado!
      devuelve Node(19)  ← distinto de nullptr, así que contains devuelve true
```

**Sin el invariante del ABB** habría que comparar con todos los nodos (O(n)). **Con el invariante**, en cada paso se descarta la mitad restante: O(log n) en un árbol equilibrado.

---

## Eliminación – `removeRec(node, value)`

La eliminación es la operación más compleja porque hay que mantener el invariante del ABB después de borrar. Existen **4 casos**:

```cpp
std::shared_ptr<Node<T>> removeRec(std::shared_ptr<Node<T>> node, const T& value) {
    if (node == nullptr) return nullptr;  // Caso 1: no encontrado

    if (value < node->getData()) {
        node->setLeft(removeRec(node->getLeft(), value));
    } else if (value > node->getData()) {
        node->setRight(removeRec(node->getRight(), value));
    } else {
        // value == node->getData() → nodo encontrado, eliminar

        // Caso 2: nodo hoja o solo hijo derecho
        if (node->getLeft() == nullptr) return node->getRight();

        // Caso 3: solo hijo izquierdo
        if (node->getRight() == nullptr) return node->getLeft();

        // Caso 4: dos hijos
        T predecessorValue = findMax(node->getLeft());
        node->setData(predecessorValue);
        node->setLeft(removeRec(node->getLeft(), predecessorValue));
    }

    return node;
}
```

### Caso 1: valor no encontrado

Si llegamos a `nullptr` sin encontrar el valor, simplemente devolvemos `nullptr`. El árbol no se modifica.

### Caso 2: nodo hoja (sin hijos) o con solo hijo derecho

```
Antes:        25           Eliminar 25 (solo hijo derecho sería 46, sin hijo izquierdo)
             /  \
           20   46

Caso sin hijo izquierdo: devolvemos node->getRight() (46)
Después:      18
             /  \
            5    46   ← 25 sustituido por su hijo derecho 46
```

Devolver `node->getRight()` hace que el padre de `25` (en este caso `18`) apunte ahora a `46`, desconectando `25`.

### Caso 3: solo hijo izquierdo

Simétricamente, si el nodo a eliminar no tiene hijo derecho, se sustituye por su hijo izquierdo.

### Caso 4: dos hijos (el más complejo)

No podemos simplemente borrar el nodo porque tiene dos subárboles. La solución es **sustituir el valor del nodo por el de su predecesor en inorden** (el mayor valor del subárbol izquierdo) y luego eliminar ese predecesor.

```
Antes de eliminar 25:
           18
          /  \
         5    25          ← eliminar este
        /    /  \
       1    20   46
           /  \
          19   22
```

**Paso 1**: Encontrar el máximo del subárbol izquierdo de `25`:
- `findMax(node->getLeft())` recorre siempre hacia la derecha hasta el nodo más a la derecha.
- El subárbol izquierdo de `25` tiene raíz `20`, y su máximo es `22`.

```
findMax(20) → 20->getRight() = 22 → 22->getRight() = nullptr → devuelve 22
```

**Paso 2**: Reemplazar el dato de `25` por `22`:

```
           18
          /  \
         5    22          ← 25 sustituido por 22 (predecesor)
        /    /  \
       1    20   46
           /  \
          19   22         ← este 22 hay que eliminarlo ahora
```

**Paso 3**: Eliminar el `22` original del subárbol izquierdo:

```
removeRec(20, 22) → 22 >= 20, ir a la derecha
  removeRec(22, 22) → encontrado, es hoja → devuelve nullptr
20->setRight(nullptr)
```

**Resultado final**:

```
           18
          /  \
         5    22
        /    /  \
       1    20   46
           /
          19
```

El invariante sigue cumpliéndose: todos los nodos a la izquierda de `22` (nuevo nodo donde estaba `25`) son menores que `22`, y `46` es mayor.

### ¿Por qué el predecesor en inorden?

El **predecesor en inorden** es el mayor valor que es menor que el nodo actual. Usarlo garantiza que:
- Es mayor que todos los elementos del subárbol izquierdo (puede ocupar la posición del nodo eliminado).
- Es menor que todos los elementos del subárbol derecho (mantiene el invariante).

---

## `findMax(node)` – encontrar el máximo de un subárbol

En un ABB, el máximo siempre está en el **nodo más a la derecha** (no tiene hijo derecho):

```cpp
T findMax(const std::shared_ptr<Node<T>>& node) const {
    std::shared_ptr<Node<T>> current = node;
    while (current->getRight() != nullptr) {
        current = current->getRight();
    }
    return current->getData();
}
```

```
findMax(subárbol de 20):
  current = 20 → getRight() = 22 → avanzar
  current = 22 → getRight() = nullptr → parar
  devuelve 22
```

---

## Recorridos del ABB

Los recorridos son idénticos a los del árbol binario genérico. Sin embargo, en el ABB el **recorrido en inorden tiene una propiedad especial fundamental**:

> En un ABB, el recorrido **inorden siempre produce los valores ordenados de menor a mayor**.

```
Árbol:         18
              /  \
             5    25
            /    /  \
           1    20   46
               /  \
              19   22

Inorden:  1  5  18  19  20  22  25  46   ← ordenado de menor a mayor ✓
```

Esto se debe directamente al invariante: cuando procesamos un nodo, ya habremos procesado todos los del subárbol izquierdo (todos menores), y después procesaremos todos los del derecho (todos mayores o iguales).

---

## Copia profunda

### `clone(node)` – estático

```cpp
static std::shared_ptr<Node<T>> clone(const std::shared_ptr<Node<T>>& node) {
    if (node == nullptr) return nullptr;

    std::shared_ptr<Node<T>> newNode = std::make_shared<Node<T>>(node->getData());
    newNode->setLeft(clone(node->getLeft()));
    newNode->setRight(clone(node->getRight()));
    return newNode;
}
```

Al ser `static`, no necesita acceder a ningún atributo del objeto actual y puede llamarse sin instancia. El resultado es un árbol completamente nuevo en memoria, con los mismos valores y la misma estructura.

### Constructor de copia y `operator=`

```cpp
BinarySearchTree<int> copyTree(tree);     // Constructor de copia: clona tree
BinarySearchTree<int> assignedTree;
assignedTree = tree;                      // Operador de asignación: clona tree
```

Modificar `copyTree` o `assignedTree` no afecta a `tree`, porque cada objeto tiene sus propios nodos en memoria.

---

## Compilación y ejecución

Desde la raíz del repositorio:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./BinarySearchTree
```

---

## Ejemplo de salida esperada

```
Raíz: 18
Tamaño: 8
Altura: 4

Inorden: 1 5 18 19 20 22 25 46
Preorden: 18 5 1 25 20 19 22 46
Postorden: 1 5 19 22 20 46 25 18
Por niveles: 18 5 25 1 20 46 19 22

Buscar 20: sí
Buscar 99: no

Eliminar 25...
Inorden tras eliminar 25: 1 5 18 19 20 22 46

Prueba constructor de copia:
1 5 18 19 20 22 46

Prueba operador = :
18 5 1 22 20 19 46
```

---

## Complejidad algorítmica

| Operación | Árbol equilibrado | Árbol degenerado (peor caso) |
|-----------|:-----------------:|:----------------------------:|
| `insert`  | O(log n)          | O(n)                         |
| `contains`| O(log n)          | O(n)                         |
| `remove`  | O(log n)          | O(n)                         |
| `size`    | O(n)              | O(n)                         |
| `height`  | O(n)              | O(n)                         |

> Un árbol **degenerado** ocurre cuando se insertan los valores siempre en orden estrictamente creciente (o decreciente), haciendo que cada nodo tenga solo un hijo y el árbol se comporte como una lista enlazada.

---

## Notas

- El destructor es `= default` porque `std::shared_ptr` gestiona la memoria automáticamente.
- `getRootData()` lanza `std::underflow_error` si el árbol está vacío.
- `findMax` lanza `std::underflow_error` si se llama con un subárbol vacío; en la práctica solo se llama desde `removeRec` cuando ya se ha comprobado que hay hijo izquierdo.
- El inorden produce siempre los valores ordenados, lo que convierte el ABB en una manera eficiente de mantener un conjunto ordenado dinámico.
