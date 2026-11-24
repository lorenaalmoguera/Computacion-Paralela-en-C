# Cláusulas de alcance de datos y sincronización

Los ejemplos usan OpenMP en C (`#include <omp.h>`). Para compilar con GCC se puede utilizar `gcc -fopenmp archivo.c -o programa`.

## `default(none)`

Se escribe en la directiva de una región paralela y obliga a indicar explícitamente el alcance de las variables cuyo alcance no está determinado de antemano. Esto ayuda a detectar si una variable debe ser compartida (`shared`) o privada (`private`, `firstprivate`, etc.). Las variables declaradas dentro de la región y el índice de un `omp for` son ejemplos de variables con alcance predeterminado, por lo que no hay que enumerarlas.

```c
int x = 5;
#pragma omp parallel default(none) shared(x)
{
    int id = omp_get_thread_num(); // privada: declarada dentro de la región
    printf("Hilo %d: x = %d\n", id, x);
}
```

`default(shared)` establece como compartidas las variables cuyo alcance se determina implícitamente; no convierte en compartidas las que ya tienen un alcance predeterminado privado.

## `firstprivate`

Crea una copia privada para cada hilo y la inicializa con el valor de la variable original al entrar en la construcción. Las modificaciones posteriores afectan solo a cada copia.

```c
int x = 5;
#pragma omp parallel num_threads(3) firstprivate(x)
{
    x += omp_get_thread_num();
    printf("Copia: %d\n", x); // valores 5, 6 y 7 si hay tres hilos
}
printf("Original: %d\n", x); // sigue siendo 5
```

El orden de impresión de los hilos puede variar. En contraste, una variable `private` ordinaria no recibe automáticamente el valor de la variable original.

## `lastprivate`

Se utiliza en construcciones como `for` y `sections`. Cada hilo trabaja con una copia privada y, al terminar la construcción, se copia a la variable original el valor correspondiente a la **última iteración en orden lógico** del bucle, o a la última sección en el orden del código. No tiene por qué ser la última iteración que termina en tiempo real.

```c
int ultimo = -1;
#pragma omp parallel for lastprivate(ultimo)
for (int i = 0; i < 5; i++) {
    ultimo = i * 10;
}
printf("%d\n", ultimo); // 40: corresponde a i = 4
```

`lastprivate` por sí sola no inicializa las copias privadas con el valor original. Si se necesita también esa inicialización, se pueden combinar `firstprivate` y `lastprivate` para la misma variable cuando la construcción lo permita. Si el bucle no ejecuta ninguna iteración, no hay una última iteración desde la que copiar el valor.

## `copyprivate`

Se usa en una construcción `single` para distribuir el valor calculado por el hilo que ejecuta el bloque a las copias privadas correspondientes de los demás hilos del equipo. La variable debe ser privada en el contexto que rodea a `single`. Hay una barrera implícita al terminar `single`, y `copyprivate` no se puede combinar con `nowait` en ese `single`.

```c
#pragma omp parallel
{
    int valor; // cada hilo tiene su propia copia
    #pragma omp single copyprivate(valor)
    {
        valor = 42; // solo un hilo ejecuta este bloque
    }
    // Tras la barrera, todas las copias de valor valen 42.
    printf("Hilo %d: %d\n", omp_get_thread_num(), valor);
}
```

`copyprivate` distribuye un resultado desde `single`; `firstprivate` inicializa copias al entrar en una construcción. `copyin`, por su parte, se usa con variables `threadprivate` al entrar en una región paralela.

# Sincronización

## `ordered`

Permite que una parte de un bucle paralelo se ejecute siguiendo el orden lógico de sus iteraciones. El resto del trabajo puede realizarse en paralelo. El bucle debe declarar la cláusula `ordered` y el bloque ordenado se marca con la directiva `ordered`.

```c
#pragma omp parallel for ordered
for (int i = 0; i < 5; i++) {
    int resultado = i * i; // puede calcularse en paralelo
    #pragma omp ordered
    printf("%d: %d\n", i, resultado); // se imprime en orden de i
}
```

