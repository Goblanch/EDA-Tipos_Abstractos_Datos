# Lista Enlazada Simple (LinkedList) en C++ con `template`

## Descripción

Una **lista enlazada simple** es una estructura de datos lineal formada por nodos encadenados mediante punteros. A diferencia de un array, los elementos no ocupan posiciones contiguas en memoria: cada nodo sabe dónde está el siguiente gracias a su puntero `next`.

Esta implementación está en `LinkedList.h` y es completamente genérica gracias al uso de `template<typename T>`.

---

## Características de esta implementación

- **Genérica** con `template<typename T>`.
- Estructura interna basada en **lista enlazada simple** con punteros `head_` (inicio) y `tail_` (final).
- Implementa **constructor de copia**, **operador de asignación (`operator=`)** y **destructor**.
- Lanza `std::out_of_range` cuando se accede a índices fuera de rango.

---

## Estructura interna

### `Node`

```cpp
struct Node {
    T data;       // Valor almacenado
    Node* next;   // Puntero al siguiente nodo

    Node(const T& value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
};
```

### Atributos de `LinkedList`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `head_` | `Node*` | Puntero al primer nodo de la lista |
| `tail_` | `Node*` | Puntero al último nodo de la lista |
| `size_` | `std::size_t` | Número de elementos actuales |

Mantener `tail_` permite insertar al final en O(1) sin recorrer toda la lista.

---

## Métodos implementados

### Públicos

| Método | Descripción |
|--------|-------------|
| `LinkedList()` | Constructor por defecto. Inicializa `head_` y `tail_` a `nullptr`. |
| `LinkedList(const LinkedList& other)` | Constructor de copia. Crea una copia profunda. |
| `operator=(const LinkedList& other)` | Operador de asignación. Limpia la lista actual y hace copia profunda. |
| `~LinkedList()` | Destructor. Llama a `clear()` para liberar todos los nodos. |
| `empty()` | Devuelve `true` si la lista está vacía. |
| `size()` | Devuelve el número de elementos. |
| `clear()` | Elimina todos los elementos y libera su memoria. |
| `pushFront(const T& value)` | Inserta un elemento al principio de la lista. |
| `pushBack(const T& value)` | Inserta un elemento al final de la lista. |
| `insert(std::size_t index, const T& value)` | Inserta un elemento en el índice indicado. Lanza `std::out_of_range` si el índice es inválido. |
| `remove(const T& value)` | Elimina la primera aparición del valor. Devuelve `true` si lo encontró y eliminó. |
| `removeAt(std::size_t index)` | Elimina el elemento en el índice indicado. Lanza `std::out_of_range` si el índice es inválido. |
| `at(std::size_t index)` | Devuelve una referencia al elemento en el índice indicado. Lanza `std::out_of_range` si el índice es inválido. |
| `print()` | Imprime la lista de `head_` a `nullptr`. |

### Privados

| Método | Descripción |
|--------|-------------|
| `copy(const LinkedList& other)` | Copia todos los nodos de `other` usando `pushBack`. |

---

## Funcionamiento con punteros (MUY IMPORTANTE)

Esta sección explica paso a paso cómo se trabaja con los punteros de la lista enlazada.

### Representación visual

```
head_ → [5] → [10] → [99] → [30] → nullptr
                               ↑
                             tail_
```

- `head_` señala siempre al **primer** nodo.
- `tail_` señala siempre al **último** nodo.
- El campo `next` del último nodo es `nullptr`.

---

### ¿Qué es `current`?

`current` es un **puntero auxiliar** (variable local temporal) que usamos para recorrer la lista sin mover `head_`. Si moviéramos `head_` directamente, perderíamos el inicio de la lista.

```cpp
Node* current = head_;   // current comienza en el primer nodo
while (current != nullptr) {
    // Aquí current apunta al nodo actual
    current = current->next;  // Avanzamos al siguiente nodo
}
```

### `print()` – recorrer la lista

```cpp
Node* current = head_;
while (current != nullptr) {
    std::cout << current->data;
    current = current->next;
}
```

Pasos visuales con la lista `[10] → [20] → [30]`:

```
Iteración 1: current → [10]  → imprime 10
Iteración 2: current → [20]  → imprime 20
Iteración 3: current → [30]  → imprime 30
Iteración 4: current → nullptr → fin del bucle
```

---

### `pushFront(value)` – insertar al principio

```cpp
Node* newNode = new Node(value, head_);  // next del nuevo nodo = antiguo head_
head_ = newNode;                          // head_ apunta al nuevo nodo

if (tail_ == nullptr) {
    tail_ = newNode;   // Si la lista estaba vacía, tail_ también lo apunta
}
++size_;
```

Antes (lista `[10] → [20]`):
```
head_ → [10] → [20] → nullptr
                ↑
              tail_
```

Después de `pushFront(5)`:
```
head_ → [5] → [10] → [20] → nullptr
                       ↑
                     tail_
```

---

### `pushBack(value)` – insertar al final

```cpp
Node* newNode = new Node(value);  // next = nullptr por defecto

if (empty()) {
    head_ = newNode;
    tail_ = newNode;
} else {
    tail_->next = newNode;  // El antiguo último nodo apunta al nuevo
    tail_ = newNode;        // tail_ avanza al nuevo nodo
}
++size_;
```

---

### `insert(index, value)` – insertar en posición arbitraria

Para insertar en medio de la lista necesitamos el **nodo anterior** (`prev`) a la posición deseada, porque debemos reconectar sus punteros.

