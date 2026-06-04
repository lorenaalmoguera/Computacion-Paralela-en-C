#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int main(int argc,char *argv[])
{
    int nproces, myrank, i;
    double data1,data2;
    MPI_Status status;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    data1 = 0;
    if(myrank == 0){
      for (i=1;i<nproces;i++){
           data1 = i * 10;
           MPI_Send(&data1,1,MPI_DOUBLE,i,5,MPI_COMM_WORLD);
         }
    }
    else{
        MPI_Recv(&data2,1,MPI_DOUBLE,0,5,MPI_COMM_WORLD,&status);
    }
    printf("Proceso %d. d1: %f - d2: %f.\n",myrank,data1,data2);  
    MPI_Finalize();
}

