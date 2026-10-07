# Funciones colectivas de MPI

Apuntes con diagramas Mermaid, explicaciones y ejemplos completos en C. Si el visor no dibuja Mermaid, cada página incluye también una lectura textual del diagrama.

## Índice

| Función | Qué hace | Resultado | Implementación con `Send` y `Recv` |
|---|---|---|---|
| [`MPI_Bcast`](01_Bcast.md) | Difunde el mismo dato desde la raíz a todos los procesos. | En todos, según la operación | [Explicación](Send_Recv/01_Bcast.md) · [Código C](Send_Recv/01_Bcast.c) |
| [`MPI_Reduce`](02_Reduce.md) | Combina las contribuciones mediante una operación y deja el resultado en la raíz. | En la raíz | [Explicación](Send_Recv/02_Reduce.md) · [Código C](Send_Recv/02_Reduce.c) |
| [`MPI_Allreduce`](03_Allreduce.md) | Reduce las contribuciones y devuelve el resultado a todos. | En todos, según la operación | [Explicación](Send_Recv/03_Allreduce.md) · [Código C](Send_Recv/03_Allreduce.c) |
| [`MPI_Gather`](04_Gather.md) | Reúne bloques de igual tamaño en la raíz, ordenados por rango. | En la raíz | [Explicación](Send_Recv/04_Gather.md) · [Código C](Send_Recv/04_Gather.c) |
| [`MPI_Gatherv`](05_Gatherv.md) | Reúne bloques de diferentes tamaños en la raíz. | En la raíz | [Explicación](Send_Recv/05_Gatherv.md) · [Código C](Send_Recv/05_Gatherv.c) |
| [`MPI_Scatter`](06_Scatter.md) | Divide los datos de la raíz en bloques iguales y entrega uno a cada proceso. | En todos, según la operación | [Explicación](Send_Recv/06_Scatter.md) · [Código C](Send_Recv/06_Scatter.c) |
| [`MPI_Scatterv`](07_Scatterv.md) | Reparte bloques de tamaños diferentes desde la raíz. | En todos, según la operación | [Explicación](Send_Recv/07_Scatterv.md) · [Código C](Send_Recv/07_Scatterv.c) |
| [`MPI_Reduce_scatter`](08_Reduce_scatter.md) | Primero reduce vectores elemento a elemento y después reparte el vector reducido. | En todos, según la operación | [Explicación](Send_Recv/08_Reduce_scatter.md) · [Código C](Send_Recv/08_Reduce_scatter.c) |
| [`MPI_Barrier`](09_Barrier.md) | Sincroniza: nadie termina la llamada hasta que todos han entrado en ella. | Sincronización | [Explicación](Send_Recv/09_Barrier.md) · [Código C](Send_Recv/09_Barrier.c) |
| [`MPI_Scan`](10_Scan.md) | Calcula una reducción acumulada inclusiva siguiendo el orden de los rangos. | En todos, según la operación | [Explicación](Send_Recv/10_Scan.md) · [Código C](Send_Recv/10_Scan.c) |

## Reglas comunes

- Todos los procesos del comunicador deben participar y llamar a las colectivas en un orden compatible.
- En las operaciones con raíz, todos deben indicar la misma raíz y comunicador.
- Los tipos y cantidades de envío y recepción deben ser compatibles, y los búferes deben tener espacio suficiente.
- Una colectiva bloqueante permite reutilizar sus búferes locales al terminar, pero no implica que todos los demás procesos hayan terminado. No hay que suponer que cualquier colectiva funciona como una barrera.
- Los bloques se asignan por rango, no por el orden en que los procesos llegan.
- Los ejemplos usan `MPI_INT` para enteros de C y requieren exactamente cuatro procesos. El orden de la salida por consola puede variar.

## Diferencias para recordar

- Bcast copia el mismo bloque a todos; Scatter reparte bloques distintos.
- Gather reúne valores sin operar sobre ellos; Reduce los combina.
- Reduce devuelve el resultado a la raíz; Allreduce lo devuelve a todos.
- Las variantes con `v` permiten cantidades y desplazamientos distintos por proceso.
- Reduce_scatter reduce un vector y reparte el resultado; Scan calcula prefijos acumulados.
- Barrier sincroniza sin transferir datos del programa.

`MPI_Scan` se incluye porque aparece en los apuntes de clase del 6 de octubre. `MPI_Barrier` aparece comentada en el ejercicio de grupos.

## Referencia

Semántica contrastada con la documentación del [MPI Forum: comunicaciones colectivas](https://www.mpi-forum.org/docs/mpi-3.1/mpi31-report/node95.htm). Los diagramas describen el resultado conceptual de cada operación.

## Implementaciones con Send y Recv

En [Send_Recv](Send_Recv/README.md) tienes las diez operaciones implementadas con mensajes punto a punto, diagramas y programas completos en C.
