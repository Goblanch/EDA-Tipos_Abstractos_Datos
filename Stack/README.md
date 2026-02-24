# Pila (Stack) en C++ con `template`

## Descripción

Una **pila** es una estructura de datos lineal que sigue la política **LIFO** (*Last In, First Out*): el último elemento en entrar es el primero en salir. Es la analogía de una pila de platos: siempre se añade y se retira por la cima.

Esta implementación está en `GenericStack.h` y es completamente genérica gracias al uso de `template<typename T>`, lo que permite usarla con enteros, cadenas, objetos, etc.

---

## Características de esta implementación

- **Genérica** con `template<typename T>`.
- Estructura interna basada en **lista enlazada simple** (nodos con puntero `next`).
- Implementa **constructor de copia**, **operador de asignación (`operator=`)** y **destructor** para una gestión correcta de la memoria.
- Lanza excepciones de tipo `std::underflow_error` cuando se intenta operar sobre una pila vacía.

---

## Estructura interna

### `Node`

Cada elemento de la pila se almacena en un nodo:

```cpp
struct Node {
    T data;       // Valor almacenado
    Node* next;   // Puntero al siguiente nodo (más abajo en la pila)

    Node(const T& value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
};
```

### Atributos de `GenericStack`

| Atributo | Tipo | Descripción |
|----------|------|-------------|
| `topNode_` | `Node*` | Puntero a la cima de la pila (el último elemento insertado) |
| `size_` | `size_t` | Número de elementos actuales en la pila |

---

## Métodos implementados

### Públicos

| Método | Descripción |
|--------|-------------|
| `GenericStack()` | Constructor por defecto. Inicializa `topNode_` a `nullptr` y `size_` a 0. |
| `GenericStack(const GenericStack& other)` | Constructor de copia. Crea una copia profunda. |
| `operator=(const GenericStack& other)` | Operador de asignación. Libera la pila actual y hace copia profunda. |
| `~GenericStack()` | Destructor. Llama a `freeStack()` para liberar todos los nodos. |
| `push(const T& item)` | Inserta un elemento en la cima. |
| `pop()` | Elimina el elemento de la cima. Lanza `std::underflow_error` si la pila está vacía. |
| `top()` | Devuelve una referencia al elemento de la cima. Lanza `std::underflow_error` si la pila está vacía. |
| `empty()` | Devuelve `true` si la pila está vacía. |
| `size()` | Devuelve el número de elementos. |
| `print()` | Imprime la pila desde la cima hasta el fondo. |

### Privados

| Método | Descripción |
|--------|-------------|
| `freeStack()` | Libera todos los nodos de la pila e inicializa `topNode_` a `nullptr` y `size_` a 0. |
| `copy(const GenericStack& other)` | Copia todos los nodos de `other` manteniendo el orden original de la pila. |

---

## Funcionamiento con punteros

### Estado inicial (pila vacía)

```
topNode_ → nullptr
```

### Después de `push(1)`, `push(10)`, `push(5)`, `push(67)`

Cada `push` crea un nodo nuevo que apunta al antiguo `topNode_` y actualiza `topNode_`:

```
topNode_ → [67] → [5] → [10] → [1] → nullptr
             ↑ cima                    ↑ fondo
```

### `push(item)` – insertar en la cima

```cpp
topNode_ = new Node(item, topNode_);
++size_;
```

El constructor de `Node` recibe el valor nuevo y el antiguo `topNode_` como `next`. Así el nuevo nodo queda en la cima y el resto de la cadena se conserva.

### `pop()` – eliminar la cima

```cpp
Node* aux = topNode_;      // 1. Guardamos puntero a la cima actual
topNode_ = topNode_->next; // 2. La nueva cima es el siguiente nodo
delete aux;                // 3. Liberamos la memoria del nodo eliminado
--size_;
```

**¿Por qué guardar `aux`?**  
Si hiciéramos `delete topNode_` directamente perderíamos el puntero a `next` y no podríamos actualizar `topNode_`. Por eso siempre se guarda el nodo temporal antes de redirigir el puntero.

Antes del `pop`:
```
topNode_ → [67] → [5] → [10] → [1] → nullptr
```

Después del `pop`:
```
topNode_ → [5] → [10] → [1] → nullptr
```

El nodo `[67]` ha sido eliminado con `delete aux`.

### `print()` – recorrer la pila

```cpp
Node* current = topNode_;
while (current != nullptr) {
    std::cout << current->data;
    current = current->next;
}
```

- `current` empieza apuntando a la cima (`topNode_`).
- En cada iteración avanzamos con `current = current->next`.
- El bucle termina cuando `current` llega a `nullptr` (fin de la cadena).

---

## Copia y gestión de memoria

### `freeStack()` – destrucción / liberación

Recorre la lista liberando cada nodo:

```cpp
while (topNode_ != nullptr) {
    Node* aux = topNode_;
    topNode_ = topNode_->next;
    delete aux;
}
size_ = 0;
```

### `copy(other)` – copia profunda

Para copiar la pila manteniendo el mismo orden de cima a fondo, el método:
1. Recorre `other` de cima a fondo y guarda los valores en un array temporal.
2. Inserta los valores desde el fondo hacia la cima usando `push`.

Esto garantiza que la copia tenga sus propios nodos en memoria y que modificar una pila no afecte a la otra.

### Constructor de copia y `operator=`

```cpp
GenericStack<int> stack2(stack);   // Constructor de copia
GenericStack<int> stack3;
stack3 = stack;                    // Operador de asignación
```

Ambas operaciones hacen una **copia profunda** (*deep copy*): se crean nodos nuevos e independientes, no se comparten punteros.

---

## Compilación y ejecución

Desde la raíz del repositorio:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./Stack
```

---

## Ejemplo de salida esperada

```
########## PILA ORIGINAL ##########

Cima -> 67 | 5 | 10 | 1 <- Fondo
########## PILA CONSTRUCTOR DE COPIA ##########

Cima -> 67 | 5 | 10 | 1 <- Fondo
########## PILA OPERADOR = ##########

Cima -> 67 | 5 | 10 | 1 <- Fondo

EJECUTADA SENTENCIA POP SOBRE PRIMERA PILA

Nueva cima original tras pop: 5
Cima copia (independiente): 67
```

---

## Notas

- `pop()` y `top()` lanzan `std::underflow_error` si la pila está vacía.
- La copia profunda garantiza que `stack2` y `stack3` sean completamente independientes de `stack`; modificar una no altera las demás.