Solo se ordena el bloque marcado, no todo el bucle. Esta ordenación puede reducir el paralelismo si el bloque es costoso.

## `critical`

Una región `critical` proporciona exclusión mutua: solo un hilo a la vez puede ejecutar una región crítica con el mismo nombre. Si no se indica nombre, las regiones críticas sin nombre comparten el mismo bloqueo. Cada hilo que alcanza la directiva ejecuta el bloque, pero de uno en uno; el orden de entrada no está garantizado.

```c
#pragma omp critical(actualizacion)
{
    suma_global += valor;
}
```

`critical` no implica una barrera: cuando un hilo sale puede continuar aunque otros todavía no hayan ejecutado el bloque. Solo hace falta una barrera adicional si el código posterior necesita esperar a que **todos** hayan terminado; también puede existir ya una barrera implícita, por ejemplo al final de un `omp for` sin `nowait`.

## `barrier`

Una barrera hace que los hilos del equipo esperen hasta que todos hayan llegado a ese punto. También garantiza la visibilidad de las operaciones de memoria anteriores conforme a las reglas de sincronización de OpenMP. No calcula ni actualiza por sí sola una suma: permite usar con seguridad los resultados que otros hilos ya han escrito.

Una barrera debe ser alcanzada por todos los hilos del equipo en una secuencia compatible; colocarla dentro de una rama que solo ejecutan algunos puede bloquear el programa.

### Ejemplo: sumas parciales y barrera

Cada hilo suma un tramo de los datos en una variable local y guarda su resultado. La barrera asegura que todas las sumas parciales estén listas antes de que el hilo principal las combine. El reparto siguiente también funciona cuando el número de datos no es divisible entre el número de hilos.

```c
#include <omp.h>
#include <stdio.h>

int main(void)
{
    enum { N = 100, MAX_HILOS = 4 };
    int datos[N], suma[MAX_HILOS] = {0}, suma_global = 0;
    for (int i = 0; i < N; i++) datos[i] = i;

    #pragma omp parallel num_threads(MAX_HILOS) default(none) shared(datos, suma, suma_global)
    {
        int iam = omp_get_thread_num();
        int np = omp_get_num_threads();
        int ini = N * iam / np;
        int fin = N * (iam + 1) / np;
        int parcial = 0;

        for (int i = ini; i < fin; i++) parcial += datos[i];
        suma[iam] = parcial;

        #pragma omp barrier
        #pragma omp master
        {
            for (int i = 0; i < np; i++) suma_global += suma[i];
        }
        // master no tiene barrera implícita al terminar.
    } // la barrera implícita de fin de región espera también al master

    printf("Suma: %d\n", suma_global); // 4950
    return 0;
}
```

Si se quisiera leer `suma_global` desde todos los hilos **dentro** de la región después de `master`, haría falta otra barrera tras ese bloque. En el ejemplo se imprime fuera, después de la barrera implícita de fin de región. Acumular primero en `parcial` también evita escribir repetidamente en posiciones contiguas del array compartido, lo que puede reducir el falso compartimiento de caché.

## Penalización: `critical` dentro de un bucle

Si todos los hilos actualizan una variable compartida sin protección, hay una **condición de carrera**:

```c
#pragma omp parallel for
for (int i = 0; i < n; i++)
    suma_global += datos[i]; // incorrecto: actualizaciones simultáneas
```

Proteger cada actualización con `critical` evita la carrera, pero obliga a los hilos a entrar de uno en uno en cada iteración y puede penalizar mucho el rendimiento. Para una suma, suele ser mejor una reducción:

```c
int suma_global = 0;
#pragma omp parallel for reduction(+:suma_global)
for (int i = 0; i < n; i++)
    suma_global += datos[i];
```

OpenMP crea acumuladores privados y combina sus resultados con la variable original al terminar la reducción. Una región crítica dentro de un bucle no es incorrecta por sí misma: es un posible cuello de botella, y puede ser necesaria para operaciones que no admitan una reducción apropiada.
