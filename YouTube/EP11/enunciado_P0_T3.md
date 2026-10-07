# P0_T3 — Punteros y aritmética de punteros

## Enunciado

Se dispone de un fichero binario que contiene datos almacenados consecutivamente. El tamaño de ese fichero es de 400 bytes. El programa deberá trabajar con un solo vector de 100 enteros, `V1`.

Muestra por pantalla los 4 primeros enteros (posiciones 0, 1, 2 y 3).

Haz que un puntero de tipo `char* pcharV1` apunte al comienzo de la zona de memoria ocupada por el vector, y pon a 0 el primer elemento. Muestra el nuevo valor del primer elemento apuntado por `pcharV1` y el primer elemento de `V1`.

Haz que un puntero de tipo `int* pintV1` apunte al comienzo de la zona de memoria ocupada por el vector, y pon a 0 el primer elemento. Muestra el nuevo valor del primer elemento apuntado por `pintV1` y de `V1`.

Haz que un puntero de tipo `double* pdoubleV1` apunte al comienzo de la zona de memoria ocupada por el vector, y pon a 0 el primer elemento. Muestra el nuevo valor del primer elemento apuntado por `pdoubleV1` y de `V1`.

## Preguntas y respuestas

### 1. ¿Cuántos bytes ocupa cada elemento del vector?

En este ejercicio, 100 enteros ocupan 400 bytes, así que cada elemento ocupa 4 bytes (`400 / 100 = 4`). En C, el tamaño de `int` puede depender de la plataforma; se consulta con `sizeof(int)`. Aquí se asume que el formato del fichero y la máquina usan enteros de 4 bytes.

### 2. ¿Cuántos bytes se avanza cuando sumamos 1 a un puntero?

Se avanza el tamaño del tipo al que apunta el puntero, es decir, `sizeof(*puntero)` bytes. Por ejemplo, si `pintV1` es un `int*` y `sizeof(int)` vale 4, `pintV1 + 1` avanza 4 bytes hasta el siguiente `int`. Si es un `char*`, normalmente avanza 1 byte porque `sizeof(char)` siempre vale 1 byte en C.

### 3. ¿Cuántos bytes se avanza cuando sumamos 1 a un puntero tipo `void*`?

En C estándar no se puede hacer aritmética directamente con `void*`, porque `void` no tiene tamaño definido. Algunos compiladores, como GCC en ciertos modos, aceptan `void* + 1` como extensión y lo interpretan como un avance de un byte, pero ese comportamiento no es portable. Para avanzar byte a byte se convierte a `char*` (o `unsigned char*`) y se suma sobre ese puntero.

### 4. ¿Qué relación existe entre `pintV1[3]` y `*(pintV1 + 3)`?

Son expresiones equivalentes: ambas acceden al valor del cuarto entero contado desde `pintV1` (índice 3). En C, la indexación se define en términos de aritmética de punteros: `pintV1[3]` equivale a `*(pintV1 + 3)`.

### 5. ¿Qué relación existe entre `nuevopintV1 = &pintV1[3]` y `nuevopintV1 = pintV1 + 3`?

Si `nuevopintV1` es de tipo `int*`, las dos asignaciones producen la misma dirección: la del elemento con índice 3. La primera obtiene la dirección con `&`; la segunda llega a esa dirección avanzando tres elementos desde `pintV1`.

### 6. ¿Cuál es el comportamiento exacto de `+`?

Depende de los operandos. Con números, `+` realiza una suma numérica. Al sumar un entero a un puntero a un tipo completo, el puntero avanza esa cantidad de elementos, no de bytes: `p + n` señala el elemento `n` posiciones después de `p`, con un desplazamiento de `n * sizeof(*p)` bytes.

La aritmética de punteros está definida dentro del mismo array y también permite formar una dirección justo después del último elemento, aunque no se puede desreferenciar esa dirección. No se debe avanzar fuera de esos límites. La suma de dos punteros no está permitida.

## Nota

Las tareas de mostrar los valores y modificar `V1` descritas en el enunciado son el ejercicio de programación; este documento solo responde las preguntas conceptuales.
