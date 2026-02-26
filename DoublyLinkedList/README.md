# Lista Doblemente Enlazada (DoublyLinkedList) en C++ con `template`

## Descripción

Una **lista doblemente enlazada** es una estructura de datos lineal formada por nodos encadenados mediante dos punteros: uno al nodo siguiente (`next`) y otro al nodo anterior (`prev`). A diferencia de la lista enlazada simple, permite recorrer la lista en **ambas direcciones** (hacia adelante y hacia atrás).

Esta implementación está en `DoublyLinkedList.h` y es completamente genérica gracias al uso de `template<typename T>`. Utiliza **punteros inteligentes** (`std::shared_ptr` y `std::weak_ptr`) para gestionar la memoria de forma automática y segura, evitando fugas de memoria sin necesidad de llamadas explícitas a `delete`.

---

## Características de esta implementación

- **Genérica** con `template<typename T>`.
- Estructura interna basada en **lista doblemente enlazada** con punteros inteligentes `head` (inicio) y `tail` (final).
- Usa `std::shared_ptr<Node>` para `next` y `head`/`tail`, y `std::weak_ptr<Node>` para `prev`, evitando referencias cíclicas que impedirían la liberación de memoria automática.
- Implementa **constructor de copia**, **operador de asignación (`operator=`)** y **destructor**.
- El método `at()` realiza una búsqueda bidireccional optimizada: recorre desde `head` si el índice está en la primera mitad, y desde `tail` si está en la segunda.
- Lanza `std::out_of_range` cuando se accede a índices fuera de rango.

---

## Estructura interna

### `Node`

```cpp
struct Node {
    T data;                    // Valor almacenado
    std::shared_ptr<Node> next; // Puntero al siguiente nodo
    std::weak_ptr<Node> prev;   // Puntero al nodo anterior (weak para evitar ciclos)

    Node(const T& value) : data(value), next(nullptr), prev() {}
};
```

#### ¿Por qué `weak_ptr` para `prev`?

Si tanto `next` como `prev` fueran `shared_ptr`, dos nodos adyacentes se apuntarían mutuamente con referencias fuertes. Esto crearía un **ciclo de referencias** que impediría que el contador de referencias llegara a cero, provocando una fuga de memoria. Al usar `weak_ptr` en `prev`, la referencia hacia atrás no incrementa el contador de referencias, y los nodos se liberan correctamente cuando ya no hay ningún `shared_ptr` apuntando a ellos.

### Atributos de `DoublyLinkedList`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `head` | `std::shared_ptr<Node>` | Puntero al primer nodo de la lista |
| `tail` | `std::shared_ptr<Node>` | Puntero al último nodo de la lista |
| `size` | `std::size_t` | Número de elementos actuales |

Mantener `tail` permite insertar al final en O(1) y recorrer la lista desde el extremo posterior en `at()` y `printBackward()`.

---

## Métodos implementados

### Públicos

| Método | Descripción |
|--------|-------------|
| `DoublyLinkedList()` | Constructor por defecto. Inicializa `head` y `tail` a `nullptr` y `size` a 0. |
| `DoublyLinkedList(const DoublyLinkedList& other)` | Constructor de copia. Crea una copia profunda. |
| `operator=(const DoublyLinkedList& other)` | Operador de asignación. Limpia la lista actual y hace copia profunda. |
| `~DoublyLinkedList()` | Destructor. Llama a `clear()` para liberar todos los nodos. |
| `empty()` | Devuelve `true` si la lista está vacía. |
| `getSize()` | Devuelve el número de elementos. |
| `clear()` | Elimina todos los elementos poniendo `head` y `tail` a `nullptr` (los `shared_ptr` liberan la memoria automáticamente). |
| `pushFront(const T& value)` | Inserta un elemento al principio de la lista. |
| `pushBack(const T& value)` | Inserta un elemento al final de la lista. |
| `insert(std::size_t index, const T& value)` | Inserta un elemento en el índice indicado. Lanza `std::out_of_range` si el índice es inválido. |
| `remove(const T& value)` | Elimina la primera aparición del valor. Devuelve `true` si lo encontró y eliminó. |
| `removeAt(std::size_t index)` | Elimina el elemento en el índice indicado. Lanza `std::out_of_range` si el índice es inválido. |
| `at(std::size_t index)` | Devuelve una referencia al elemento en el índice indicado. Busca desde `head` o `tail` según la mitad en que esté el índice. Lanza `std::out_of_range` si el índice es inválido. |
| `printForward()` | Imprime la lista de `head` a `tail` con el formato `Head -> ... <- Tail`. |
| `printBackward()` | Imprime la lista de `tail` a `head` con el formato `Tail -> ... <- Head`. |

