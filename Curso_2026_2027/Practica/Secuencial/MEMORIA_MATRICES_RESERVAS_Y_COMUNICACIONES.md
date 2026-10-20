# Memoria de una matriz: reservas por filas, bloque contiguo y comunicaciones

## 1. Qué está pidiendo el profesor en estas prácticas

Según la explicación recogida en clase, cuando pide almacenar la matriz mediante un puntero doble, **quiere que utilicemos una tabla de filas y una reserva independiente para los datos de cada fila**.

En estas prácticas, por tanto, no basta con construir un `int **` que apunte a un único bloque de datos contiguo: aunque permita escribir `matriz[i][j]`, esa organización no es la estrategia que está pidiendo.

Esta es una condición de la práctica. En el lenguaje C, el tipo `int **` por sí solo no determina si los datos de las filas están en reservas independientes o en un bloque único. Hay que mirar cómo se reserva la memoria y cómo se rellenan los punteros.

En los ejemplos siguientes se usa `int` para relacionarlos con la expresión anotada en clase. En nuestras prácticas 7, 8 y 9, los datos son `unsigned char`; se aplica la misma estructura, utilizando el tipo y los tamaños correspondientes.

## 2. Qué reserva el malloc que aparece dentro del for

El esquema indicado por el profesor es:

```c
int **matriz = malloc((size_t)nf * sizeof *matriz);

for (int i = 0; i < nf; ++i) {
    matriz[i] = malloc((size_t)nc * sizeof *matriz[i]);
}
```

Los fragmentos explican la organización de la memoria; no son programas completos. Suponen `nf > 0`, `nc > 0`, tamaños representables y reservas correctas. En un programa completo se comprueba cada `malloc` y se libera lo ya reservado si una reserva falla.

Hay dos clases de reserva:

| Reserva | Qué almacena | Cantidad |
|---|---|---:|
| `malloc(nf * sizeof *matriz)` | La tabla de direcciones de las filas. | `nf` punteros de tipo `int *`. |
| `malloc(nc * sizeof *matriz[i])` | Los datos de una fila. | `nc` elementos de tipo `int`. |

**Sí: los `malloc` del bucle reservan el espacio de los datos que vamos a procesar.** La reserva exterior almacena las direcciones necesarias para localizar esos datos.

En cada iteración:

1. `malloc` reserva espacio para los `nc` elementos de una fila.
2. Devuelve la dirección inicial de ese espacio.
3. Esa dirección se guarda en `matriz[i]`.

Reservar no equivale a leer ni inicializar los datos. Antes de calcular con ellos hay que cargarlos desde el fichero, copiarlos o asignarles valores.

## 3. Cuántos malloc se ejecutan

Si todas las reservas tienen éxito y el bucle se completa:

```text
Dentro del for:             nf llamadas a malloc
Tabla de punteros:           1 llamada a malloc
Total de esta matriz:   nf + 1 llamadas a malloc
```

Con una matriz de 512 filas son **512 reservas de filas más una reserva de la tabla: 513 llamadas**.

Si la tabla exterior se declarase como un array local de punteros, esa tabla no necesitaría `malloc`, pero las reservas de las filas continuarían existiendo. Esta observación no cambia el requisito concreto de utilizar la representación solicitada en el enunciado.

Una llamada a `malloc` tampoco equivale necesariamente a una petición nueva al sistema operativo: el asignador puede gestionar internamente memoria ya obtenida. Aquí contamos llamadas de nuestro programa y bloques asignados, no peticiones al sistema.

## 4. Cómo quedan las direcciones de las filas

Utilizamos los mismos seis valores para visualizar una matriz de 2 filas y 3 columnas:

```text
Matriz lógica:

       columna 0   columna 1   columna 2
fila 0      1          23           2
fila 1     15           7           8
```

Con reservas independientes:

```text
matriz: int **
    │
    ▼
Tabla de punteros                 Reservas de datos
┌────────────────┐
│ matriz[0] ──────┼──────────────> [ 1 ][23 ][ 2 ]
├────────────────┤
│ matriz[1] ──────┼──────> [15 ][ 7 ][ 8 ]
└────────────────┘

Cada fila es contigua internamente.
No se garantiza que una fila continúe donde termina la anterior.
```

Las filas pueden estar en direcciones muy distintas. Aunque dos reservas aparezcan cerca por casualidad, el programa no puede tratarlas como un único array de `nf * nc` elementos.

El acceso:

```c
matriz[i][j]
```

equivale a:

```c
*(*(matriz + i) + j)
```

