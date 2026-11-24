# `threadprivate`

La directiva `threadprivate` hace que cada hilo tenga su propia copia de una variable con duración de almacenamiento estática. Puede aplicarse, por ejemplo, a una variable global. Cada hilo accede a su copia cuando usa el nombre de la variable; no es necesario incluirla en una cláusula `private` de cada región paralela.

La copia de un hilo puede conservar su valor entre regiones paralelas, pero **esa persistencia no está garantizada en todos los casos**: OpenMP establece condiciones, entre ellas que las regiones tengan el mismo número de hilos y la misma política de afinidad, y que el ajuste dinámico de hilos esté desactivado. Por eso no conviene suponer que el hilo con un determinado identificador conservará siempre su valor al entrar en otra región.

En este ejemplo, cada copia parte del inicializador `14` y cada hilo modifica la suya:

```c
#include <omp.h>
#include <stdio.h>

int fp = 14;
#pragma omp threadprivate(fp)

int main(void)
{
    #pragma omp parallel num_threads(4) default(none)
    {
        int iam = omp_get_thread_num();
        fp += iam + 1;
        printf("DENTRO: fp = %d, hilo = %d\n", fp, iam);
    }

    printf("FUERA: fp = %d\n", fp);
    return 0;
}
```

Si se crean cuatro hilos, sus copias terminan con los valores 15, 16, 17 y 18, respectivamente. Fuera de la región continúa el hilo que la encontró, con su propia copia, cuyo valor es 15. El orden de las líneas impresas dentro de la región puede variar.

# Cláusula `copyin`

`copyin(fp)` copia el valor que tiene la copia de `fp` del hilo que encuentra la región paralela (el hilo principal del nuevo equipo) a las copias de los demás hilos del equipo **al entrar en esa región**. Así todos comienzan con el mismo valor, aunque después cada uno modifica su propia copia. `copyin` requiere que la variable sea `threadprivate`; no combina los resultados al salir.

```c
#include <omp.h>

int fp = 14;
#pragma omp threadprivate(fp)

int main(void)
{
    for (int i = 0; i < 3; i++) {
        #pragma omp parallel num_threads(i + 3) default(none) copyin(fp)
        {
            int iam = omp_get_thread_num();
            fp += (iam + 1) * 10;
        }
        fp += 1000;
    }
    return 0;
}
```

En la primera iteración se solicitan tres hilos. Al entrar, `copyin` les da a todos el valor 14:

| Hilo | `fp` al entrar | `fp` al salir de la región |
| --- | ---: | ---: |
| 0 (principal) | 14 | 24 |
| 1 | 14 | 34 |
| 2 | 14 | 44 |

Este esquema sigue la primera región del ejemplo; cada hilo trabaja con una copia independiente:

```text
Antes de entrar: fp del hilo principal = 14
                           │
                        copyin
              ┌────────────┼────────────┐
              ▼            ▼            ▼
           hilo 0       hilo 1       hilo 2
            fp=14        fp=14        fp=14
              │            │            │
             +10          +20          +30
              ▼            ▼            ▼
            fp=24        fp=34        fp=44
              │
              └── continúa fuera y suma 1000 → fp=1024
```

Las copias no se suman entre sí. El hilo principal continúa con su copia, 24, y le suma 1000. En la siguiente región, `copyin` copia **1024**, no 14. La evolución de la copia del hilo principal es:

| Iteración | Valor al entrar, copiado por `copyin` | Después de la región | Después de sumar 1000 |
| --- | ---: | ---: | ---: |
| 0 | 14 | 24 | 1024 |
| 1 | 1024 | 1034 | 2034 |
| 2 | 2034 | 2044 | 3044 |

Por tanto, `copyin` no copia necesariamente el valor inicial de la variable: copia el valor actual de la copia del hilo principal cada vez que se entra en la región paralela.



La inicialización de cada copia `threadprivate` y su persistencia dependen de las reglas de OpenMP. `copyin` establece expresamente el valor de las copias a partir del hilo principal al entrar en la región.


# Reducción: `reduction`

Una reducción combina los resultados parciales de varios hilos en una variable original. Se escribe `reduction(operador: lista_de_variables)` en una construcción compatible, como `parallel for` o `for`.

