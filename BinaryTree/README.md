# Árbol Binario (BinaryTree) en C++ con `template`

## Descripción

Un **árbol binario** es una estructura de datos no lineal en la que cada nodo puede tener **como máximo dos hijos**: uno izquierdo y uno derecho. A diferencia de las listas enlazadas, los elementos no forman una cadena lineal sino una estructura jerárquica en forma de árbol.

Esta implementación está en `BinaryTree.h` y es completamente genérica gracias al uso de `template<typename T>`. La gestión de memoria se delega en `std::shared_ptr`, lo que significa que **no es necesario liberar manualmente la memoria**: los nodos se destruyen solos cuando ningún `shared_ptr` los referencia.

---

## Conceptos clave

### Terminología

```
        1          ← raíz (root)
       / \
      2   3        ← nodos internos
     / \ / \
    4  5 6  7      ← hojas (nodos sin hijos)
```

| Término | Significado |
|---------|-------------|
| **Raíz** | Nodo sin padre; punto de entrada al árbol |
| **Hijo** | Nodo directamente conectado hacia abajo |
| **Padre** | Nodo directamente conectado hacia arriba |
| **Hoja** | Nodo sin hijos |
| **Altura** | Número de niveles del árbol (el árbol de arriba tiene altura 3) |
| **Tamaño** | Número total de nodos |
| **Subárbol** | Cualquier nodo junto con todos sus descendientes |

---

## Estructura interna

### `Node<T>` (en `Common/Node.h`)

Los nodos ya no son simples structs con punteros crudos como en las listas enlazadas. Aquí son instancias de una **clase completa** gestionada con `std::shared_ptr`:

```cpp
template <typename T>
class Node {
private:
    T data_;                          // Valor almacenado
    std::shared_ptr<Node<T>> left;    // Puntero al hijo izquierdo
    std::shared_ptr<Node<T>> right;   // Puntero al hijo derecho
public:
    Node(const T& data);
    const T& getData() const;
    void setData(const T& newData);
    std::shared_ptr<Node<T>> getLeft() const;
    void setLeft(std::shared_ptr<Node<T>> newLeft);
    std::shared_ptr<Node<T>> getRight() const;
    void setRight(std::shared_ptr<Node<T>> newRight);
    void processNode() const;         // Imprime data_
};
```

### ¿Por qué `shared_ptr` en lugar de punteros crudos?

En las listas enlazadas anteriores usábamos `Node*` y teníamos que llamar a `delete` manualmente. Aquí se usa `std::shared_ptr<Node<T>>`, un **puntero inteligente** que lleva internamente un contador de referencias. Cuando el contador llega a 0 (nadie apunta al nodo), la memoria se libera automáticamente. Por eso el destructor de `BinaryTree` es simplemente `= default`.

### Atributos de `BinaryTree`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `root` | `std::shared_ptr<Node<T>>` | Puntero a la raíz del árbol |

---

## Métodos implementados

### Públicos

| Método | Descripción |
|--------|-------------|
| `BinaryTree()` | Constructor por defecto. Crea un árbol vacío (`root = nullptr`). |
| `BinaryTree(const T& data)` | Constructor con dato. Crea un árbol con un único nodo como raíz. |
| `BinaryTree(const BinaryTree& other)` | Constructor de copia. Crea una copia profunda del árbol. |
| `operator=(const BinaryTree& other)` | Operador de asignación. Copia profunda del árbol. |
| `~BinaryTree()` | Destructor por defecto (la memoria la gestiona `shared_ptr`). |
| `empty()` | Devuelve `true` si el árbol está vacío. |
| `getRootData()` | Devuelve el dato de la raíz. Lanza `std::underflow_error` si el árbol está vacío. |
| `getLeftSubtree()` | Devuelve un árbol cuya raíz es el hijo izquierdo de la raíz actual. |
| `getRightSubtree()` | Devuelve un árbol cuya raíz es el hijo derecho de la raíz actual. |
| `addLeft(BinaryTree& leftTree)` | Conecta `leftTree` como subárbol izquierdo de la raíz actual. |
| `addRight(BinaryTree& rightTree)` | Conecta `rightTree` como subárbol derecho de la raíz actual. |
| `buildTree(leftTree, rightTree, data)` | Crea una raíz con `data` y le conecta los subárboles izquierdo y derecho. |
| `size()` | Devuelve el número total de nodos. |
| `height()` | Devuelve la altura del árbol. |
| `traverseInOrder()` | Imprime el recorrido en inorden (izq – raíz – der). |
| `traversePreOrder()` | Imprime el recorrido en preorden (raíz – izq – der). |
| `traversePostOrder()` | Imprime el recorrido en postorden (izq – der – raíz). |
| `traverseLevelOrder()` | Imprime el recorrido por niveles (anchura). |

