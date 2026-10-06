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
    const int llegada = 102, reparto = 103;
    
    if (rank == 0) {
        /* Fase 1: recoger y sumar las aportaciones. */
        total = local;
        for (int origen = 1; origen < size; origen++) {
            int recibido;
            MPI_Recv(&recibido, 1, MPI_INT, origen, llegada,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += recibido;
        }
        /* Fase 2: enviar el resultado a todos. */
        for (int destino = 1; destino < size; destino++)
            MPI_Send(&total, 1, MPI_INT, destino, reparto, MPI_COMM_WORLD);
    } else {
        MPI_Send(&local, 1, MPI_INT, 0, llegada, MPI_COMM_WORLD);
        MPI_Recv(&total, 1, MPI_INT, 0, reparto, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
    printf("P%d: %d\n", rank, total);

    MPI_Finalize();
    return 0;
}
