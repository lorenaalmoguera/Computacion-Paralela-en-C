#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 4) {
        if (rank == 0) fprintf(stderr, "Ejecuta con 4 procesos.\n");
        MPI_Finalize(); return 1;
    }

    int local[2] = {10 * (rank + 1), 10 * (rank + 1) + 1};
    int total[8];
    const int tag = 111;

    if (rank == 0) {
        total[0] = local[0]; total[1] = local[1];
        for (int src = 1; src < size; ++src)
            MPI_Recv(&total[2 * src], 2, MPI_INT, src, tag,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int dst = 1; dst < size; ++dst)
            MPI_Send(total, 8, MPI_INT, dst, tag, MPI_COMM_WORLD);
    } else {
        MPI_Send(local, 2, MPI_INT, 0, tag, MPI_COMM_WORLD);
        MPI_Recv(total, 8, MPI_INT, 0, tag,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    printf("P%d:", rank);
    for (int i = 0; i < 8; ++i) printf(" %d", total[i]);
    printf("\n");
    MPI_Finalize();
    return 0;
}
