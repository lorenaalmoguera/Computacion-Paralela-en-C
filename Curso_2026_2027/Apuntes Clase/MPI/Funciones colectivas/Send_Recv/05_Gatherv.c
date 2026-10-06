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

    int n = rank + 1, local[4], datos[10];
    int counts[4] = {1, 2, 3, 4};
    int displs[4] = {0, 1, 3, 6};
    const int etiqueta = 105;
    for (int i = 0; i < n; i++) local[i] = rank * 10 + i;
    
    if (rank == 0) {
        for (int i = 0; i < counts[0]; i++)
            datos[displs[0] + i] = local[i];
        for (int origen = 1; origen < size; origen++)
            MPI_Recv(datos + displs[origen], counts[origen], MPI_INT,
                     origen, etiqueta, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < 10; i++) printf("%d ", datos[i]);
        printf("\n");
    } else {
        MPI_Send(local, n, MPI_INT, 0, etiqueta, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
