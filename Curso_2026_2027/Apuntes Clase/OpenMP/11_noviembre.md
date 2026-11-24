# Reparto de trabajo en OpenMP

Los ejemplos usan C y OpenMP. Incluye `<omp.h>` para utilizar sus funciones y `<stdio.h>` para imprimir. Con GCC se puede compilar usando `gcc -fopenmp archivo.c -o programa`.

## `omp for`

`omp for` reparte las iteraciones de un bucle entre los hilos de un equipo que ya existe. Por eso se coloca dentro de una región paralela y debe ir seguido del bucle que se va a repartir:

```c
#pragma omp parallel num_threads(4) shared(datos)
{
    #pragma omp for schedule(static)
    for (int i = 0; i < 10000; i++) {
        procesar(&datos[i]);
    }
}
```

Cada iteración se asigna a un hilo. Es necesario que las iteraciones puedan ejecutarse en paralelo sin dependencias peligrosas y que `procesar` sea seguro para las posiciones que recibe. El índice `i` es privado para cada hilo. Al final de `omp for` hay una barrera implícita salvo que se use `nowait`.

La cláusula `schedule` determina cómo se reparten las iteraciones. Si se omite, OpenMP deja la planificación predeterminada a la implementación; en la asignatura puede exigirse escribirla explícitamente.

## `schedule(tipo, tamaño)`

El tamaño indica cuántas iteraciones contiene un **bloque** o *chunk*, no cuántas iteraciones realiza necesariamente cada hilo en total: un hilo puede recibir varios bloques. El último bloque puede ser más pequeño si no quedan suficientes iteraciones.

### `static`

Con `schedule(static, tamaño)`, las iteraciones se dividen en bloques del tamaño indicado y se asignan a los hilos por turnos, en orden de identificador. Por ejemplo, con 12 iteraciones, tres hilos y `schedule(static, 2)`:

| Hilo | Iteraciones asignadas |
| --- | --- |
| 0 | 0–1 y 6–7 |
| 1 | 2–3 y 8–9 |
| 2 | 4–5 y 10–11 |

Si se escribe `schedule(static)` sin tamaño, el espacio de iteraciones se divide en bloques aproximadamente iguales, como máximo uno por hilo; el tamaño exacto depende de la implementación. Es una opción útil cuando las iteraciones tienen costes parecidos y se busca poco coste de planificación.

### `dynamic`

Con `schedule(dynamic, tamaño)`, los hilos solicitan un nuevo bloque cuando terminan el anterior, hasta que no queda trabajo. Esto puede mejorar el equilibrio si algunas iteraciones tardan mucho más que otras, a cambio del coste de repartir bloques durante la ejecución.

```c
#pragma omp parallel for schedule(dynamic, 2)
for (int i = 0; i < 12; i++) {
    procesar(i);
}
```

Aquí hay seis bloques de dos iteraciones. No se puede saber de antemano cuántos recibirá cada hilo: depende de cuándo vaya quedando disponible. OpenMP no garantiza que se elija el hilo libre con menor identificador. Si se omite el tamaño en `dynamic`, el tamaño predeterminado es 1.

### `guided`

Con `schedule(guided, tamaño)`, los hilos también solicitan trabajo conforme quedan libres, pero los bloques empiezan siendo grandes y disminuyen según quedan menos iteraciones. El tamaño indicado actúa como mínimo para los bloques, salvo el último, que puede ser menor si quedan menos iteraciones. Si se omite, el mínimo predeterminado es 1.

El cálculo concreto de los tamaños puede variar entre implementaciones; no debe suponerse una secuencia exacta de bloques. Es una alternativa para equilibrar cargas variables con menos solicitudes de bloques al comienzo que una planificación dinámica con bloques pequeños.

```c
#pragma omp parallel for schedule(guided, 2)
for (int i = 0; i < 100; i++) {
    procesar(i);
}
```

En general, bloques pequeños permiten ajustar mejor el reparto, pero aumentan el coste de planificación. El mejor tamaño depende del trabajo y debe medirse.

# Construcciones de reparto

## `parallel for`

Combina la creación de una región paralela y el reparto de un bucle. Es una forma abreviada de usar `parallel` y después `for`:

```c
#pragma omp parallel for num_threads(4) schedule(static)
for (int i = 0; i < 10000; i++) {
    procesar(&datos[i]);
}
// Al salir de la región, todos los hilos han terminado.
```

## `sections` y `section`

`sections` reparte bloques de código distintos entre los hilos del equipo. Cada `section` se ejecuta una vez por uno de los hilos; un hilo puede ejecutar más de una sección y no se garantiza qué hilo ejecutará cada una.

```c
#pragma omp parallel
{
    #pragma omp sections
    {
        #pragma omp section
        { preparar_datos(); }

        #pragma omp section
        { preparar_configuracion(); }
    }
    // Por defecto, todos esperan aquí a que terminen las secciones.
}
```

Las dos tareas del ejemplo deben ser independientes o estar sincronizadas apropiadamente. `sections` tiene una barrera implícita al final, salvo que se use `nowait`. La forma combinada que crea el equipo y reparte las secciones es `#pragma omp parallel sections`, en plural:

```c
#pragma omp parallel sections
{
    #pragma omp section
    { tarea_a(); }
    #pragma omp section
    { tarea_b(); }
}
```

# Una ejecución por equipo

## `single`

El bloque `single` lo ejecuta una sola vez uno de los hilos del equipo; no tiene por qué ser el hilo principal. Los demás no ejecutan el bloque. Por defecto, todos esperan en una barrera implícita al terminar; puede eliminarse con `nowait` cuando sea seguro.

```c
#pragma omp parallel shared(valor)
{
    #pragma omp single
    { valor = calcular_valor(); }

    usar(valor); // la barrera implícita espera a que valor esté listo
}
```

Si `usar` modifica datos compartidos, esa operación puede necesitar su propia sincronización.

## `master`

El bloque `master` lo ejecuta únicamente el hilo principal del equipo, identificado como hilo 0. A diferencia de `single`, **no tiene barrera implícita al final**:

```c
#pragma omp parallel shared(valor)
{
    #pragma omp master
    { valor = calcular_valor(); }

    #pragma omp barrier
    usar(valor); // espera a que el hilo principal haya terminado
}
```

La barrera solo es necesaria si el trabajo posterior depende de que termine `master` y no existe otra sincronización adecuada. La distinción principal es que `single` puede ser ejecutado por cualquier hilo y sincroniza al final por defecto, mientras que `master` lo ejecuta el hilo principal y no sincroniza por sí solo.