Primero se consulta el puntero de la fila `i`; después se avanza `j` elementos dentro de esa fila. Esa consulta permite que las filas estén en reservas independientes.

## 5. Alternativa: reservar todos los datos linealmente

También se puede representar una matriz lógica mediante un único array de datos:

```c
int *datos = malloc((size_t)nf * nc * sizeof *datos);
```

```text
Un único bloque de datos:

[ 1 ][23 ][ 2 ][15 ][ 7 ][ 8 ]
 └── fila 0 ──┘ └── fila 1 ──┘
 ^ datos
```

El acceso a la fila `i`, columna `j`, se calcula así:

```c
datos[(size_t)i * nc + j]
```

Esto **sí representa una matriz desde el punto de vista del problema**, pero el objeto de acceso es un puntero simple a datos lineales: no es un `int **` ni un array bidimensional declarado como tal en C.

La reserva no conserva por sí sola las dimensiones. El programa debe conocer `nf` y `nc` para interpretar las posiciones.

### Corrección de la expresión anotada

En la transcripción aparece una expresión de la forma:

```c
(int *)malloc(nf * nc * sizeof(int *))
```

Para reservar los **datos enteros**, corresponde `sizeof(int)`, no `sizeof(int *)`:

```c
int *datos = malloc((size_t)nf * nc * sizeof *datos);
```

`sizeof(int *)` es el tamaño de una dirección; `sizeof(int)` es el de un entero. No hay que suponer que coinciden. El cast `(int *)` no construye una matriz ni cambia el número de bytes que se ha pedido; en C no es necesario para el resultado de `malloc`.

Para tamaños grandes, convertir un operando a `size_t` evita hacer primero la multiplicación como `int`, pero **no evita todos los desbordamientos**: hay que comprobar también que el producto final cabe en `size_t` antes de reservar.

## 6. Bloque contiguo con una tabla de filas: también permite int **

Una vez reservado el bloque lineal, se puede construir una tabla que indique dónde empieza cada fila:

```c
int *datos = malloc((size_t)nf * nc * sizeof *datos);
int **matriz = malloc((size_t)nf * sizeof *matriz);

for (int i = 0; i < nf; ++i) {
    matriz[i] = datos + (size_t)i * nc;
}
```

Aquí el `for` **no llama a `malloc`**. Solo calcula y guarda direcciones dentro del bloque ya reservado.

```text
Tabla de filas                       Único bloque de datos

matriz[0] ────────────────────────> [ 1 ][23 ][ 2 ][15 ][ 7 ][ 8 ]
matriz[1] ────────────────────────────────────────^
                                    ^ datos
```

Se puede escribir:

```c
matriz[i][j]
```

y se accede al mismo elemento que con:

```c
datos[(size_t)i * nc + j]
```

**No necesitamos un puntero para cada columna.** La tabla guarda el comienzo de cada fila; dentro de la fila, las columnas se localizan sumando su índice. Eso funciona porque los elementos de cada fila son contiguos y conocemos su ancho.

En esta versión hay dos reservas: una para todos los datos y otra para la tabla de filas. Si la tabla fuese local, quedaría una única reserva dinámica para los datos.

### No confundir dos tablas diferentes

Para una matriz, la tabla de filas se maneja con `int **matriz` y **sus entradas son `int *`**.

En el documento anterior, `unsigned char **submatrices[5]` era un array **cuyas entradas son punteros dobles**, porque cada entrada identifica una matriz distinta. Es otro nivel de organización:

```text
Tabla de submatrices → tabla de filas de una submatriz → datos de una fila
```

## 7. Por qué esta segunda estrategia no es la pedida

En las dos representaciones con tabla de filas podemos escribir `matriz[i][j]`, pero la organización de los datos cambia:

| Aspecto | Una reserva por fila | Bloque contiguo con tabla de filas |
|---|---|---|
| Variable de acceso | `int **matriz`. | `int **matriz`, más `int *datos`. |
| Reservas de datos | `nf`. | 1. |
| Reservas de la tabla | 1 en el ejemplo dinámico. | 1 en el ejemplo dinámico. |
| Total de llamadas a `malloc` | `nf + 1`. | 2. |
| Dentro del `for` de construcción | Se reserva cada fila. | Se guarda dónde comienza cada fila. |
| Contigüidad garantizada | Dentro de cada fila. | En todos los datos. |
| Acceso a una celda | `matriz[i][j]`. | `matriz[i][j]`. |

Según la indicación que has recogido, **el profesor quiere la primera columna para estas prácticas**: datos reservados por filas, sin asumir continuidad entre ellas.

