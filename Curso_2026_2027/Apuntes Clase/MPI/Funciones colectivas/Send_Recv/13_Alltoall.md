# MPI_Alltoall con Send y Recv

## Diagrama

```mermaid
flowchart LR
    A["Pi envía su bloque a Pj"] --> B["Pj recibe el bloque de Pi"]
    B --> C["Para cada pareja i<j:<br/>i envía y recibe;<br/>j recibe y envía"]
    C --> D["Cada proceso ordena lo recibido<br/>por rango de origen"]
```

## Funcionamiento punto a punto

Cada proceso tiene un bloque para cada destino, incluidos los datos destinados a sí mismo. Para evitar que ambos extremos de una pareja ejecuten un envío bloqueante a la vez, se ordenan las parejas `(i,j)`, con `i < j`: Pi hace `MPI_Send` y luego `MPI_Recv`; Pj hace primero `MPI_Recv` y después `MPI_Send`. Los demás procesos avanzan por el mismo bucle hasta que les toca su pareja.

El ejemplo intercambia dos enteros por pareja. El bloque del origen `i` dirigido a `j` se guarda en el receptor en el espacio del origen `i`. Las etiquetas deben coincidir y estar reservadas para este tráfico.

## Código

Programa completo: [13_Alltoall.c](13_Alltoall.c).

```c
/* Pi */
MPI_Send(&sendbuf[2 * j], 2, MPI_INT, j, tag, MPI_COMM_WORLD);
MPI_Recv(&recvbuf[2 * j], 2, MPI_INT, j, tag,
         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

/* Pj */
MPI_Recv(&recvbuf[2 * i], 2, MPI_INT, i, tag,
         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
MPI_Send(&sendbuf[2 * i], 2, MPI_INT, i, tag, MPI_COMM_WORLD);
```

## Comparación con la colectiva

```c
MPI_Alltoall(sendbuf, 2, MPI_INT,
             recvbuf, 2, MPI_INT, MPI_COMM_WORLD);
```

La colectiva es más directa y normalmente debe preferirse en código de aplicación. Este algoritmo por parejas ayuda a ver el intercambio y el orden por rango.

## Compilar

```bash
mpicc -std=c99 13_Alltoall.c -o 13_Alltoall
mpiexec -n 4 ./13_Alltoall
```

[Volver al índice](README.md)
