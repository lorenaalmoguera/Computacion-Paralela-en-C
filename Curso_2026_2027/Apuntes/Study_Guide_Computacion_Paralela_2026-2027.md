# Study Guide Computación Paralela 2026 2027

Guía de repaso para relacionar los fundamentos de arquitectura, la programación secuencial en C y el primer modelo paralelo con MPI. La secuencia de estudio es deliberada: primero se valida qué calcula el programa; después se decide cómo repartirlo y comunicarlo.

## Mapa de la asignatura

```text
Arquitectura y rendimiento
          ↓
Programa secuencial correcto en C
          ↓
Diseño del reparto y las comunicaciones
          ↓
MPI: procesos, mensajes y operaciones colectivas
          ↓
Comparación con la referencia secuencial
```

## 1. Ideas que debes poder explicar

### Paralelismo y rendimiento

- Paralelismo: varias tareas ejecutan simultáneamente en recursos distintos.
- Concurrencia: varias tareas progresan en intervalos solapados; pueden alternarse en un único núcleo.
- Un sistema paralelo incluye hardware, software y herramientas de ejecución.
- Aceleración: `S(P) = T(1) / T(P)`.
- Eficiencia: `E(P) = S(P) / P`.
- La escalabilidad describe qué ocurre al cambiar recursos y tamaño del problema; se analiza, no se resume siempre en una cifra.
- Ley de Amdahl: `S(P) = 1 / ((1-f) + f/P)`. La fracción secuencial limita la mejora.

### C secuencial y ficheros

- Los índices de un vector de longitud `n` van de `0` a `n-1`.
- `argc` cuenta argumentos; `argv[0]` es el nombre del programa y los argumentos del usuario empiezan en `argv[1]`.
- `fopen(nombre, "rb")` abre un fichero binario; devuelve `NULL` si no puede abrirlo.
- `fread(destino, tamaño, cantidad, fichero)` escribe en la dirección `destino`.
- `fread(v+i, sizeof *v, 1, f)` lee un elemento en cada vuelta. `&v[i]` es una forma equivalente de indicar el destino.
- Texto almacena caracteres; binario almacena representaciones de datos. El tamaño binario depende de los tipos y el formato acordado.
- `malloc` proporciona almacenamiento dinámico hasta `free`. Una llamada a `malloc` para dos vectores requiere reservar el doble de elementos.

### Memoria y punteros

Dos reservas: `v1 = malloc(tam * sizeof *v1)` y `v2 = malloc(tam * sizeof *v2)`; se liberan ambas.

Una reserva: `datos = malloc(2 * tam * sizeof *datos)`, `v1 = datos`, `v2 = datos + tam`; se libera únicamente `datos`.

```text
datos ──> [ V1: tam elementos ][ V2: tam elementos ]
          ^ v1                 ^ v2 = datos + tam
```

- `v[i]` es equivalente a `*(v+i)`.
- `&v[i]` es equivalente a `v+i`: ambas expresiones son la dirección de la posición `i`.
- `p+1` avanza un elemento del tipo apuntado, es decir, `sizeof(*p)` bytes.
- `void*` no admite aritmética portable en C estándar.
- Un `char*` puede acceder a los bytes de la representación de otro objeto. Modificar un byte no equivale necesariamente a poner el objeto entero a cero.
- No desreferencies una dirección de `int` como `double*`: las reglas de tipos y la alineación lo pueden hacer comportamiento indefinido.

### MPI

- Cada proceso tiene su propia memoria; los nombres iguales en dos procesos no comparten automáticamente el dato.
- `MPI_Init` inicia MPI; `MPI_Comm_size` obtiene el tamaño del grupo; `MPI_Comm_rank` obtiene el identificador local; `MPI_Finalize` cierra MPI.
- `MPI_COMM_WORLD` es el comunicador predefinido del grupo de procesos lanzados.
- `MPI_Send` y `MPI_Recv` deben emparejar origen/destino, etiqueta, comunicador y datos compatibles.
- `MPI_Recv` es bloqueante. Si no llega un mensaje compatible, puede esperar indefinidamente.
- `MPI_Bcast` distribuye desde una raíz y lo llaman todos los miembros del comunicador.
- `MPI_Reduce` combina contribuciones y entrega el resultado a la raíz; `MPI_Allreduce` entrega el resultado a todos.
- Diseña el reparto, las comunicaciones y las necesidades de memoria antes de codificar.

## 2. Mini ejemplos de examen

### Dirección y valor

```c
int datos[3] = {10, 20, 30};
int *v = datos;
int valor = v[2];             /* valor del tercer int: 30 */
int mismo_valor = *(v + 2);   /* el mismo valor */
int *direccion = &v[2];      /* dirección del tercer int */
int *misma_direccion = v + 2; /* la misma dirección */
```

En `fread(&v[i], sizeof v[i], 1, f)`, `&` es necesario porque `fread` necesita una dirección donde escribir, no el valor actual de `v[i]`.

### Offset de `V2` en la reserva conjunta

Si `tam == 3`, una reserva contiene seis enteros:

```text
índice del bloque:  0       1       2       3       4       5
                   V1[0]   V1[1]   V1[2]   V2[0]   V2[1]   V2[2]
v1 ────────────────^
v2 ───────────────────────────────^
```

