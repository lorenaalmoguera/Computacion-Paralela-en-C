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

    int counts[4], displs[4], total_n = 0;
    for (int i = 0; i < size; ++i) {
        counts[i] = i + 1;
        displs[i] = total_n;
        total_n += counts[i];
    }
    int local[4], all[10];
    for (int i = 0; i < counts[rank]; ++i)
        local[i] = 10 * (rank + 1) + i;
    const int gather_tag = 121, share_tag = 122;

    if (rank == 0) {
        for (int i = 0; i < counts[0]; ++i) all[displs[0] + i] = local[i];
        for (int src = 1; src < size; ++src)
            MPI_Recv(&all[displs[src]], counts[src], MPI_INT, src, gather_tag,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int dst = 1; dst < size; ++dst)
            MPI_Send(all, total_n, MPI_INT, dst, share_tag, MPI_COMM_WORLD);
    } else {
        MPI_Send(local, counts[rank], MPI_INT, 0, gather_tag, MPI_COMM_WORLD);
        MPI_Recv(all, total_n, MPI_INT, 0, share_tag,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    printf("P%d:", rank);
    for (int i = 0; i < total_n; ++i) printf(" %d", all[i]);
    printf("\n");
    MPI_Finalize();
    return 0;
}