Por tanto, no debemos sustituirla por un bloque lineal y añadir después una tabla solo para conservar la sintaxis de doble índice. Que esa solución sea posible en C no significa que respete el objetivo concreto del ejercicio.

## 8. Relación entre reservas y mensajes MPI

La frase «tengo `nf` malloc, entonces tengo esa cantidad de mensajes» necesita una condición:

> Si el algoritmo decide enviar cada fila en una llamada independiente, realizará `nf` envíos para transmitir una vez las `nf` filas.

Por ejemplo, suponiendo MPI inicializado, un destino válido, un receptor preparado y datos ya cargados:

```c
for (int i = 0; i < nf; ++i) {
    MPI_Send(matriz[i], nc, MPI_INT, destino, etiqueta, comunicador);
}
```

Aquí hay `nf` llamadas a `MPI_Send` porque el programa las ha escrito dentro del bucle. Las llamadas a `malloc` **no generan mensajes MPI ni fijan automáticamente su cantidad**.

| Hecho | Qué determina |
|---|---|
| `malloc` | Qué memoria tiene disponible el proceso. |
| `MPI_Send`, `MPI_Recv` y colectivas | Qué comunicaciones solicita el programa. |
| Organización de los datos | Qué regiones pueden describirse directamente como un bloque contiguo. |

Una fila reservada no tiene por qué enviarse si solo se utiliza localmente. Un bloque contiguo también puede enviarse en varios mensajes. El número de paquetes de red internos tampoco tiene por qué coincidir con el de llamadas de comunicación.

## 9. Qué significa poder utilizar colectivas con la matriz

### 9.1. Datos contiguos: una descripción directa con un tipo básico

Con un bloque lineal de enteros, se puede describir toda la matriz mediante:

```c
MPI_Bcast(datos, nf * nc, MPI_INT, raiz, comunicador);
```

Todos los procesos deben llamar a la misma colectiva y tener un búfer válido con capacidad suficiente; la raíz debe tener los datos inicializados. Este ejemplo supone además que `nf * nc` cabe en el argumento `int` de cantidad de esta firma.

Si hay una tabla de filas apuntando a ese bloque, se pasa `datos` o, con `nf > 0`, `matriz[0]`, **no la dirección de la tabla `matriz`**.

La contigüidad también facilita repartir bloques completos de filas con `Scatter` o `Scatterv`: el bloque de varias filas consecutivas es contiguo. Hay que calcular correctamente las cantidades y los desplazamientos y preparar la memoria de recepción.

En cambio, un bloque de columnas de varias filas normalmente no forma un único tramo contiguo en una representación por filas: incluso con el bloque lineal, hay que estudiar el patrón que se quiere comunicar.

### 9.2. Reservas independientes: no se puede fingir un bloque lineal

Si cada fila se ha reservado por separado, esto **no es válido para difundir toda la matriz** cuando `nf > 1`:

```c
/* Incorrecto con filas reservadas independientemente: */
MPI_Bcast(matriz[0], nf * nc, MPI_INT, raiz, comunicador);
```

La llamada describe `nf * nc` enteros consecutivos a partir de la primera fila. Esa reserva solo contiene `nc` enteros. MPI no sigue automáticamente los punteros de la tabla para encontrar las demás filas.

Esto tampoco comunica los datos de la matriz:

```c
/* Incorrecto: matriz apunta a una tabla de direcciones, no a los enteros. */
MPI_Bcast(matriz, nf * nc, MPI_INT, raiz, comunicador);
```

Las direcciones de un proceso no permiten localizar automáticamente los datos en la memoria de otro proceso.

### 9.3. Las colectivas no están prohibidas con reservas por filas

La limitación anterior se refiere a tratar **toda la matriz como un único vector contiguo de un tipo básico**. No significa que el estándar prohíba cualquier colectiva sobre sus datos.

Por ejemplo, con todas las filas preparadas en todos los procesos, puede difundirse cada fila:

```c
for (int i = 0; i < nf; ++i) {
    MPI_Bcast(matriz[i], nc, MPI_INT, raiz, comunicador);
}
```

Son `nf` llamadas colectivas; cada fila es un búfer contiguo válido. No equivale a afirmar que internamente habrá exactamente `nf` mensajes de red.

También se pueden reunir los datos en un búfer temporal contiguo o describir ubicaciones no contiguas mediante tipos derivados MPI. **Esos tipos ya existen en MPI-1**, pero aquí se mencionan solo como contexto del estándar: no se presupone que estén vistos o permitidos en las prácticas.