En una suma, OpenMP crea copias privadas de la variable y las inicializa a **0**. Cada hilo acumula en su copia; al terminar la reducción, OpenMP combina las copias entre sí y con el valor original. La variable original no tiene que ser global: puede ser local a `main` y compartida en la región paralela.

Hay dos inicializaciones diferentes:

- Las copias privadas reciben automáticamente el valor inicial asociado al operador de reducción: por ejemplo, 0 para `+` y 1 para `*`.
- La variable original debe tener un valor apropiado antes de la construcción. Su valor también participa en el resultado: si una suma empieza con 100, el resultado final incluye esos 100.

Una construcción `for` tiene por defecto una barrera implícita al final, tras la cual se puede usar el resultado de la reducción. Con `nowait` no se puede suponer que la variable original esté actualizada cuando un hilo sale del `for`; se necesita sincronización antes de leerla. En `parallel for`, el resultado está disponible después de terminar la región paralela.

## Ejemplo: producto escalar

El producto escalar suma los productos de los elementos correspondientes de dos arrays: `data1[0] * data2[0] + ... + data1[9] * data2[9]`.

```c
#include <omp.h>
#include <stdio.h>

int main(void)
{
    int data1[10], data2[10];
    int prod_int = 0;

    for (int i = 0; i < 10; i++) {
        data1[i] = i;
        data2[i] = i * 2;
    }

    #pragma omp parallel num_threads(4) default(none) shared(data1, data2, prod_int)
    {
        #pragma omp for schedule(static, 2) reduction(+:prod_int)
        for (int i = 0; i < 10; i++) {
            prod_int += data1[i] * data2[i];
        }
    }

    printf("Producto escalar: %d\n", prod_int); // 570
    return 0;
}
```

`prod_int` es compartida en la región `parallel`, pero dentro del `for` cada hilo trabaja con su copia de reducción. `schedule(static, 2)` reparte las iteraciones en bloques de dos. Si se crean cuatro hilos, los resultados son:

| Hilo | Iteraciones | Suma parcial |
| --- | --- | ---: |
| 0 | 0, 1, 8, 9 | 292 |
| 1 | 2, 3 | 26 |
| 2 | 4, 5 | 82 |
| 3 | 6, 7 | 170 |

La reducción combina las sumas parciales con el valor original:

```text
Hilo:                 0       1       2       3
Suma parcial:        292      26      82      170
                      └───────┴───────┴───────┘
                                  │
                           reducción con +
                                  ▼
Valor original 0 + parciales = 570
```

Al terminar, se combinan con el valor original: `0 + 292 + 26 + 82 + 170 = 570`. Si se crean menos hilos, cambia el reparto, pero no el resultado de esta suma entera, siempre que no haya desbordamiento. `num_threads(4)` solicita cuatro hilos; no siempre garantiza que se creen cuatro.

## Operadores y reducciones múltiples

Entre los identificadores habituales de reducción en C están `+`, `-`, `*`, `max` y `min`. También existen operadores lógicos y de bits de C. Las formas `.and.` y `.or.` que aparecen en algunos apuntes son de Fortran, no de C. La compatibilidad concreta depende de la versión de OpenMP implementada por el compilador.

Para `max` y `min`, inicializa la variable original con un valor adecuado al problema; por ejemplo, para el máximo de un array no vacío puede usarse su primer elemento. Para una multiplicación independiente, inicializa el original a 1; si se deja en 0, el producto final será 0.

Con el mismo operador, estas dos formas son equivalentes para variables distintas:

```c
reduction(+:dato1, dato2)
reduction(+:dato1) reduction(+:dato2)
```

También se pueden combinar operadores distintos en la misma directiva:

```c
reduction(+:dato1) reduction(*:dato3)
```

El operador `-` requiere atención: en OpenMP las copias privadas se inicializan a cero y se combinan mediante suma. Por ejemplo, con `reduction(-:x)` y la operación `x -= valor`, se suman los parciales negativos al valor original. No equivale a restar arbitrariamente las copias entre sí.

En operaciones de coma flotante, el orden de combinación puede variar y producir pequeñas diferencias de redondeo respecto a una ejecución secuencial.

## La misma suma sin `reduction`

Cada hilo puede acumular primero en una variable local y combinar su parcial con la variable compartida una sola vez mediante `critical`:

