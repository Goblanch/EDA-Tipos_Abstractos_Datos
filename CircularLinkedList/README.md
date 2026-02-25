# Lista Enlazada Circular (CircularLinkedList) en C++ con `template`

## Descripción

Una **lista enlazada circular** es una variante de la lista enlazada simple en la que el último nodo **no apunta a `nullptr`**, sino que su puntero `next` apunta de vuelta al primer nodo (`head_`). Esto forma un ciclo cerrado que permite recorrer la lista indefinidamente sin llegar a un extremo nulo.

Esta implementación está en `CircularLinkedList.h` y es completamente genérica gracias al uso de `template<typename T>`.

---

## Características de esta implementación

- **Genérica** con `template<typename T>`.
- Estructura interna basada en **lista enlazada circular** con punteros `head_` (inicio) y `tail_` (final).
- El puntero `next` de `tail_` apunta siempre a `head_`, cerrando el ciclo.
- Implementa **constructor de copia**, **operador de asignación (`operator=`)** y **destructor**.
- Lanza `std::out_of_range` cuando se accede a índices fuera de rango.

---

## Estructura interna

### `Node`

```cpp
struct Node {
    T data;       // Valor almacenado
    Node* next;   // Puntero al siguiente nodo (o a head_ si es el último)

    Node(const T& value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
};
```

### Atributos de `CircularLinkedList`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `head_` | `Node*` | Puntero al primer nodo de la lista |
| `tail_` | `Node*` | Puntero al último nodo de la lista |
| `size_` | `std::size_t` | Número de elementos actuales |

Mantener `tail_` permite insertar al final en O(1) y mantener la circularidad (`tail_->next = head_`) sin recorrer toda la lista.

---

## Métodos implementados

### Públicos

| Método | Descripción |
|--------|-------------|
| `CircularLinkedList()` | Constructor por defecto. Inicializa `head_`, `tail_` a `nullptr` y `size_` a 0. |
| `CircularLinkedList(const CircularLinkedList& other)` | Constructor de copia. Crea una copia profunda. |
| `operator=(const CircularLinkedList& other)` | Operador de asignación. Limpia la lista actual y hace copia profunda. |
| `~CircularLinkedList()` | Destructor. Llama a `clear()` para liberar todos los nodos. |
| `empty()` | Devuelve `true` si la lista está vacía. |
| `size()` | Devuelve el número de elementos. |
| `clear()` | Elimina todos los elementos y libera su memoria. |
| `pushFront(const T& value)` | Inserta un elemento al principio de la lista. |
| `pushBack(const T& value)` | Inserta un elemento al final de la lista. |
| `insert(std::size_t index, const T& value)` | Inserta un elemento en el índice indicado. Lanza `std::out_of_range` si el índice es inválido. |
| `remove(const T& value)` | Elimina la primera aparición del valor. Devuelve `true` si lo encontró y eliminó. |
| `removeAt(std::size_t index)` | Elimina el elemento en el índice indicado. Lanza `std::out_of_range` si el índice es inválido. |
| `at(std::size_t index)` | Devuelve una referencia al elemento en el índice indicado. Lanza `std::out_of_range` si el índice es inválido. |
| `print()` | Imprime la lista desde `head_` indicando que el último enlace vuelve a `head_`. |

### Privados

| Método | Descripción |
|--------|-------------|
| `copy(const CircularLinkedList& other)` | Copia todos los nodos de `other` usando `pushBack`. |

---

## Funcionamiento con punteros (MUY IMPORTANTE)

Esta sección explica paso a paso cómo se trabaja con los punteros de la lista enlazada circular.

### Representación visual

```
head_ → [5] → [10] → [99] → [30]
  ↑                            |
  └────────── tail_->next ─────┘
                    ↑
                  tail_
```

- `head_` señala siempre al **primer** nodo.
- `tail_` señala siempre al **último** nodo.
- El campo `next` del último nodo **apunta de vuelta a `head_`**, cerrando el ciclo.

---

### ¿Por qué no se puede usar `nullptr` para detectar el final?

En una lista lineal el bucle termina cuando `current == nullptr`. Aquí **nunca** llegamos a `nullptr`: todos los nodos están conectados en círculo. Por eso se usa el **contador `size_`** para saber cuántos nodos hay que visitar:

```cpp
Node* current = head_;
for (std::size_t i = 0; i < size_; ++i) {
    // Procesamos current->data
    current = current->next;
}
```

---

### `print()` – recorrer la lista