Casos especiales manejados por `insert`:
- `index == 0` → llama a `pushFront`.
- `index == size_` → llama a `pushBack`.
- En otro caso, recorre hasta el nodo en posición `index - 1`:

```cpp
Node* prev = head_;
for (size_t i = 0; i < index - 1; ++i) {
    prev = prev->next;
}

Node* newNode = new Node(value, prev->next);  // nuevo->next = nodo que estaba en index
prev->next = newNode;                          // prev->next = nuevo nodo
++size_;
```

Ejemplo: insertar `99` en el índice 2 de la lista `[5] → [10] → [20] → [30]`:

```
Antes:  head_ → [5] → [10] → [20] → [30] → nullptr
                        ↑
                       prev (índice 1, buscamos index-1=1)

1. newNode = new Node(99, prev->next)  →  [99]->next = [20]
2. prev->next = newNode                →  [10]->next = [99]

Después: head_ → [5] → [10] → [99] → [20] → [30] → nullptr
```

---

### `remove(value)` – eliminar por valor

Esta es la operación más delicada. Para borrar un nodo hay que tener el **nodo anterior** para reconectar la cadena.

#### Caso especial: borrar `head_`

```cpp
if (head_->data == value) {
    Node* temp = head_;
    head_ = head_->next;   // head_ avanza al segundo nodo
    delete temp;
    --size_;

    if (head_ == nullptr) {
        tail_ = nullptr;   // Lista vacía: tail_ también a nullptr
    }
    return true;
}
```

#### Caso general: buscar el nodo anterior

El truco clave: el bucle **no busca el nodo a borrar**, sino el **nodo anterior** a él. Se detiene cuando `current->next->data == value`:

```cpp
Node* current = head_;
while (current->next != nullptr && current->next->data != value) {
    current = current->next;
}
```

Cuando el bucle termina, `current` apunta al nodo **justo antes** del que queremos borrar. Después:

```cpp
Node* temp = current->next;       // temp → nodo a eliminar
current->next = temp->next;       // saltamos el nodo a eliminar

if (temp == tail_) {
    tail_ = current;              // si era el último, actualizamos tail_
}

delete temp;
--size_;
```

Ejemplo: eliminar `20` de la lista `[5] → [10] → [20] → [30]`:

```
Antes:  head_ → [5] → [10] → [20] → [30] → nullptr
                        ↑      ↑
                      current  temp

1. temp = current->next       → temp apunta a [20]
2. current->next = temp->next → [10]->next = [30]
3. delete temp                → libera [20]

Después: head_ → [5] → [10] → [30] → nullptr
                                ↑
                              tail_
```

---

### `removeAt(index)` – eliminar por índice

Similar a `remove`, pero se llega al nodo anterior recorriendo por índice en lugar de por valor.

#### Borrar el primer nodo (`index == 0`):

```cpp
Node* temp = head_;
head_ = head_->next;
delete temp;
--size_;

if (head_ == nullptr) {
    tail_ = nullptr;
}
```

#### Borrar en otro índice:

```cpp
Node* prev = head_;
for (size_t i = 0; i < index - 1; ++i) {
    prev = prev->next;  // prev queda en la posición index-1
}

Node* temp = prev->next;   // temp → nodo a eliminar (en posición index)
prev->next = temp->next;   // saltamos el nodo

if (temp == tail_) {
    tail_ = temp;          // NOTA: en el código fuente aparece así; lo correcto sería tail_ = prev
}

delete temp;
--size_;
```

---

### `at(index)` – acceso por índice

Recorre la lista con un `current` hasta llegar al índice deseado:

```cpp
Node* current = head_;
for (size_t i = 0; i < index; ++i) {
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

```cpp
while (head_ != nullptr) {
    Node* temp = head_;
    head_ = head_->next;
    delete temp;
}
tail_ = nullptr;
size_ = 0;
```

### `copy(other)` – copia profunda

```cpp
Node* current = other.head_;
while (current != nullptr) {
    pushBack(current->data);
    current = current->next;
}
```

Recorre `other` de principio a fin y llama a `pushBack` por cada elemento, creando nodos nuevos independientes.

### Constructor de copia y `operator=`

```cpp
LinkedList<int> copyList(list);     // Constructor de copia
LinkedList<int> assignedList;
assignedList.pushBack(1000);
assignedList = list;                // Operador de asignación: limpia y copia
```

`operator=` llama primero a `clear()` (para liberar los nodos actuales y evitar fugas de memoria) y después a `copy()`.

---

## Compilación y ejecución

Desde la raíz del repositorio:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./LinkedList
```

---

## Ejemplo de salida esperada

```
Head -> 10 -> 20 -> 30 -> nullptr
Head -> 5 -> 10 -> 20 -> 30 -> nullptr
Head -> 5 -> 10 -> 99 -> 20 -> 30 -> nullptr
Head -> 5 -> 10 -> 99 -> 30 -> nullptr
Head -> 10 -> 99 -> 30 -> nullptr
Element at index 1: 99
Head -> 10 -> 99 -> 30 -> nullptr
Head -> 10 -> 99 -> 30 -> nullptr
```

---

## Notas

- `insert`, `removeAt` y `at` lanzan `std::out_of_range` si el índice es inválido.
- `remove` devuelve `false` si el valor no se encuentra en la lista (no lanza excepción).
- `tail_` permite que `pushBack` sea O(1); sin él sería necesario recorrer toda la lista cada vez.
- La copia profunda garantiza que `copyList` y `assignedList` sean completamente independientes de `list`.