```c
int prod_int = 0;
#pragma omp parallel num_threads(4) default(none) shared(data1, data2, prod_int)
{
    int parcial = 0; // privada: se declara dentro de la región

    #pragma omp for schedule(static, 2)
    for (int i = 0; i < 10; i++) {
        parcial += data1[i] * data2[i];
    }

    #pragma omp critical
    {
        prod_int += parcial;
    }
}
printf("Producto escalar: %d\n", prod_int); // 570
```

`critical` evita que dos hilos actualicen `prod_int` simultáneamente. Sin esa protección habría una condición de carrera. La barrera al final de la región asegura que todas las contribuciones se han añadido antes del `printf`. Acumular localmente y entrar una sola vez en `critical` evita serializar cada iteración; para esta operación, `reduction` expresa directamente la intención y suele ser la opción más sencilla.


# De cara al examen. Si tenemos libertad es mejor usar automatizadores. Sin embargo, en el examen depende siempre de lo que nos diga héctor.



Las clausulas que hemos visto:

* if
* private
* shared
* default
* firstprivate
* lastprivate
* reduction
* copyin
* copyprivate
* schedule
* ordered
* nowait
# Resumen de cláusulas y directivas

La tabla usa los nombres de C/C++ (`for`). En Fortran, `do` ocupa el lugar de `for`.

| Cláusula | `parallel` | `for` / `do` | `sections` | `single` | `parallel for` / `parallel do` | `parallel sections` |
| --- | :---: | :---: | :---: | :---: | :---: | :---: |
| `if` | ✓ | — | — | — | ✓ | ✓ |
| `private` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `shared` | ✓ | — | — | — | ✓ | ✓ |
| `default` | ✓ | — | — | — | ✓ | ✓ |
| `firstprivate` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `lastprivate` | — | ✓ | ✓ | — | ✓ | ✓ |
| `reduction` | ✓ | ✓ | ✓ | — | ✓ | ✓ |
| `copyin` | ✓ | — | — | — | ✓ | ✓ |
| `copyprivate` | — | — | — | ✓ | — | — |
| `schedule` | — | ✓ | — | — | ✓ | — |
| `ordered` | — | ✓ | — | — | ✓ | — |
| `nowait` | — | ✓ | ✓ | ✓ | — | — |

Las directivas combinadas `parallel for` y `parallel sections` admiten las cláusulas correspondientes a sus componentes, con algunas excepciones. Por ejemplo, no admiten `nowait`: al terminar la región paralela debe existir una sincronización.

- `ordered` en la tabla es la **cláusula del bucle**; permite usar la directiva `#pragma omp ordered` dentro de él.
- En `single`, `copyprivate` y `nowait` no pueden utilizarse juntas: `copyprivate` necesita la sincronización al final de `single`.

La tabla recoge las formas habituales de estas construcciones; la disponibilidad de algunas opciones puede depender de la versión de OpenMP y del compilador.

# Funciones y variables de entorno de OpenMP

Para usar las funciones en C hay que incluir `#include <omp.h>`. Permiten consultar o modificar la configuración y medir tiempos durante la ejecución.

| Función | Qué hace |
| --- | --- |
| `void omp_set_num_threads(int num_threads)` | Establece el número de hilos solicitado por defecto para las siguientes regiones paralelas encontradas por la tarea que llama, cuando no se use `num_threads(...)`. El número efectivo puede depender de otros límites y ajustes. |
| `int omp_get_num_threads(void)` | Devuelve el número de hilos del equipo actual. Fuera de una región paralela devuelve 1. |
| `int omp_get_max_threads(void)` | Devuelve el límite superior de hilos que podría formar una próxima región paralela sin cláusula `num_threads`, según la configuración de la tarea actual. No es necesariamente el número que se creará ni el número de procesadores. |
| `int omp_get_thread_num(void)` | Devuelve el identificador del hilo en su equipo actual, desde 0 hasta `omp_get_num_threads() - 1`. Fuera de una región paralela devuelve 0. |
| `int omp_get_num_procs(void)` | Devuelve el número de procesadores disponibles para el programa en ese momento. Representa recursos de ejecución disponibles, que pueden ser procesadores lógicos; no necesariamente núcleos físicos. |
| `int omp_in_parallel(void)` | Devuelve un valor distinto de 0 si se ejecuta dentro de una región paralela activa; en caso contrario devuelve 0. Una región con un solo hilo no es activa. |
| `void omp_set_dynamic(int valor)` | Activa el ajuste dinámico del número de hilos si `valor` es distinto de 0; lo desactiva si es 0. |
| `int omp_get_dynamic(void)` | Devuelve un valor distinto de 0 si está activado el ajuste dinámico del número de hilos. |
| `void omp_set_nested(int valor)` | Activa o desactiva el paralelismo anidado. Es una interfaz obsoleta. |
| `int omp_get_nested(void)` | Consulta si está habilitado el paralelismo anidado. Es una interfaz obsoleta. |
| `double omp_get_wtime(void)` | Devuelve un tiempo transcurrido en segundos, útil para medir intervalos. Su origen es arbitrario: se restan dos lecturas del mismo hilo. |
| `double omp_get_wtick(void)` | Devuelve en segundos la resolución del temporizador utilizado por `omp_get_wtime`. |

