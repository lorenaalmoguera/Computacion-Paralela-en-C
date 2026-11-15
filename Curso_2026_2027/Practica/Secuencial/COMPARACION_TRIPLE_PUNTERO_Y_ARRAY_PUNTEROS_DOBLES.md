# Prácticas 7, 8 y 9: triple puntero frente a array de punteros dobles

## 1. Las dos soluciones representan lo mismo

Nuestra solución utiliza una tabla dinámica de submatrices:

```c
unsigned char ***submatrices =
    malloc((size_t)div * sizeof *submatrices);
```

La alternativa indicada por el profesor utiliza un array cuyos elementos son punteros dobles:

```c
unsigned char **submatrices[5] = {NULL};
```

Cada elemento `submatrices[s]` es de tipo `unsigned char **` y representa una submatriz. En ambas versiones se accede a un elemento con:

```c
submatrices[s][i][j]
/* submatriz s, fila local i, columna local j */
```

**La diferencia está en cómo se almacena la tabla exterior de submatrices.** No cambia el tipo de cada submatriz ni obliga a cambiar los bucles de copia o de cálculo.

El tamaño 5 responde al límite de los enunciados: se utilizan únicamente las primeras `div` entradas, con `2 <= div && div <= 5`. El resto permanece a `NULL` y no se utiliza.

## 2. Qué significa cada declaración

### Nuestra versión: un puntero triple

```c
unsigned char ***submatrices;
```

Se lee: «`submatrices` es un puntero a un puntero a un puntero a `unsigned char`».

Después de reservar las tablas y las filas:

| Expresión | Tipo | Qué representa |
|---|---|---|
| `submatrices` | `unsigned char ***` | Puntero a la tabla dinámica exterior. |
| `submatrices[s]` | `unsigned char **` | Puntero a la tabla de filas de la submatriz `s`. |
| `submatrices[s][i]` | `unsigned char *` | Puntero a la fila `i` de esa submatriz. |
| `submatrices[s][i][j]` | `unsigned char` | Valor de una celda. |

El triple puntero no reserva por sí solo ninguna tabla ni ningún dato. En nuestros programas las reservas se hacen con `malloc`, nivel a nivel.

### Alternativa: un array de punteros dobles

```c
unsigned char **submatrices[5] = {NULL};
```

Se lee desde el nombre: «`submatrices` es un array de cinco elementos; cada elemento es un puntero a un puntero a `unsigned char`».

| Expresión | Tipo | Qué representa |
|---|---|---|
| `submatrices` como objeto declarado | Array de 5 elementos `unsigned char **` | La propia tabla exterior. |
| `submatrices[s]` | `unsigned char **` | Puntero a la tabla de filas de la submatriz `s`. |
| `submatrices[s][i]` | `unsigned char *` | Puntero a una fila. |
| `submatrices[s][i][j]` | `unsigned char` | Valor de una celda. |

Dentro de `main`, ese array tiene duración automática: existe hasta salir del bloque donde se declara. Las tablas de filas y las filas siguen reservándose dinámicamente.

**No es `unsigned char *submatrices[5]`**: eso sería un array de punteros simples y no conservaría esta representación de cada submatriz mediante una tabla de filas.

## 3. Diagrama de memoria

```text
NUESTRA SOLUCIÓN

submatrices: variable de tipo unsigned char ***
     │
     ▼
Tabla exterior dinámica: div entradas de tipo unsigned char **
     │ submatrices[s]
     ▼
Tabla dinámica de filas de la submatriz s: entradas unsigned char *
     │ submatrices[s][i]
     ▼
Fila dinámica: [dato 0][dato 1][dato 2]...


ALTERNATIVA DEL PROFESOR

submatrices: array local de 5 entradas de tipo unsigned char **
     │ submatrices[s]
     ▼
Tabla dinámica de filas de la submatriz s: entradas unsigned char *
     │ submatrices[s][i]
     ▼
Fila dinámica: [dato 0][dato 1][dato 2]...
```

La segunda versión elimina **una reserva dinámica**, la de la tabla exterior. Conserva las reservas dinámicas de las tablas de filas y de los datos.

Hay tres índices en las dos soluciones porque seguimos seleccionando submatriz, fila y columna. El número de índices no exige que la variable exterior se declare necesariamente como un puntero triple.

## 4. Qué cambia exactamente en nuestros programas

### Paso 1: cambiar la declaración exterior

Antes:

```c
unsigned char ***submatrices =
    malloc((size_t)div * sizeof *submatrices);
```

Después:

