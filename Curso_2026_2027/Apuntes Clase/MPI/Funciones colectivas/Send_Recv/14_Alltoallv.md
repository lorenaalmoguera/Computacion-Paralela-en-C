# MPI_Alltoallv con Send y Recv

## Diagrama

```mermaid
flowchart LR
    A["Cada origen prepara un bloque por destino"] --> B["MPI_Send / MPI_Recv<br/>en parejas i<j"]
    B --> C["Cada receptor coloca el bloque<br/>en el desplazamiento del origen"]
```

## Funcionamiento punto a punto

`MPI_Alltoallv` permite tamaños distintos en cada pareja. En Pi, `sendcounts[j]` y `sdispls[j]` identifican el mensaje dirigido a Pj. En Pj, `recvcounts[i]` y `rdispls[i]` describen dónde recibir el mensaje de Pi. Los conteos deben concordar en cada pareja. Este programa calcula tamaños deterministas y compacta sus bloques en buffers locales.

Se recorren las parejas `(i,j)`, `i < j`, con orden complementario de envío y recepción. Un conteo cero representa un mensaje vacío. El bloque propio se copia localmente. Los desplazamientos son índices de elementos `MPI_INT`.

## Código

Programa completo: [14_Alltoallv.c](14_Alltoallv.c).

```c
/* Pi envía a Pj y recibe el bloque de Pj. */
MPI_Send(&sendbuf[sdispls[j]], sendcounts[j], MPI_INT,
         j, tag, MPI_COMM_WORLD);
MPI_Recv(&recvbuf[rdispls[j]], recvcounts[j], MPI_INT,
         j, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
```

En Pj se ejecuta primero la recepción del bloque de Pi, con `recvcounts[i]` y `rdispls[i]`, y luego el envío propio a Pi.

## Comparación con la colectiva

```c
MPI_Alltoallv(sendbuf, sendcounts, sdispls, MPI_INT,
              recvbuf, recvcounts, rdispls, MPI_INT, MPI_COMM_WORLD);
```

## Compilar

```bash
mpicc -std=c99 14_Alltoallv.c -o 14_Alltoallv
mpiexec -n 4 ./14_Alltoallv
```

[Volver al índice](README.md)