**Ajuste dinámico de hilos:** cuando está activado, la implementación puede ajustar el tamaño de los equipos para las regiones paralelas según los recursos disponibles. Esto es distinto de `schedule(dynamic)`, que decide cómo se entregan los bloques de iteraciones a los hilos de un equipo ya creado.

**Paralelismo anidado:** consiste en encontrar una región `parallel` dentro de otra. Las funciones `omp_set_nested`, `omp_get_nested` y la variable `OMP_NESTED` están obsoletas en OpenMP; para código nuevo se prefiere controlar el número máximo de niveles paralelos activos mediante `omp_set_max_active_levels`, `omp_get_max_active_levels` y `OMP_MAX_ACTIVE_LEVELS`. Por defecto estará desactivado y habrá que activarlo.

Ejemplo de medición de tiempo:

```c
double inicio = omp_get_wtime();
// Código que se desea medir.
double transcurrido = omp_get_wtime() - inicio;
printf("Tiempo: %f segundos\n", transcurrido);
```

Como se determina exactamente el número de hilos de una región paralela? En primer caso si tiene la clausula numthreads me da igual cualquier otra cosa. Si está la clausula numthreads manda esa clausula. SI no esta esta clausula dependerá de una variable de entorno. Este comportamiento lo podemos modificar `omp_set_num_threads`. No es una funcion que tengamos que llamar dentro de una funcion paralela. En cualquier caso es un número fijo de hilos.

La clausula numthreads no tiene porque estar presente. Que hace `omp_set_dynamic = true`: el numero de hilosd e las regiones paralelas que no tiene la clausula num_threads será dinámico en función del estado de la máquina. Es decir, se define por tiempo de ejecución y por los recursos libres que tenemos en el sistema.

## Variables de entorno

Se establecen antes de ejecutar el programa y proporcionan una configuración inicial. Algunas opciones pueden modificarse posteriormente mediante funciones o cláusulas de OpenMP.

| Variable | Qué configura |
| --- | --- |
| `OMP_SCHEDULE` | El tipo de reparto y, opcionalmente, el tamaño de bloque para bucles con `schedule(runtime)`, por ejemplo `dynamic,2`. No cambia un bucle que indique explícitamente `schedule(static)` o `schedule(dynamic)`. |
| `OMP_NUM_THREADS` | El número de hilos solicitado por defecto para regiones paralelas; una cláusula `num_threads` puede indicar otro número para su región. |
| `OMP_DYNAMIC` | El ajuste dinámico del número de hilos: `TRUE` para activarlo o `FALSE` para desactivarlo. |
| `OMP_NESTED` | Activa o desactiva el paralelismo anidado (`TRUE` / `FALSE`). Está obsoleta; se prefiere `OMP_MAX_ACTIVE_LEVELS`. |

Por ejemplo, en PowerShell se pueden configurar estas variables antes de ejecutar el programa:

```powershell
$env:OMP_NUM_THREADS = '4'
$env:OMP_DYNAMIC = 'FALSE'
$env:OMP_SCHEDULE = 'dynamic,2'
.\programa.exe
```

Con esa configuración, un bucle declarado con `#pragma omp parallel for schedule(runtime)` utiliza la planificación dinámica con bloques de dos iteraciones.

Referencia: [especificación de OpenMP 5.2](https://www.openmp.org/spec-html/5.2/openmp.html).
