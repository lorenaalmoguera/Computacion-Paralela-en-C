# Clase del 11 de septiembre: arquitectura y rendimiento

Versión revisada de los conceptos del [PDF original](11_septiembre.pdf). Se conserva el PDF con las notas tal como se registraron en clase.

## Arquitectura y objetivo

Paralelizar busca reducir el tiempo de una aplicación repartiendo trabajo entre recursos. Un computador clásico contiene control, unidades aritmético-lógicas, registros, memoria, interconexión y entrada/salida. Una arquitectura paralela incorpora varios elementos de procesamiento capaces de ejecutar trabajo simultáneamente.

## Memoria automática y dinámica

malloc, calloc y realloc proporcionan almacenamiento dinámico. No es correcto afirmar que esa reserva se hace en la pila: habitualmente se gestiona en una zona denominada heap. El estándar de C distingue duración automática y almacenamiento asignado dinámicamente, sin exigir una organización física concreta.

Un array local puede tener duración automática; su dirección y su tiempo de vida no se deben confundir con los de un bloque obtenido mediante malloc.

### Qué hace realloc

realloc intenta cambiar el tamaño de una reserva y puede moverla. Para un tamaño nuevo positivo, si falla devuelve NULL y la reserva anterior sigue siendo válida. Usa un puntero temporal para no perderla:

```c
/* v ya apunta a una reserva; nuevo_tam > 0 y multiplicacion sin desbordar. */
int *nuevo = realloc(v, (size_t)nuevo_tam * sizeof *v);
if (nuevo != NULL) {
    v = nuevo;
} else {
    /* v conserva la reserva anterior: gestionar el fallo. */
}
```

Si la reserva cambia, hay que recalcular cualquier puntero interior derivado de ella. Por ejemplo, tras ampliar el bloque datos para dos vectores, se reconstruyen v1 y v2 con la nueva dirección y los tamaños adecuados.

## Segmentación

Un pipeline solapa fases de instrucciones diferentes. Tres fases ocupadas a la vez no significan que se completen tres instrucciones por ciclo.

```text
Ciclo     1       2       3       4       5
Instr. A  buscar  ejecutar guardar
Instr. B          buscar  ejecutar guardar
Instr. C                  buscar  ejecutar guardar
```

En este esquema ideal, tras llenar el pipeline se completa una instrucción por ciclo. Dependencias, conflictos de recursos y saltos pueden introducir esperas. Un procesador superescalar puede emitir más de una instrucción por ciclo; eso es otra característica, no una consecuencia automática de tener tres fases.

## Concurrencia, paralelismo y multitarea

La concurrencia permite que varias tareas progresen en intervalos solapados, incluso alternándose en un núcleo. El paralelismo implica ejecución simultánea en recursos distintos. La multitarea del sistema operativo puede coexistir con una aplicación paralela; no hace falta que esta tenga el procesador en exclusiva para ser paralela.

## Afinidad y caché

La afinidad fija o restringe los procesadores donde puede ejecutarse un trabajador. Puede reducir migraciones y mejorar la localidad, pero no fusiona las cachés L2 de dos CPU en una única caché ni garantiza una aceleración.

La topología de memoria y cachés depende del hardware. Repartir datos y trabajadores con buena localidad requiere considerar esa topología.

## Tiempo, aceleración y eficiencia

Primero mide los tiempos con un reloj apropiado. Después calcula:

| Medida | Expresión | Interpretación |
|---|---|---|
| Aceleración | S(P) = T(1) / T(P) | Relación entre el tiempo de referencia y el paralelo. |
| Eficiencia | E(P) = S(P) / P | Aceleración por recurso utilizado. |
| Escalabilidad | Variar recursos y/o problema | Estudiar cómo cambian tiempo, aceleración y eficiencia. |

La eficiencia no es una lectura directa del porcentaje de CPU ocupado. La escalabilidad puede analizarse con métricas y curvas; no hay una sola cifra universal que la describa en cualquier situación.

Si T(1)=12 s y T(4)=4 s, S(4)=3 y E(4)=0.75, es decir 75 % de eficiencia según esta definición.

## Aclaraciones respecto al PDF

- Memoria dinámica: almacenamiento asignado, no una reserva en la pila.
- Pipeline: fases solapadas, no tres instrucciones completadas por ciclo de forma automática.
- Afinidad: ubicación de la ejecución, no suma física de las cachés.
- Rendimiento: se miden tiempos y después se calculan aceleración y eficiencia.

Referencias: [WG14, reglas de almacenamiento de C](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n2772.pdf) y [afinidad de CPU en Linux](https://www.kernel.org/doc/html/latest/admin-guide/cgroup-v1/cpusets.html).

[Resumen de fundamentos](../../Apuntes/01_fundamentos.md).
