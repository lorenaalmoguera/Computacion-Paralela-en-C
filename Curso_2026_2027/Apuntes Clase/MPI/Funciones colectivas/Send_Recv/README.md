# Colectivas con Send y Recv

Cada ejemplo es un programa independiente: toda la implementación está dentro de su main y muestra las llamadas MPI_Send y MPI_Recv. No hay cabeceras propias ni funciones auxiliares. Se incluyen únicamente mpi.h (necesaria para usar MPI) y stdio.h (para imprimir).

## Índice

| Operación | Diagrama, colectiva original y explicación punto a punto | Programa independiente |
|---|---|---|
| MPI_Bcast | [01_Bcast.md](01_Bcast.md) | [01_Bcast.c](01_Bcast.c) |
| MPI_Reduce | [02_Reduce.md](02_Reduce.md) | [02_Reduce.c](02_Reduce.c) |
| MPI_Allreduce | [03_Allreduce.md](03_Allreduce.md) | [03_Allreduce.c](03_Allreduce.c) |
| MPI_Gather | [04_Gather.md](04_Gather.md) | [04_Gather.c](04_Gather.c) |
| MPI_Gatherv | [05_Gatherv.md](05_Gatherv.md) | [05_Gatherv.c](05_Gatherv.c) |
| MPI_Scatter | [06_Scatter.md](06_Scatter.md) | [06_Scatter.c](06_Scatter.c) |
| MPI_Scatterv | [07_Scatterv.md](07_Scatterv.md) | [07_Scatterv.c](07_Scatterv.c) |
| MPI_Reduce_scatter | [08_Reduce_scatter.md](08_Reduce_scatter.md) | [08_Reduce_scatter.c](08_Reduce_scatter.c) |
| MPI_Barrier | [09_Barrier.md](09_Barrier.md) | [09_Barrier.c](09_Barrier.c) |
| MPI_Scan | [10_Scan.md](10_Scan.md) | [10_Scan.c](10_Scan.c) |

## Cómo estudiar cada ejemplo

1. Mira el diagrama y los datos que obtiene cada proceso.
2. Compara el bloque que usa la colectiva original con el programa que usa mensajes punto a punto.
3. Sigue los envíos, recepciones, etiquetas y operaciones locales descritos en la explicación.

## Alcance de los programas

Los diez programas requieren exactamente cuatro procesos, trabajan con MPI_INT y usan P0 como raíz o coordinador cuando hace falta. Las reducciones usan suma. Son ejemplos concretos de la funcionalidad, con tamaños fijos: no reproducen toda la API MPI ni sus algoritmos internos. En Reduce_scatter cada proceso recibe un entero.

Todos los procesos deben ejecutar el algoritmo correspondiente. Las etiquetas de estos ejemplos deben reservarse para sus mensajes y no mezclarse con tráfico ajeno en el mismo comunicador. Las fases de recogida y reparto usan etiquetas diferentes. El orden evita ciclos de espera incluso si Send espera a su Recv.

## Compilación

Desde esta carpeta y con MPI instalado, por ejemplo:

```bash
mpicc -std=c99 03_Allreduce.c -o 03_Allreduce
mpiexec -n 4 ./03_Allreduce
```

Cada página contiene el comando para su programa. El orden de las líneas impresas puede variar.

## Validación

Los diez programas pasan la comprobación de sintaxis C99. Sus bloques punto a punto también superan una simulación local con cuatro procesos y mensajes síncronos: se verifican resultados, etiquetas, cantidades y que la barrera espere todas las llegadas. La ejecución con MPI real queda pendiente porque no está instalado en este entorno.