En general `v2 = v1 + tam`. El desplazamiento en bytes es `tam * sizeof(int)`.

### Leer y reducir

Una estrategia MPI para contar elementos pares sería: cada proceso cuenta pares en su bloque local, después todos aportan un contador a `MPI_Reduce` con operación suma y la raíz recibe el total. Para sumar los valores pares, cada proceso aporta la suma local de esos valores: contar y sumar son problemas distintos. Para el máximo, la operación colectiva correspondiente es máximo.

## 3. Errores que debes detectar rápido

1. Leer `argv[2]` sin haber comprobado que existe.
2. Reservar `tam` elementos y escribir `2*tam`.
3. Pasar `v[i]` a `fread` en vez de `&v[i]` o `v+i`.
4. Pedir `tam` elementos en cada iteración cuando el bucle ya avanza una posición.
5. Liberar `v1` y `v2` por separado si solo se reservó el bloque `datos`.
6. Acceder al índice `tam` de un vector con `tam` elementos.
7. Suponer que los mensajes MPI coinciden solo porque tienen el mismo `tag`.
8. Llamar a una colectiva en distinto orden o desde solo una parte del comunicador.
9. Paralelizar antes de tener una salida secuencial de referencia.

## 4. Comprueba tu comprensión

Intenta responder sin mirar la clave.

1. ¿Qué diferencia hay entre `v[1]`, `*(v+1)`, `&v[1]` y `v+1`?
2. Si `sizeof(int)==4`, ¿cuántos bytes avanza `int *p` al sumar 3?
3. ¿Qué dos punteros se derivan de una única reserva para dos vectores de `tam` enteros? ¿Qué se libera?
4. ¿Qué pasa si `fopen` devuelve `NULL` y se llama a `fread`?
5. En una reserva única de dos vectores de 200 enteros, ¿qué posición del bloque corresponde a `v1[205]`?
6. ¿Por qué `*pchar = 0` puede cambiar `v1[0]` sin convertirlo en cero?
7. ¿Por qué no se debe leer `v1` mediante `double*`?
8. ¿Qué información identifica un envío MPI compatible con su recepción?
9. ¿Qué diferencia hay entre `MPI_Reduce` y `MPI_Allreduce`?
10. Si el 20 % del trabajo es secuencial, ¿cuál es la aceleración ideal límite al aumentar mucho `P`?

### Clave

1. `v[1]` y `*(v+1)` son el valor; `&v[1]` y `v+1` son su dirección.
2. `3 * sizeof(int) = 12` bytes.
3. `v1 = datos`, `v2 = datos + tam`; se libera `datos` una vez.
4. Usar un flujo nulo en fread tiene comportamiento indefinido; hay que comprobar `f` antes de leer.
5. La posición 205 del bloque: `v2[5]`, siempre que el bloque contenga primero V1[0..199] y luego V2[0..199].
6. El puntero `char*` escribe un byte de la representación, no los bytes completos del `int`.
7. La dirección contiene objetos `int`, no un objeto `double`; el acceso puede violar reglas de tipo y alineación.
8. Emisor/destinatario, etiqueta y comunicador/contexto identifican el mensaje. Además, los tipos deben ser compatibles y la recepción tener capacidad suficiente; su count puede ser mayor que el del envío.
9. `Reduce` deja el resultado en la raíz; `Allreduce` hace que todos lo reciban.
10. `1 / 0.2 = 5` como máximo ideal según Amdahl.

## 5. Rutina de repaso

1. Explica el algoritmo secuencial en voz alta y pruébalo con un vector pequeño.
2. Dibuja la memoria y escribe las direcciones iniciales de cada segmento.
3. Traza a mano una llamada a `fread` y otra a `MPI_Send`/`MPI_Recv`.
4. Calcula aceleración y eficiencia a partir de tiempos dados.
5. Compara cualquier resultado paralelo con la solución secuencial.

## Temario incluido en esta edición

Fundamentos de paralelismo, rendimiento, memoria dinámica, ficheros binarios, vectores y punteros; modelo de memoria distribuida; MPI inicialización, identificación, envío, recepción y colectivas hasta el 6 de octubre.

En [las explicaciones de colectivas](../Apuntes%20Clase/MPI/Funciones%20colectivas/README.md), repasa: Gather reúne bloques por rango; Gatherv admite tamaños y desplazamientos distintos; Scatter y Scatterv reparten bloques; Scan calcula prefijos inclusivos; Reduce_scatter reduce y reparte segmentos; Barrier espera a que todos lleguen. Las [versiones Send/Recv](../Apuntes%20Clase/MPI/Funciones%20colectivas/Send_Recv/README.md) muestran algoritmos didácticos.

Para comprobarte: ¿por qué Scan no cambia el count entre procesos? ¿Cuánto espacio requiere Gatherv si hay huecos? ¿Por qué MPI_Finalize no arregla comunicaciones pendientes? Consulta la [revisión conceptual](Revision_conceptual_para_profesor.md) y la [clase del 6 de octubre](../Apuntes%20Clase/MPI/06_octubre.md).
