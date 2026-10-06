#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4) {
        if (rank == 0) fprintf(stderr, "Ejecuta con 4 procesos.\n");
        MPI_Finalize();
        return 1;
    }

    int datos[10] = {0, 10, 11, 20, 21, 22, 30, 31, 32, 33};
    int counts[4] = {1, 2, 3, 4}, displs[4] = {0, 1, 3, 6};
    int n = rank + 1, local[4];
    const int etiqueta = 107;
    
    if (rank == 0) {
        for (int i = 0; i < counts[0]; i++)
            local[i] = datos[displs[0] + i];
        for (int destino = 1; destino < size; destino++)
            MPI_Send(datos + displs[destino], counts[destino], MPI_INT,
                     destino, etiqueta, MPI_COMM_WORLD);
    } else {
        MPI_Recv(local, n, MPI_INT, 0, etiqueta, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
    printf("P%d:", rank);
    for (int i = 0; i < n; i++) printf(" %d", local[i]);
    printf("\n");

    MPI_Finalize();
    return 0;
}