### Privados

| Método | Descripción |
|--------|-------------|
| `clone(node)` | Crea una copia profunda del subárbol con raíz en `node`. Lo usa el constructor de copia y `operator=`. |
| `inOrder(node)` | Implementación recursiva del recorrido en inorden. |
| `preOrder(node)` | Implementación recursiva del recorrido en preorden. |
| `postOrder(node)` | Implementación recursiva del recorrido en postorden. |
| `size_(node)` | Cuenta recursivamente los nodos del subárbol. |
| `height_(node)` | Calcula recursivamente la altura del subárbol. |

---

## Construcción del árbol

### `buildTree(leftTree, rightTree, data)` – construir de abajo hacia arriba

La manera principal de construir un árbol con esta implementación es **primero crear los subárboles hoja y luego unirlos**:

```cpp
BinaryTree<int> node4(4);  // Hoja con valor 4
BinaryTree<int> node5(5);  // Hoja con valor 5
BinaryTree<int> node6(6);  // Hoja con valor 6
BinaryTree<int> node7(7);  // Hoja con valor 7

BinaryTree<int> node2;
node2.buildTree(node4, node5, 2);  // 2 tiene hijos 4 y 5

BinaryTree<int> node3;
node3.buildTree(node6, node7, 3);  // 3 tiene hijos 6 y 7

BinaryTree<int> tree;
tree.buildTree(node2, node3, 1);   // 1 tiene hijos 2 y 3
```

El resultado visual:

```
        1
       / \
      2   3
     / \ / \
    4  5 6  7
```

Lo que hace `buildTree` internamente:

```cpp
void buildTree(const BinaryTree& leftTree, const BinaryTree& rightTree, const T& data) {
    root = std::make_shared<Node<T>>(data);   // Crea la nueva raíz
    root->setLeft(leftTree.root);             // Conecta subárbol izquierdo
    root->setRight(rightTree.root);           // Conecta subárbol derecho
}
```

> **Nota sobre `shared_ptr`**: al hacer `root->setLeft(leftTree.root)`, no se copia el subárbol; ambos objetos **comparten** los mismos nodos. Si se quiere independencia total, hay que usar el constructor de copia.

---

## Recorridos del árbol (traversals)

Un recorrido visita todos los nodos del árbol exactamente una vez. La diferencia entre ellos está en **cuándo se procesa la raíz** respecto a sus hijos.

### Inorden: izquierda → raíz → derecha

```cpp
void inOrder(const std::shared_ptr<Node<T>>& node) const {
    if (node != nullptr) {
        inOrder(node->getLeft());    // 1. Visitar subárbol izquierdo
        node->processNode();         // 2. Procesar el nodo actual
        inOrder(node->getRight());   // 3. Visitar subárbol derecho
    }
}
```

Traza sobre el árbol de ejemplo:

```
inOrder(1)
  inOrder(2)
    inOrder(4)
      inOrder(nullptr) → fin
      procesa 4  → imprime "4"
      inOrder(nullptr) → fin
    procesa 2  → imprime "2"
    inOrder(5)
      ...      → imprime "5"
  procesa 1  → imprime "1"
  inOrder(3)
    ...        → imprime "6", luego "3", luego "7"

Salida: 4 2 5 1 6 3 7
```

### Preorden: raíz → izquierda → derecha

```cpp
void preOrder(const std::shared_ptr<Node<T>>& node) const {
    if (node != nullptr) {
        node->processNode();         // 1. Procesar el nodo actual PRIMERO
        preOrder(node->getLeft());   // 2. Visitar subárbol izquierdo
        preOrder(node->getRight());  // 3. Visitar subárbol derecho
    }
}
```

```
Salida: 1 2 4 5 3 6 7
```

El preorden visita la raíz **antes** que sus hijos. Es útil para **serializar** un árbol (guardarlo y reconstruirlo en el mismo orden).

### Postorden: izquierda → derecha → raíz

```cpp
void postOrder(const std::shared_ptr<Node<T>>& node) const {
    if (node != nullptr) {
        postOrder(node->getLeft());   // 1. Visitar subárbol izquierdo
        postOrder(node->getRight());  // 2. Visitar subárbol derecho
        node->processNode();          // 3. Procesar el nodo actual AL FINAL
    }
}
```

```
Salida: 4 5 2 6 7 3 1
```

El postorden visita la raíz **después** de sus hijos. Es útil cuando hay que procesar los nodos hijos antes que el padre (p. ej., al eliminar un árbol nodo a nodo).

### Por niveles (BFS): nivel 0 → nivel 1 → nivel 2 → ...

```
Nivel 0:         1
Nivel 1:       2   3
Nivel 2:      4 5 6 7

Salida: 1 2 3 4 5 6 7
```

