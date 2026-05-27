#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
 
int N = 12000;
//Programa valido para N divisible entero por np
int main(int argc,char *argv[])
{
    int nproces, myrank, i;
    int *vector, prodint,temp;
    int ini,fin,tamBloque;
    MPI_Status status;

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD,&nproces);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    
    tamBloque = N/nproces;
    prodint = 0;
    if (myrank==0){
	    vector = (int*)malloc(N*sizeof(vector[0]));    	
	    for (i=0;i<N;i++){
		    vector[i] = i+1;
	    }
	    ini = 0;
	    fin = tamBloque;
    }
    else{
	    vector = (int*)malloc(tamBloque*sizeof(vector[0]));    	
	    ini = tamBloque*myrank;
	    //Si no divisible hay que cambiar "fin" si 
	    //12000/np no es divisible para myrank = nproces - 1
	    fin = tamBloque*(myrank+1);
	    }
    if (myrank == 0){
	 	for (i=1;i<nproces;i++){  
		 	MPI_Send(&vector[tamBloque*i],tamBloque,MPI_INT,i,5,MPI_COMM_WORLD);	
	 	}
    }
	else{
		MPI_Recv(vector,tamBloque,MPI_INT,0,5,MPI_COMM_WORLD,&status);
	}    

	for (i=0;i<tamBloque;i++){
		prodint += vector[i]*vector[i];
		}
		    
    if (myrank==0){
        for (i=1;i<nproces;i++){
        	MPI_Recv(&temp,1,MPI_INT,i,5,MPI_COMM_WORLD,&status);
        	prodint += temp;
         }    
     }
    else{
	    MPI_Send(&prodint,1,MPI_INT,0,5,MPI_COMM_WORLD);
    }	    
    
    printf("Proceso %d. Producto interno: %d.\n",myrank,prodint);  
    MPI_Finalize();
}
