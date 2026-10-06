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

    int local = rank + 1, prefijo;
    const int etiqueta = 112;
    
    if (rank == 0) {
        prefijo = local; /* P0 inicia la cadena. */
    } else {
        MPI_Recv(&prefijo, 1, MPI_INT, rank - 1, etiqueta,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        prefijo += local;
    }
    if (rank + 1 < size)
        MPI_Send(&prefijo, 1, MPI_INT, rank + 1, etiqueta, MPI_COMM_WORLD);
    printf("P%d: %d\n", rank, prefijo);

    MPI_Finalize();
    return 0;
}