```cpp
Node* current = head_;
std::cout << "Head -> ";
for (size_t i = 0; i < size_; ++i) {
    std::cout << current->data << " -> ";
    current = current->next;
}
std::cout << "(back to Head)\n";
```

Pasos visuales con la lista `[10] → [20] → [30]` (con `tail_->next = head_`):

```
Iteración 1: current → [10]  → imprime "10 -> "
Iteración 2: current → [20]  → imprime "20 -> "
Iteración 3: current → [30]  → imprime "30 -> "
Fin del bucle (i == size_) → imprime "(back to Head)"
```

---

### `pushFront(value)` – insertar al principio

```cpp
Node* newNode = new Node(value);

if (empty()) {
    head_ = tail_ = newNode;
    newNode->next = newNode; // Enlace circular consigo mismo
} else {
    newNode->next = head_; // El nuevo nodo apunta al antiguo head_
    head_ = newNode;       // head_ apunta al nuevo nodo
    tail_->next = head_;   // tail_ actualiza su enlace al nuevo head_
}

++size_;
```

Antes (lista `[10] → [20]`, con `tail_->next = head_`):
```
head_ → [10] → [20]
  ↑               |
  └───────────────┘
              tail_
```

Después de `pushFront(5)`:
```
head_ → [5] → [10] → [20]
  ↑                     |
  └─────────────────────┘
                    tail_
```

---

### `pushBack(value)` – insertar al final

```cpp
Node* newNode = new Node(value);

if (empty()) {
    head_ = tail_ = newNode;
    newNode->next = newNode; // Enlace circular consigo mismo
} else {
    newNode->next = head_;  // El nuevo nodo cierra el ciclo apuntando a head_
    tail_->next = newNode;  // El antiguo tail_ apunta al nuevo nodo
    tail_ = newNode;        // tail_ avanza al nuevo nodo
}

++size_;
```

---

### `insert(index, value)` – insertar en posición arbitraria

Casos especiales manejados:
- `index == 0` → llama a `pushFront`.
- `index == size_` → llama a `pushBack`.
- En otro caso, recorre hasta el nodo en posición `index - 1`:

```cpp
Node* current = head_;
for (std::size_t i = 0; i < index - 1; ++i) {
    current = current->next;
}

Node* newNode = new Node(value, current->next); // nuevo->next = nodo en posición index
current->next = newNode;                        // nodo anterior apunta al nuevo
++size_;
```

Ejemplo: insertar `99` en el índice 2 de la lista `[5] → [10] → [20] → [30]` (circular):

```
Antes:  head_ → [5] → [10] → [20] → [30] → (head_)
                        ↑
                      current (índice 1, buscamos index-1=1)

1. newNode = new Node(99, current->next)  →  [99]->next = [20]
2. current->next = newNode                →  [10]->next = [99]

Después: head_ → [5] → [10] → [99] → [20] → [30] → (head_)
```

---

### `remove(value)` – eliminar por valor

Esta es la operación más delicada en una lista circular porque **no existe `nullptr`** como condición de fin del recorrido. La condición de parada del bucle es `current->next != head_` (volvimos al inicio sin encontrar el valor):

#### Caso especial: borrar `head_`

```cpp
if (head_->data == value) {
    if (size_ == 1) {       // Solo hay un nodo
        delete head_;
        head_ = tail_ = nullptr;
        size_ = 0;
        return true;
    }

    Node* temp = head_;
    head_ = head_->next;   // head_ avanza al segundo nodo
    tail_->next = head_;   // tail_ actualiza el enlace circular al nuevo head_
    delete temp;
    --size_;
    return true;
}
```

#### Caso general: buscar el nodo anterior

El bucle **no busca el nodo a borrar**, sino el **nodo anterior** a él. Se detiene cuando `current->next->data == value` o cuando `current->next == head_` (fin del recorrido sin encontrarlo):

```cpp
Node* current = head_;
while (current->next != head_ && current->next->data != value) {
    current = current->next;
}

// No encontrado: current->next volvió a head_ sin coincidir
if (current->next == head_) {
    return false;
}
```

Cuando se encuentra, se reconecta la cadena y se actualiza `tail_` si era el último:

```cpp
Node* temp = current->next;       // temp → nodo a eliminar
current->next = temp->next;       // saltamos el nodo

if (temp == tail_) {
    tail_ = current;              // si era el último, actualizamos tail_
}

delete temp;
--size_;
```

Ejemplo: eliminar `20` de `[5] → [10] → [20] → [30] → (head_)`:

```
Antes:  head_ → [5] → [10] → [20] → [30] → (head_)
                        ↑      ↑
                      current  temp

1. temp = current->next       → temp apunta a [20]
2. current->next = temp->next → [10]->next = [30]
3. delete temp                → libera [20]

Después: head_ → [5] → [10] → [30] → (head_)
                                ↑
                              tail_
```

---

### `removeAt(index)` – eliminar por índice

Similar a `remove`, pero se llega al nodo anterior recorriendo por índice.

#### Borrar el primer nodo (`index == 0`):

```cpp
if (size_ == 1) {
    delete head_;
    head_ = tail_ = nullptr;
    size_ = 0;
    return;
}

Node* temp = head_;
head_ = head_->next;
tail_->next = head_;   // Mantener la circularidad con el nuevo head_
delete temp;
--size_;
```

#### Borrar en otro índice:

```cpp
Node* current = head_;
for (size_t i = 0; i < index - 1; ++i) {
    current = current->next;  // current queda en la posición index-1
}

Node* temp = current->next;   // temp → nodo a eliminar (en posición index)
current->next = temp->next;   // saltamos el nodo

if (temp == tail_) {
    tail_ = current;           // si era el último, actualizamos tail_
}

delete temp;
--size_;
```

---

### `at(index)` – acceso por índice

Recorre la lista con un `current` durante `index` pasos:

```cpp
Node* current = head_;
for (std::size_t i = 0; i < index; ++i) {
    current = current->next;
}
return current->data;
```

---

### Secuencia típica de borrado (resumen)

```cpp
Node* temp = current->next;       // Paso 1: guardar el nodo a borrar
current->next = temp->next;       // Paso 2: saltar ese nodo en la cadena
delete temp;                       // Paso 3: liberar memoria
```

Estos tres pasos siempre aparecen juntos cuando se elimina un nodo intermedio o final. **Nunca** se puede hacer `delete current->next` antes de guardar `current->next->next`, porque se perdería el enlace al resto de la lista.

---

## Copia y gestión de memoria

### `clear()` – liberar todos los nodos

En una lista circular no podemos iterar con `while (current != nullptr)` porque nunca llegamos a `nullptr`. Se usa el contador `size_` para saber exactamente cuántos nodos hay que liberar:

```cpp
void clear() {
    if (empty()) return;

    Node* current = head_;
    for (std::size_t i = 0; i < size_; ++i) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }

    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
}
```

### `copy(other)` – copia profunda

```cpp
void copy(const CircularLinkedList& other) {
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;

    if (other.empty()) return;

    Node* current = other.head_;
    for (std::size_t i = 0; i < other.size_; ++i) {
        pushBack(current->data);
        current = current->next;
    }
}
```

Recorre `other` mediante contador y llama a `pushBack` por cada elemento, creando nodos nuevos completamente independientes.

### Constructor de copia y `operator=`

```cpp
CircularLinkedList<int> copyList(list);     // Constructor de copia

CircularLinkedList<int> assignedList;
assignedList.pushBack(1000);
assignedList = list;                        // Operador de asignación: limpia y copia
```

`operator=` comprueba primero la autoasignación (`this != &other`), luego llama a `clear()` para liberar los nodos actuales y evitar fugas de memoria, y después a `copy()`.

---

## Compilación y ejecución

Desde la raíz del repositorio:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./CircularLinkedList
```

---

## Ejemplo de salida esperada

```
Head -> 10 -> 20 -> 30 -> (back to Head)
Head -> 5 -> 10 -> 20 -> 30 -> (back to Head)
Head -> 5 -> 10 -> 99 -> 20 -> 30 -> (back to Head)
Head -> 5 -> 10 -> 99 -> 30 -> (back to Head)
Head -> 10 -> 99 -> 30 -> (back to Head)
Element at index 1: 99
Head -> 10 -> 99 -> 30 -> (back to Head)
Head -> 10 -> 99 -> 30 -> (back to Head)
```

---

## Notas

- `insert`, `removeAt` y `at` lanzan `std::out_of_range` si el índice es inválido.
- `remove` devuelve `false` si el valor no se encuentra en la lista (no lanza excepción).
- `tail_` permite que `pushBack` sea O(1) y mantiene la circularidad sin recorrer toda la lista.
- La circularidad implica que **siempre** hay que usar el contador `size_` o la condición `current->next != head_` para evitar bucles infinitos.
- La copia profunda garantiza que `copyList` y `assignedList` sean completamente independientes de `list`.
