#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int main(int argc,char *argv[])
{
    int nproces, myrank, i;
    int data1,data2,err;
    MPI_Status status;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    data1 = 0;
    data2 = 0;
    if(myrank > 0){
        MPI_Recv(&data1,1,MPI_INT,0,5,MPI_COMM_WORLD,&status);
    }
    else{
      data1 = 33;
      for (i=1;i<nproces;i++){
           MPI_Send(&data1,1,MPI_INT,i,5,MPI_COMM_WORLD);
         }
    }
    printf("Proceso %d. d1: %d - d2: %d.\n",myrank,data1,data2);  
    MPI_Finalize();
}

