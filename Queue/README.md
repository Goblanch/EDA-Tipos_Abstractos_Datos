# Cola (Queue) en C++ con `template`

## Descripción

Una **cola** es una estructura de datos lineal que sigue la política **FIFO** (*First In, First Out*): el primer elemento en entrar es el primero en salir. Es la analogía de una cola de personas: quien llega antes, sale antes.

Esta implementación está en `GenericQueue.h` y es completamente genérica gracias al uso de `template<typename T>`.

---

## Características de esta implementación

- **Genérica** con `template<typename T>`.
- Estructura interna basada en **lista enlazada simple** con dos punteros extremos (`front_` y `back_`), lo que permite insertar y eliminar en tiempo O(1).
- Implementa **constructor de copia**, **operador de asignación (`operator=`)** y **destructor**.
- Lanza excepciones de tipo `std::underflow_error` al intentar acceder o eliminar en una cola vacía.

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

### Atributos de `GenericQueue`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `front_` | `Node*` | Puntero al primer nodo de la cola (el más antiguo) |
| `back_` | `Node*` | Puntero al último nodo de la cola (el más reciente) |
| `size_` | `std::size_t` | Número de elementos actuales en la cola |

---

## Métodos implementados

### Públicos

| Método | Descripción |
|--------|-------------|
| `GenericQueue()` | Constructor por defecto. Inicializa `front_` y `back_` a `nullptr`. |
| `GenericQueue(GenericQueue& other)` | Constructor de copia. Crea una copia profunda. |
| `operator=(GenericQueue& other)` | Operador de asignación. Limpia la cola actual y hace copia profunda. |
| `~GenericQueue()` | Destructor. Llama a `clear()` para liberar todos los nodos. |
| `enqueue(const T& value)` | Inserta un elemento al final de la cola. |
| `dequeue()` | Elimina el elemento del frente. Lanza `std::underflow_error` si está vacía. |
| `front()` | Devuelve una referencia al elemento del frente. Lanza `std::underflow_error` si está vacía. |
| `empty()` | Devuelve `true` si la cola está vacía. |
| `size()` | Devuelve el número de elementos. |
| `print()` | Imprime la cola de frente a fondo. |

### Privados

| Método | Descripción |
|--------|-------------|
| `clear()` | Libera todos los nodos y reinicia `front_`, `back_` y `size_`. |
| `copy(GenericQueue& other)` | Copia todos los nodos de `other` en el mismo orden usando `enqueue`. |

---

## Funcionamiento con punteros

### Estado inicial (cola vacía)

```
front_ → nullptr
back_  → nullptr
```

### Después de `enqueue(10)`, `enqueue(20)`, `enqueue(30)`

```
front_ → [10] → [20] → [30] → nullptr
                         ↑
                       back_
```

El frente (`front_`) es el primer elemento en entrar; el fondo (`back_`) es el último.

### `enqueue(value)` – insertar al final

```cpp
Node* newNode = new Node(value);

if (empty()) {
    front_ = back_ = newNode;   // Primer nodo: ambos punteros lo señalan
} else {
    back_->next = newNode;      // El antiguo último nodo apunta al nuevo
    back_ = newNode;            // back_ avanza al nuevo nodo
}
++size_;
```

Pasos visuales (insertar 30 cuando ya hay `10 → 20`):

```
Antes:  front_ → [10] → [20] → nullptr
                          ↑
                        back_

1. back_->next = newNode:  [20]->next = [30]
2. back_ = newNode:        back_ apunta a [30]

Después: front_ → [10] → [20] → [30] → nullptr
                                  ↑
                                back_
```

### `dequeue()` – eliminar el frente

```cpp
Node* temp = front_;        // 1. Guardamos puntero al frente actual
front_ = front_->next;      // 2. El nuevo frente es el siguiente nodo
delete temp;                 // 3. Liberamos la memoria del nodo eliminado
--size_;

if (front_ == nullptr) {
    back_ = nullptr;        // 4. Si la cola quedó vacía, back_ también es nullptr
}
```

Por qué guardar `temp`: si borrásemos `front_` directamente perderíamos la referencia a `front_->next` y no podríamos avanzar el puntero.

Antes del `dequeue`:
```
front_ → [10] → [20] → [30] → nullptr
                         ↑
                       back_
```

Después del `dequeue`:
```
front_ → [20] → [30] → nullptr
                  ↑
                back_
```

El nodo `[10]` ha sido liberado con `delete temp`.

### `print()` – recorrer la cola

```cpp
Node* current = front_;
while (current != nullptr) {
    std::cout << current->data;
    current = current->next;
}
```

- `current` empieza en `front_` (el primer elemento).
- Avanza con `current = current->next` en cada iteración.
- El bucle termina cuando `current == nullptr`.

---

## Copia y gestión de memoria

### `clear()` – liberar todos los nodos

```cpp
while (front_ != nullptr) {
    Node* temp = front_;
    front_ = front_->next;
    delete temp;
}
back_ = nullptr;
size_ = 0;
```

Recorre la lista de frente a fondo, liberando cada nodo sin perder el puntero al siguiente.

### `copy(other)` – copia profunda

```cpp
Node* current = other.front_;
while (current != nullptr) {
    enqueue(current->data);
    current = current->next;
}
```

Recorre la cola `other` de frente a fondo y llama a `enqueue` por cada valor, creando nodos nuevos en la cola actual. Así los dos objetos son independientes en memoria.

### Constructor de copia y `operator=`

```cpp
GenericQueue<int> q2(q);    // Constructor de copia
GenericQueue<int> q3;
q3 = q;                     // Operador de asignación
```

`operator=` llama primero a `clear()` para evitar fugas de memoria y después a `copy()` para la copia profunda. La comprobación `if (this != &other)` evita la auto-asignación.

---

## Compilación y ejecución

Desde la raíz del repositorio:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./Queue
```

---

## Ejemplo de salida esperada

```
########## COLA ORIGINAL ##########
Front -> 10 | 20 | 30 <- Back
Elemento delante: 10
########## COLA COPIADA ##########
Front -> 10 | 20 | 30 <- Back
########## COLA OPERADOR = ##########
Front -> 10 | 20 | 30 <- Back
Despues de dequeue, cola original:
Front -> 20 | 30 <- Back
Cola copiada se mantiene intacta:
Front -> 10 | 20 | 30 <- Back
```

---

## Notas

- `dequeue()` y `front()` lanzan `std::underflow_error` si la cola está vacía.
- Cuando tras un `dequeue()` la cola queda vacía, `back_` se pone a `nullptr` explícitamente para evitar puntero colgante.
- La copia profunda garantiza que `q2` y `q3` sean completamente independientes de `q`.