### Privados

| Método | Descripción |
|--------|-------------|
| `copy(const DoublyLinkedList& other)` | Copia todos los nodos de `other` usando `pushBack`. Reinicializa `head`, `tail` y `size` antes de copiar. |

---

## Funcionamiento con punteros (MUY IMPORTANTE)

Esta sección explica paso a paso cómo se trabaja con los punteros de la lista doblemente enlazada.

### Representación visual

```
         ┌───────┐   ┌───────┐   ┌───────┐   ┌───────┐
head  →  │  [5]  │ ↔ │ [10]  │ ↔ │ [99]  │ ↔ │ [30]  │  ← tail
         └───────┘   └───────┘   └───────┘   └───────┘
```

- `head` señala siempre al **primer** nodo; su `prev` es vacío (`weak_ptr` no inicializado).
- `tail` señala siempre al **último** nodo; su `next` es `nullptr`.
- Cada nodo intermedio tiene `next` apuntando al siguiente y `prev` apuntando al anterior.

---

### ¿Qué es `current`?

`current` es un **puntero inteligente auxiliar** (variable local temporal de tipo `shared_ptr<Node>`) que usamos para recorrer la lista sin mover `head` ni `tail`. Incrementa temporalmente el contador de referencias del nodo al que apunta.

```cpp
std::shared_ptr<Node> current = head;
while (current != nullptr) {
    // Aquí current apunta al nodo actual
    current = current->next;  // Avanzamos al siguiente nodo
}
```

Para recorrer hacia atrás se usa `current->prev.lock()`, que convierte el `weak_ptr` en un `shared_ptr` válido (o en `nullptr` si el nodo ya fue destruido):

```cpp
std::shared_ptr<Node> current = tail;
while (current != nullptr) {
    std::shared_ptr<Node> previous = current->prev.lock();
    current = previous;
}
```

---

### `printForward()` – recorrer la lista hacia adelante

```cpp
std::shared_ptr<Node> current = head;
std::cout << "Head -> ";
while (current != nullptr) {
    std::cout << current->data;
    if (current->next != nullptr) {
        std::cout << " <-> ";
    }
    current = current->next;
}
std::cout << " <- Tail\n";
```

Pasos visuales con la lista `[10] <-> [20] <-> [30]`:

```
Iteración 1: current → [10]  → imprime "10 <-> "
Iteración 2: current → [20]  → imprime "20 <-> "
Iteración 3: current → [30]  → imprime "30"
Iteración 4: current → nullptr → fin del bucle
```

Salida: `Head -> 10 <-> 20 <-> 30 <- Tail`

---

### `printBackward()` – recorrer la lista hacia atrás

```cpp
std::shared_ptr<Node> current = tail;
std::cout << "Tail -> ";
while (current != nullptr) {
    std::cout << current->data;
    std::shared_ptr<Node> previous = current->prev.lock();
    if (previous != nullptr) {
        std::cout << " <-> ";
    }
    current = previous;
}
std::cout << " <- Head\n";
```

Pasos visuales con la lista `[10] <-> [20] <-> [30]`:

```
Iteración 1: current → [30]  → imprime "30 <-> "
Iteración 2: current → [20]  → imprime "20 <-> "
Iteración 3: current → [10]  → imprime "10"
Iteración 4: current → nullptr → fin del bucle
```

Salida: `Tail -> 30 <-> 20 <-> 10 <- Head`

---

### `pushFront(value)` – insertar al principio

```cpp
std::shared_ptr<Node> newNode = std::make_shared<Node>(value);

if (empty()) {
    head = tail = newNode;
} else {
    newNode->next = head;   // El nuevo nodo apunta al antiguo head
    head->prev = newNode;   // El antiguo head apunta de vuelta al nuevo nodo
    head = newNode;          // head avanza al nuevo nodo
}

++size;
```