La documentación de [tipos derivados de MPI-1.1](https://www.mpi-forum.org/docs/mpi-1.1/mpi-11-html/node54.html) explica la descripción de regiones no contiguas y la alternativa de empaquetado.

Una lectura precisa de lo dicho en clase es: «con filas independientes no puedo usar directamente una colectiva sobre toda la matriz como si fuera un único bloque de elementos básicos; con el bloque contiguo sí puedo describir ese bloque directamente».

## 10. Cómo se libera cada organización

### Filas reservadas por separado

```c
for (int i = 0; i < nf; ++i) {
    free(matriz[i]);
}
free(matriz);
```

Cada fila corresponde a un bloque independiente. Se libera cada una y luego la tabla.

### Bloque de datos único con tabla de filas

```c
free(matriz);  /* Tabla de filas. */
free(datos);   /* Único bloque de datos. */
```

No se hace `free(matriz[i])` para cada fila: esas direcciones son vistas dentro de un mismo bloque, no reservas independientes. Tampoco se accede a la tabla después de liberarla.

Si la tabla es un array local, no se pasa a `free`; únicamente se liberan los bloques dinámicos que realmente se hayan creado.

## 11. Qué ocurre con problemas grandes

Para una matriz rectangular densa con filas del mismo ancho, en principio se puede representar todo el contenido en un bloque contiguo. Elegir otra organización puede responder a restricciones del ejercicio o a necesidades del problema.

Con tamaños grandes aparecen límites concretos:

- El producto `nf * nc * sizeof(elemento)` puede desbordarse antes de reservar: hay que comprobarlo.
- Una petición única grande puede fallar por límites del proceso, del asignador o del espacio de direcciones disponible.
- Reservar por filas permite pedir bloques menores, pero no aumenta la memoria total disponible y también puede fallar.
- Muchos bloques pequeños añaden tablas de punteros, costes de asignación y gestión, y pueden afectar a la localidad de los accesos.
- Las firmas MPI con cantidades de tipo `int` también limitan cuántos elementos se pueden describir en una llamada. El tamaño de la reserva y el límite de una comunicación son problemas diferentes.

**Contiguo para el programa significa contiguo en su espacio de direcciones virtuales**, no necesariamente en la memoria física. El sistema operativo puede respaldar páginas virtuales consecutivas con páginas físicas separadas.

Tampoco debe confundirse la capacidad agregada de un clúster con un único bloque local: tener memoria repartida entre varios nodos no permite que un proceso haga un `malloc` ordinario que use automáticamente toda esa memoria remota.

## 12. Relación con las prácticas 7, 8 y 9

En nuestros programas, `MatTotal` se reserva por filas. Las filas de cada submatriz también se reservan individualmente. Esta organización coincide con la intención que has recogido del profesor.

Cambiar la tabla exterior de submatrices de un triple puntero dinámico a un array de punteros dobles no modifica esa propiedad:

```c
unsigned char **submatrices[5] = {NULL};

/* Dentro de los bucles correspondientes: */
submatrices[s][i] = malloc((size_t)columnas * sizeof *submatrices[s][i]);
```

El array exterior contiene referencias a submatrices. La contigüidad de sus datos la deciden las reservas de las filas, no esa declaración exterior.

Para las copias con `memcpy`, la consecuencia es la misma: una fila o un segmento de fila sí es contiguo; varias filas reservadas independientemente no se copian como un bloque único suponiendo que una continúa a la otra.

Consulta la [comparación del triple puntero y el array de punteros dobles](COMPARACION_TRIPLE_PUNTERO_Y_ARRAY_PUNTEROS_DOBLES.md) para los cambios concretos de cada práctica.

## 13. Ideas que deben quedar claras

1. El `malloc` del bucle reserva los datos de una fila; la tabla exterior guarda sus direcciones.
2. Con tabla dinámica y una reserva por fila hay `nf + 1` llamadas a `malloc`.
3. `int **` no garantiza una organización concreta: la determina la construcción de la memoria.
4. Un bloque lineal puede representar una matriz lógica y puede acompañarse de una tabla de filas.
5. Para estas prácticas, según la indicación del profesor, se pretende la reserva independiente por filas.
6. Reservas y mensajes son decisiones diferentes; se obtiene un envío por fila cuando así lo programa el algoritmo.
7. La contigüidad permite describir directamente toda la matriz mediante un tipo MPI básico, pero una colectiva no interpreta automáticamente una tabla de punteros.