```c
unsigned char **submatrices[5] = {NULL};
```

La comprobación de `div` debe hacerse antes de dividir por él y antes de utilizarlo para recorrer el array:

```c
if (div < 2 || div > 5) {
    fprintf(stderr, "El numero de submatrices debe estar entre 2 y 5.\n");
    return 1;
}
```

### Paso 2: conservar las reservas interiores

No se eliminan estas reservas:

```c
submatrices[s] = malloc((size_t)filas * sizeof *submatrices[s]);

/* Para cada fila i de esta submatriz: */
submatrices[s][i] =
    malloc((size_t)columnas * sizeof *submatrices[s][i]);
```

En las prácticas 7 y 8, `filas` varía entre submatrices y `columnas` vale `dim`. En la 9, `filas` vale `dim` y `columnas` varía entre submatrices.

### Paso 3: conservar accesos, copias y cálculos

Estas expresiones no cambian:

```c
memcpy(submatrices[s][i], origen, bytes);
unsigned char valor = submatrices[s][i][j];
free(submatrices[s][i]);
free(submatrices[s]);
```

### Paso 4: quitar únicamente la liberación exterior

En nuestra versión se hace:

```c
free(submatrices);
```

Con el array local **se elimina esa llamada**. El array no fue creado por `malloc` y no se pasa a `free`. Las filas y las tablas de filas sí se liberan porque continúan siendo dinámicas.

Los fragmentos de este documento explican la representación; no constituyen programas completos. Para usarlos en una versión completa hay que comprobar cada reserva y lectura y liberar lo ya reservado si se produce un error.

## 5. Aplicación a la práctica 7

Archivo de referencia: [P0_T7_lorena.c](P0-7/P0_T7_lorena.c). [Enunciado](P0-7/README.md).

La matriz se divide en **bloques de filas consecutivas**. Cada submatriz tiene `dim` columnas. Después se calcula el máximo de cada fila y se guarda en `MaxFila`, respetando el orden de filas de la matriz original.

El reparto de las filas se calcula igual en ambas versiones:

```c
int filas_base = dim / div;
int resto = dim % div;

/* Para la submatriz s: */
int filas = filas_base + (s < resto);
```

Con `dim=512` y `div=3`, hay 171, 171 y 170 filas.

La reserva y copia con el array exterior quedan así, suponiendo que `MatTotal` ya está leída y que las reservas tienen éxito:

```c
unsigned char **submatrices[5] = {NULL};
int filaoriginal = 0;

for (int s = 0; s < div; ++s) {
    int filas = filas_base + (s < resto);
    submatrices[s] = malloc((size_t)filas * sizeof *submatrices[s]);

    for (int i = 0; i < filas; ++i) {
        submatrices[s][i] =
            malloc((size_t)dim * sizeof *submatrices[s][i]);

        memcpy(submatrices[s][i], MatTotal[filaoriginal],
               (size_t)dim * sizeof *submatrices[s][i]);
        ++filaoriginal;
    }
}
```

El cálculo de `MaxFila` conserva los bucles `s → i → j`. La correspondencia entre fila original y fila local tampoco cambia:

```text
div=3

Submatriz 0: filas originales   0..170 → filas locales 0..170
Submatriz 1: filas originales 171..341 → filas locales 0..170
Submatriz 2: filas originales 342..511 → filas locales 0..169
```

En el código actual de la práctica 7 falta la validación de `div` que sí aparece en las versiones 8 y 9. Al utilizar la alternativa del array hay que añadirla; también es necesaria en la versión con triple puntero para respetar el enunciado y evitar dividir por cero.

## 6. Aplicación a la práctica 8

Archivo de referencia: [P0_T8_lorena.c](P0-8/P0_T8_lorena.c).

En nuestra implementación la división sigue siendo **por filas**, igual que en la práctica 7. Lo que cambia es el resultado: se calcula el máximo de cada **columna** de la matriz original.

Por tanto, declaración, reserva y copia de las submatrices pueden ser las mismas del apartado anterior. El cálculo debe recorrer una misma columna a través de todas las submatrices:

```c
for (int j = 0; j < dim; ++j) {
    unsigned char maximo = 0;

    for (int s = 0; s < div; ++s) {
        int filas = filas_base + (s < resto);
        for (int i = 0; i < filas; ++i) {
            if (submatrices[s][i][j] > maximo)
                maximo = submatrices[s][i][j];
        }
    }
    MaxColumna[j] = maximo;
}
```