Antes (lista `[10] <-> [20]`):
```
head → [10] <-> [20] ← tail
```

Después de `pushFront(5)`:
```
head → [5] <-> [10] <-> [20] ← tail
```

---

### `pushBack(value)` – insertar al final

```cpp
std::shared_ptr<Node> newNode = std::make_shared<Node>(value);

if (empty()) {
    head = tail = newNode;
} else {
    newNode->prev = tail;   // El nuevo nodo apunta al antiguo tail
    tail->next = newNode;   // El antiguo tail apunta al nuevo nodo
    tail = newNode;          // tail avanza al nuevo nodo
}

++size;
```

---

### `insert(index, value)` – insertar en posición arbitraria

Casos especiales manejados:
- `index == 0` → llama a `pushFront`.
- `index == size` → llama a `pushBack`.
- En otro caso, recorre hasta el nodo que actualmente ocupa la posición `index` y lo desplaza:

```cpp
std::shared_ptr<Node> current = head;
for (std::size_t i = 0; i < index; ++i) {
    current = current->next;
}

std::shared_ptr<Node> previous = current->prev.lock();
std::shared_ptr<Node> newNode = std::make_shared<Node>(value);

newNode->next = current;   // nuevo->next = nodo que estaba en index
newNode->prev = previous;  // nuevo->prev = nodo que estaba en index-1

previous->next = newNode;  // nodo anterior apunta al nuevo
current->prev = newNode;   // nodo siguiente apunta de vuelta al nuevo

++size;
```

Ejemplo: insertar `99` en el índice 2 de la lista `[5] <-> [10] <-> [20] <-> [30]`:

```
Antes:  head → [5] <-> [10] <-> [20] <-> [30] ← tail
                          ↑       ↑
                       previous current (índice 2)

1. newNode = new Node(99)
2. newNode->next = current    →  [99]->next = [20]
3. newNode->prev = previous   →  [99]->prev = [10]
4. previous->next = newNode   →  [10]->next = [99]
5. current->prev  = newNode   →  [20]->prev = [99]

Después: head → [5] <-> [10] <-> [99] <-> [20] <-> [30] ← tail
```

---

### `remove(value)` – eliminar por valor

Primero recorre la lista buscando el nodo con el valor indicado. Una vez encontrado, distingue tres casos:

#### Eliminar la cabeza (`current == head`)

```cpp
head = head->next;
if (head != nullptr) {
    head->prev.reset();  // El nuevo head no tiene predecesor
} else {
    tail = nullptr;       // La lista quedó vacía
}
```

#### Eliminar la cola (`current == tail`)

```cpp
tail = tail->prev.lock();
tail->next = nullptr;
```

#### Eliminar un nodo intermedio

```cpp
std::shared_ptr<Node> previous = current->prev.lock();
std::shared_ptr<Node> nextNode = current->next;

previous->next = nextNode;  // El nodo anterior salta al siguiente
nextNode->prev = previous;  // El nodo siguiente apunta de vuelta al anterior
```

Ejemplo: eliminar `20` de la lista `[5] <-> [10] <-> [20] <-> [30]`:

```
Antes:  head → [5] <-> [10] <-> [20] <-> [30] ← tail
                          ↑       ↑       ↑
                       previous current  nextNode

1. previous->next = nextNode  →  [10]->next = [30]
2. nextNode->prev = previous  →  [30]->prev = [10]
   (current queda sin referencias shared_ptr → se libera automáticamente)

Después: head → [5] <-> [10] <-> [30] ← tail
```

---

### `removeAt(index)` – eliminar por índice

Similar a `remove`, pero se llega al nodo a eliminar recorriendo por índice en lugar de por valor. Distingue igualmente los tres casos (cabeza, cola, intermedio) y reconecta los punteros de la misma forma.

```cpp
std::shared_ptr<Node> current = head;
for (std::size_t i = 0; i < index; ++i) {
    current = current->next;
}
// A continuación se aplica la misma lógica de remove para cabeza, cola o intermedio
```

---

### `at(index)` – acceso por índice con búsqueda bidireccional

En lugar de recorrer siempre desde `head`, este método elige la dirección más corta:

```cpp
std::shared_ptr<Node> current;

if (index < size / 2) {
    // El índice está en la primera mitad: recorrer desde head
    current = head;
    for (std::size_t i = 0; i < index; ++i) {
        current = current->next;
    }
} else {
    // El índice está en la segunda mitad: recorrer desde tail
    current = tail;
    for (std::size_t i = size; i > index; --i) {
        current = current->prev.lock();
    }
}

return current->data;
```

Ejemplo con lista `[10] <-> [99] <-> [30]` y `size = 3`:
- `at(1)`: `1 < 3/2 = 1` es falso → recorre desde `tail` con `i` desde `size=3` hasta `i > 1`, lo que produce **dos** pasos atrás → nodo `[10]` (índice 0).

---

### Secuencia típica de borrado en nodo intermedio (resumen)

```cpp
std::shared_ptr<Node> previous = current->prev.lock();  // Paso 1: obtener el nodo anterior
std::shared_ptr<Node> nextNode = current->next;          // Paso 2: obtener el nodo siguiente
previous->next = nextNode;                               // Paso 3: saltar el nodo en la cadena forward
nextNode->prev = previous;                               // Paso 4: saltar el nodo en la cadena backward
// current ya no tiene shared_ptr apuntando a él → se destruye automáticamente
```

Al usar `shared_ptr`, **no es necesario llamar a `delete`** explícitamente: el nodo se destruye en cuanto ningún `shared_ptr` lo referencia.

---

## Copia y gestión de memoria

### `clear()` – liberar todos los nodos

```cpp
void clear() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}
```

A diferencia de las implementaciones con punteros crudos, no es necesario recorrer la lista nodo a nodo. Al poner `head` y `tail` a `nullptr`, el contador de referencias de cada `shared_ptr` se va reduciendo en cascada: el primer nodo queda sin referencias y se destruye, lo que a su vez reduce el contador del segundo nodo, y así sucesivamente.

### `copy(other)` – copia profunda

```cpp
void copy(const DoublyLinkedList& other) {
    head = nullptr;
    tail = nullptr;
    size = 0;

    std::shared_ptr<Node> current = other.head;
    while (current != nullptr) {
        pushBack(current->data);
        current = current->next;
    }
}
```

Recorre `other` de principio a fin y llama a `pushBack` por cada elemento, creando nodos nuevos completamente independientes.

### Constructor de copia y `operator=`

```cpp
DoublyLinkedList<int> copyList(list);        // Constructor de copia

DoublyLinkedList<int> assignedList;
assignedList.pushBack(1000);
assignedList = list;                         // Operador de asignación: limpia y copia
```

`operator=` comprueba primero la autoasignación (`this != &other`), luego llama a `clear()` para liberar los nodos actuales y evitar fugas de memoria, y después a `copy()`.

---

## Compilación y ejecución

Desde la raíz del repositorio:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./DoublyLinkedList
```

---

## Ejemplo de salida esperada

```
Head -> 10 <-> 20 <-> 30 <- Tail
Tail -> 30 <-> 20 <-> 10 <- Head
Head -> 5 <-> 10 <-> 20 <-> 30 <- Tail
Head -> 5 <-> 10 <-> 99 <-> 20 <-> 30 <- Tail
Head -> 5 <-> 10 <-> 99 <-> 30 <- Tail
Head -> 10 <-> 99 <-> 30 <- Tail
Element at index 1: 10
Head -> 10 <-> 99 <-> 30 <- Tail
Head -> 10 <-> 99 <-> 30 <- Tail
```

---

## Notas

- `insert`, `removeAt` y `at` lanzan `std::out_of_range` si el índice es inválido.
- `remove` devuelve `false` si el valor no se encuentra en la lista (no lanza excepción).
- El uso de `std::shared_ptr` y `std::weak_ptr` elimina la necesidad de gestionar la memoria manualmente: no hay llamadas a `new`/`delete` fuera de `std::make_shared`.
- `std::weak_ptr` en `prev` es imprescindible para evitar ciclos de referencias que impedirían la liberación automática de la memoria.
- `tail` permite que `pushBack` sea O(1) y que `at()` realice la búsqueda desde el extremo más cercano al índice solicitado.
- La copia profunda garantiza que `copyList` y `assignedList` sean completamente independientes de `list`.