A diferencia de los tres anteriores, **no es recursivo**: usa una **cola** (`std::queue`) para procesar los nodos nivel por nivel.

```cpp
void traverseLevelOrder() const {
    std::queue<std::shared_ptr<Node<T>>> q;
    q.push(root);                        // Empezamos con la raíz

    while (!q.empty()) {
        std::shared_ptr<Node<T>> current = q.front();
        q.pop();

        current->processNode();          // Procesamos el nodo actual

        if (current->getLeft() != nullptr)
            q.push(current->getLeft());  // Añadimos hijo izquierdo si existe

        if (current->getRight() != nullptr)
            q.push(current->getRight()); // Añadimos hijo derecho si existe
    }
}
```

**¿Por qué funciona la cola?** Porque la cola es FIFO: cuando sacamos el nodo `1`, añadimos sus hijos `2` y `3`. Cuando sacamos `2`, añadimos sus hijos `4` y `5`. En ese momento la cola tiene `[3, 4, 5]`. Al sacar `3` añadimos `6` y `7`: `[4, 5, 6, 7]`. Así todos los nodos del mismo nivel se procesan juntos antes de bajar al siguiente.

---

## `size_` y `height_` – algoritmos recursivos

### Contar nodos

```cpp
std::size_t size_(const std::shared_ptr<Node<T>>& node) const {
    if (node == nullptr) return 0;
    return 1 + size_(node->getLeft()) + size_(node->getRight());
}
```

La fórmula es directa: el tamaño de un árbol es **1 (la raíz) más el tamaño del subárbol izquierdo más el del derecho**. El caso base es el árbol vacío, que tiene tamaño 0.

```
size_(1) = 1 + size_(2) + size_(3)
size_(2) = 1 + size_(4) + size_(5) = 1 + 1 + 1 = 3
size_(3) = 1 + size_(6) + size_(7) = 1 + 1 + 1 = 3
size_(1) = 1 + 3 + 3 = 7  ✓
```

### Calcular la altura

```cpp
std::size_t height_(const std::shared_ptr<Node<T>>& node) const {
    if (node == nullptr) return 0;

    std::size_t leftHeight  = height_(node->getLeft());
    std::size_t rightHeight = height_(node->getRight());

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}
```

La altura es **1 (la raíz) más la mayor altura entre el subárbol izquierdo y el derecho**.

```
height_(4) = 1 + max(0, 0) = 1
height_(2) = 1 + max(height_(4), height_(5)) = 1 + max(1,1) = 2
height_(1) = 1 + max(height_(2), height_(3)) = 1 + max(2,2) = 3  ✓
```

---

## Copia profunda

### `clone(node)` – copiar un subárbol

```cpp
std::shared_ptr<Node<T>> clone(const std::shared_ptr<Node<T>>& node) const {
    if (node == nullptr) return nullptr;

    std::shared_ptr<Node<T>> newNode = std::make_shared<Node<T>>(node->getData());
    newNode->setLeft(clone(node->getLeft()));    // Clona el subárbol izquierdo
    newNode->setRight(clone(node->getRight()));  // Clona el subárbol derecho
    return newNode;
}
```

Recorre el árbol en **preorden** (crea el nodo actual antes de clonar sus hijos) y devuelve una estructura completamente nueva en memoria, independiente del original.

### Constructor de copia y `operator=`

```cpp
BinaryTree<int> copyTree(tree);     // Constructor de copia: clona tree
BinaryTree<int> assignedTree;
assignedTree = tree;                // Operador de asignación: clona tree
```

Ambas operaciones llaman a `clone(other.root)`, garantizando que el nuevo árbol tenga sus propios nodos independientes.

---

## Compilación y ejecución

Desde la raíz del repositorio:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./BinaryTree
```

---

## Ejemplo de salida esperada

```
Root: 1
Size: 7
Height: 3

In-order: 4 2 5 1 6 3 7
Pre-order: 1 2 4 5 3 6 7
Post-order: 4 5 2 6 7 3 1
Level-order: 1 2 3 4 5 6 7

Copy constructor test:
4 2 5 1 6 3 7

Assignment operator test:
1 2 4 5 3 6 7
```

---

## Notas

- El destructor es `= default` porque `std::shared_ptr` gestiona la memoria automáticamente. No es necesario liberar nada manualmente.
- `buildTree` **comparte** los nodos de los subárboles pasados como argumento; no los clona. Para obtener copias independientes usa el constructor de copia.
- `getRootData()` lanza `std::underflow_error` si el árbol está vacío.
- A diferencia de las listas enlazadas (estructuras lineales), los algoritmos sobre árboles son naturalmente **recursivos** porque cada subárbol es en sí mismo un árbol.