Inicializar a cero es válido aquí porque el tipo es `unsigned char`: ninguno de sus valores es negativo. No hay que trasladar esa inicialización sin pensar a una matriz de enteros con valores negativos.

El array exterior no convierte la práctica 8 en una división por columnas: el reparto lo deciden las reservas y los desplazamientos de copia, no el número de asteriscos de la declaración exterior.

## 7. Aplicación a la práctica 9

Archivo de referencia: [P0-0_Lorena.c](P0-9/P0-0_Lorena.c), nombre actual del programa de la práctica 9. [Enunciado](P0-9/README.md).

Aquí sí se divide en **bloques de columnas consecutivas**. Todas las submatrices tienen `dim` filas y cada una tiene un ancho calculado según el reparto:

```c
int columnas_base = dim / div;
int resto = dim % div;
```

La reserva y copia con el array exterior quedan así, bajo las mismas precondiciones de los fragmentos anteriores:

```c
unsigned char **submatrices[5] = {NULL};
int inicio_columna = 0;

for (int s = 0; s < div; ++s) {
    int columnas = columnas_base + (s < resto);
    submatrices[s] = malloc((size_t)dim * sizeof *submatrices[s]);

    for (int i = 0; i < dim; ++i) {
        submatrices[s][i] =
            malloc((size_t)columnas * sizeof *submatrices[s][i]);

        memcpy(submatrices[s][i], MatTotal[i] + inicio_columna,
               (size_t)columnas * sizeof *submatrices[s][i]);
    }
    inicio_columna += columnas;
}
```

`inicio_columna` es un desplazamiento en la fila original, no un número de fila. Con `div=3`:

```text
Cada fila de MatTotal:

[ columnas 0..170 | columnas 171..341 | columnas 342..511 ]
       │                   │                    │
       ▼                   ▼                    ▼
 submatriz 0         submatriz 1          submatriz 2
 ancho 171           ancho 171            ancho 170
 512 filas           512 filas            512 filas
```

En el cálculo de máximos, el índice de columna local se transforma en índice global así:

```c
int j_global = inicio_columna + j_local;
MaxColumna[j_global] = maximo;
```

Cada columna completa pertenece a una única submatriz, a diferencia de la práctica 8, donde una columna está repartida por sus bloques de filas.

## 8. Liberación correcta en cada práctica

Para las prácticas 7 y 8, con el array de punteros dobles:

```c
for (int s = 0; s < div; ++s) {
    int filas = filas_base + (s < resto);
    for (int i = 0; i < filas; ++i)
        free(submatrices[s][i]);
    free(submatrices[s]);
}
/* No se hace free(submatrices). */
```

Para la práctica 9:

```c
for (int s = 0; s < div; ++s) {
    for (int i = 0; i < dim; ++i)
        free(submatrices[s][i]);
    free(submatrices[s]);
}
/* No se hace free(submatrices). */
```

La liberación de `MatTotal`, `MaxFila` o `MaxColumna` permanece igual. Primero se copia a almacenamiento independiente y después se libera `MatTotal`.

Hacer simplemente `submatrices[s][i] = MatTotal[filaoriginal]` no copia los datos. Si después se libera esa fila de `MatTotal`, el puntero queda colgando. Ninguna de las dos declaraciones exteriores resuelve ese problema: las prácticas piden reservar y copiar.

## 9. Contigüidad y número de llamadas a memcpy

En nuestros tres programas, `MatTotal` se reserva **fila a fila**. Las filas no tienen por qué ser consecutivas en memoria. Las submatrices también reservan cada fila por separado.

Cambiar `unsigned char ***` por `unsigned char **[5]` no cambia ese almacenamiento.

| Práctica | División en nuestro código | Región contigua que se copia en cada llamada | Llamadas con esta representación |
|---|---|---|---:|
| 7 | Filas | Una fila completa. | `dim`, es decir, 512. |
| 8 | Filas | Una fila completa. | `dim`, es decir, 512. |
| 9 | Columnas | Un segmento de una fila para una submatriz. | `dim * div`, entre 1024 y 2560. |

No se puede copiar todo un bloque de varias filas con una sola llamada suponiendo contigüidad que no se ha reservado. Para reducir llamadas mediante bloques contiguos habría que cambiar también la organización de los datos y revisar origen y destino. Es una decisión diferente de cambiar la tabla exterior.

Estas cantidades corresponden al almacenamiento actual y a las copias directas a las submatrices, sin introducir empaquetado temporal. No son un mínimo universal para cualquier representación posible.

## 10. Comparación de memoria y duración

