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

    int dato = rank == 0 ? 42 : 0;
    const int etiqueta = 101;
    
    if (rank == 0) {
        for (int destino = 1; destino < size; destino++)
            MPI_Send(&dato, 1, MPI_INT, destino, etiqueta, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&dato, 1, MPI_INT, 0, etiqueta, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
    printf("P%d: %d\n", rank, dato);

    MPI_Finalize();
    return 0;
}
