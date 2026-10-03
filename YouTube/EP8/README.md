# P0_T1 — Vectores en un fichero binario

## Enunciado

Se dispone de un fichero binario que contiene dos vectores de datos almacenados consecutivamente. El tamaño del fichero es de 400 bytes. El programa trabajará con dos vectores, `V1` y `V2`, que tendrán el mismo tamaño.

### Preguntas sobre el fichero

1. ¿Cuántos bytes corresponden a `V1` y a cuántos a `V2`?
2. ¿Cuál es la cantidad máxima de datos de `V1` y de `V2`?
3. Suponiendo que el fichero almacena enteros, ¿cuál es la cantidad máxima de enteros de `V1` y de `V2`?
4. ¿Cuál sería el tamaño del fichero si, en lugar de ser binario, fuera de tipo texto? ¿Puede determinarse?
5. Si el fichero de 400 bytes es de tipo texto, ¿podemos determinar únicamente a partir de su tamaño cuántos enteros contiene?

La cantidad de datos enteros que se deben leer se indicará mediante un argumento en la línea de ejecución del programa. Dicha cantidad será como máximo de 50.

### Preguntas sobre lectura y memoria

1. ¿Cómo podemos saber exactamente cuántos elementos tenemos que leer?
2. ¿Qué tipo de memoria es preferible utilizar, estática o dinámica?
3. Si el número de elementos se almacena en la variable `tam`, ¿qué diferencias existen entre estas reservas?

   - `int v[tam]`
   - `int *v = malloc(tam * sizeof(int))`

Una vez leídos los dos vectores, que tienen el mismo número de elementos, hay que compararlos posición a posición y contar cuántas posiciones cumplen que el elemento de `V1` es estrictamente mayor que el elemento correspondiente de `V2`.

La comparación se plantea de dos formas: con aritmética de punteros y sin usar `[]`, y con índices y corchetes `[]`.

## Respuestas explicadas

### 1. ¿Cuántos bytes corresponden a cada vector?

Como los dos vectores tienen el mismo tamaño y comparten los 400 bytes del fichero, se divide el tamaño total entre dos:

- `V1`: 200 bytes.
- `V2`: 200 bytes.

Esto supone que los 400 bytes contienen únicamente los datos de los vectores y que no hay una cabecera u otra información adicional.

### 2. ¿Cuál es la cantidad máxima de datos de cada vector?

Cada vector puede almacenar tantos elementos como quepan en sus 200 bytes. Para saber el número exacto hace falta conocer el tamaño de cada elemento. La fórmula es:

> elementos por vector = bytes del vector / bytes por elemento

Por tanto, sin especificar el tipo de dato o su tamaño, no se puede dar una cantidad exacta de elementos.

### 3. ¿Cuántos enteros caben en cada vector?

Depende del tamaño de `int` en la máquina que crea y lee el fichero. En el fichero proporcionado, los valores se corresponden con enteros de 4 bytes. Bajo esa suposición:

> 200 bytes / 4 bytes por entero = 50 enteros por vector

Así que caben como máximo 50 enteros en `V1` y 50 en `V2` (100 en total). En C, el tamaño local de `int` se consulta con `sizeof(int)`. Para que un formato binario sea portable entre distintas máquinas también habría que fijar el tamaño y el orden de bytes.

### 4. ¿Qué tamaño tendría un fichero de texto equivalente¿

No se puede determinar un tamaño único con la información del enunciado. En un fichero de texto los enteros se representan con caracteres: por ejemplo, `-27 104 8`. La cantidad de caracteres depende del valor de cada entero, de si tiene signo negativo y de los separadores o saltos de línea que se escriban. En cambio, en el fichero binario de esta práctica cada entero ocupa 4 bytes.

### 5. ¿Podemos saber cuántos enteros contiene un texto de 400 bytes?

No. El tamaño por sí solo no basta, porque cada entero puede ocupar una cantidad diferente de caracteres. Por ejemplo, `1 2 3` contiene tres enteros, mientras que `100000 200000` contiene dos, y sus longitudes dependen también de los espacios usados. Habría que leer y contar los números separados según el formato del texto.

### 6. ¿Cómo sabemos cuántos elementos leer?

El usuario proporciona la cantidad como argumento al ejecutar el programa. En C, los argumentos se reciben en `argv`; `argv[1]` contiene el primer argumento que escribe el usuario (mientras que `argv[0]` suele ser el nombre del programa). Ese texto se convierte a entero y se comprueba que está entre 1 y 50.

La cantidad indicada se aplica a cada vector: se leen `tam` enteros para `V1` y otros `tam` para `V2`. Por ejemplo, si `tam` vale 10, se leen 20 enteros en total.

### 7. ¿Es preferible memoria estática o dinámica?

En esta práctica la reserva debe hacerse con memoria dinámica. Como `tam` se conoce al ejecutar el programa, se reserva espacio para exactamente `tam` enteros para cada vector. Así cada vector utiliza solo la memoria que necesita. Hay que comprobar que la reserva se realizó correctamente y liberar cada bloque cuando ya no se use. Aunque el máximo sea 50, se debe seguir el requisito de la práctica y no sustituirlo por arrays de tamaño fijo.

### 8. Diferencia entre `int v[tam]` y reservar con `malloc`

`int v[tam]` crea un array de longitud variable cuyo tamaño se decide al llegar a esa declaración y no puede cambiar después. Su memoria deja de estar disponible al salir del bloque donde se declaró y no se libera con `free`. Aunque el valor de `tam` se conozca en ejecución, esta declaración no realiza la reserva dinámica solicitada en la práctica, así que no es la opción que debe usarse aquí. Además, el soporte de arrays de longitud variable depende de la versión y del compilador de C.

`int *v = malloc(tam * sizeof(int))` solicita memoria dinámicamente. Si la reserva funciona, `v` apunta al primer elemento; si falla, `malloc` devuelve `NULL`. La reserva permanece hasta que se libera con `free(v)`. Se puede escoger el tamaño a partir de `tam`, y es necesario gestionar tanto el posible fallo como la liberación.

En las dos formas los elementos pueden recorrerse mediante índices. La diferencia está en cómo se reserva y cuánto dura la memoria, no en cómo se comparan los valores.

### 9. ¿Qué significa comparar posición a posición?

Se recorren las posiciones desde la primera hasta la última de los `tam` elementos. En cada posición se compara `V1` con `V2` en ese mismo lugar. Si el valor de `V1` es mayor, se incrementa un contador. Al final, ese contador indica cuántas posiciones cumplen la condición y estará entre 0 y `tam`.

«Estrictamente mayor» significa que un empate no cuenta: si ambos valores son iguales, esa posición no incrementa el contador.

Con índices se identifica una posición escribiendo `V1[i]` y `V2[i]`. Con aritmética de punteros se parte de la dirección inicial y se avanza hasta la posición deseada; conceptualmente, `V1[i]` equivale a `*(V1 + i)`. Avanzar un puntero una unidad significa avanzar un elemento del tipo apuntado, no un byte.


