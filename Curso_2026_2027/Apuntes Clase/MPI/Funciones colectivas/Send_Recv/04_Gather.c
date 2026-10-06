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

    int local = (rank + 1) * 10;
    int datos[4];
    const int etiqueta = 104;
    
    if (rank == 0) {
        datos[0] = local; /* Copia local de la aportacion de P0. */
        for (int origen = 1; origen < size; origen++)
            MPI_Recv(&datos[origen], 1, MPI_INT, origen, etiqueta,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < size; i++) printf("%d ", datos[i]);
        printf("\n");
    } else {
        MPI_Send(&local, 1, MPI_INT, 0, etiqueta, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
