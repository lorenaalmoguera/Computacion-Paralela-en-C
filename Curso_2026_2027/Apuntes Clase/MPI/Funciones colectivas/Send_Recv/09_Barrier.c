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

    int token = 1;
    const int llegada = 110, permiso = 111;
    printf("P%d antes\n", rank);
    
    if (rank == 0) {
        /* Nadie recibe permiso hasta que todos hayan llegado. */
        for (int origen = 1; origen < size; origen++)
            MPI_Recv(&token, 1, MPI_INT, origen, llegada,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int destino = 1; destino < size; destino++)
            MPI_Send(&token, 1, MPI_INT, destino, permiso, MPI_COMM_WORLD);
    } else {
        MPI_Send(&token, 1, MPI_INT, 0, llegada, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, 0, permiso, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
    printf("P%d despues\n", rank);

    MPI_Finalize();
    return 0;
}