| Aspecto | Triple puntero y tabla exterior dinámica | Array local de punteros dobles |
|---|---|---|
| Declaración | `unsigned char ***submatrices` | `unsigned char **submatrices[5]` |
| Entradas exteriores reservadas | `div`, mediante `malloc`. | 5, como parte del array local. |
| Duración de la tabla exterior | Hasta liberarla con `free`. | Hasta salir del bloque donde se declara. |
| Tablas de filas y datos | Dinámicos. | Dinámicos. |
| Acceso a una celda | `submatrices[s][i][j]` | `submatrices[s][i][j]` |
| Liberación exterior | `free(submatrices)`. | No se hace. |
| Cambio del número de entradas | Puede reservarse otra tabla o redimensionarla. | Limitado a la capacidad declarada. |
| Copias y cálculo de máximos | Los de cada práctica. | Los mismos. |

Si `P = sizeof(unsigned char **)`, nuestra tabla exterior dinámica almacena `div * P` bytes de entradas, además de la variable puntero exterior y posibles costes del asignador. La tabla local fija almacena `5 * P` bytes de entradas. El resto de las reservas es el mismo. No se debe afirmar que el array fijo siempre consume menos memoria total: elimina una reserva, pero puede tener entradas sin usar.

Contando **solo las llamadas a malloc para construir submatrices**, y suponiendo éxito:

| Práctica | Nuestra versión | Array exterior local |
|---|---:|---:|
| 7 y 8 | `1 + div + 512` | `div + 512` |
| 9 | `1 + div + 512 * div` | `div + 512 * div` |

El primer `1` es la tabla exterior. Las reservas de `MatTotal` y del vector de máximos quedan fuera de este recuento. El coste principal de datos y recorridos permanece igual; quitar una reserva no garantiza una mejora apreciable de tiempo.

## 11. Por qué el array puede pasarse a una función con triple puntero

En la mayoría de las expresiones, el nombre de un array se convierte en un puntero a su primer elemento. Como los elementos del array son `unsigned char **`, el puntero al primero es `unsigned char ***`.

Por eso una función que recorre las submatrices puede aceptar:

```c
void mostrar(unsigned char ***submatrices, int div);
```

Y puede llamarse desde ambas soluciones:

```c
mostrar(submatrices, div);
```

Esto no transforma el objeto array local en una reserva dinámica. La función debe conocer el número de entradas y no debe liberar la tabla exterior si se le pasó el array local. Dentro de la función, el parámetro no conserva información automática de la longitud original.

También puede escribirse un parámetro así:

```c
void mostrar(unsigned char **submatrices[], int div);
```

En una declaración de parámetro, C ajusta esa notación de array a un puntero: ambas firmas anteriores expresan el mismo tipo de parámetro.

En cambio, en el bloque donde se declaró el array, `sizeof` permite ver la diferencia:

```c
unsigned char **tabla[5];
sizeof tabla;       /* 5 * sizeof(unsigned char **) */

unsigned char ***p;
sizeof p;           /* Tamaño del propio puntero triple. */
```

No son objetos del mismo tipo, aunque puedan usarse de forma parecida al indexarlos o pasarlos a una función.

## 12. ¿Y si se declara el array con div en lugar de 5?

Tras validar `div`, algunos compiladores de C permiten un array de longitud variable o VLA:

```c
unsigned char **submatrices[div];
```

También conserva las reservas interiores y tampoco se libera con `free`. Su soporte depende del estándar y del compilador; el array fijo de cinco entradas evita esa dependencia en estas prácticas. No se inicializa un VLA con `{NULL}` como el array fijo: si se necesita inicialización, se recorre y se asigna `NULL` a cada entrada.

El tamaño del array se fija al entrar en esa declaración: cambiar después la variable `div` no redimensiona el array ya creado.

## 13. Qué solución encaja con estas prácticas

El array fijo de cinco punteros dobles encaja con el límite conocido de dos a cinco submatrices y simplifica la gestión de la tabla exterior. Nuestra representación con triple puntero también permite expresar correctamente la misma estructura y ajustar esa tabla al número solicitado.

Los requisitos de reserva dinámica de las submatrices siguen cumpliéndose con el array exterior local: sus tablas de filas y sus datos se reservan con `malloc`. Lo que deja de ser dinámico es únicamente el contenedor exterior.

Para adaptar los programas actuales basta con cambiar la declaración exterior, mantener las reservas interiores y los cálculos, validar `div` y eliminar el `free` exterior. No se han modificado los programas `.c`: este documento permite comparar las dos soluciones antes de elegir una.
