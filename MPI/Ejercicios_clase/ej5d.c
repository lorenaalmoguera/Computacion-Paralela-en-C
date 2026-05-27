#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int main(int argc,char *argv[])
{
    int nproces, myrank, i, err;
    int data1,data2;
    MPI_Status status;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    data1 = 0;
    data2 = 0;
    if (myrank == 0) data1 = 33;
    err = MPI_Bcast(&data1,1,MPI_INTEGER,0,MPI_COMM_WORLD);
    printf("Proceso %d. d1: %d - d2: %d.\n",myrank,data1,data2);  
    MPI_Finalize();
}

