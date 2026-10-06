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

    int local = rank + 1;
    int total = 0;
    const int etiqueta = 102;
    
    if (rank == 0) {
        total = local; /* La raiz tambien aporta su valor. */
        for (int origen = 1; origen < size; origen++) {
            int recibido;
            MPI_Recv(&recibido, 1, MPI_INT, origen, etiqueta,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += recibido;
        }
        printf("Total: %d\n", total);
    } else {
        MPI_Send(&local, 1, MPI_INT, 0, etiqueta, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
