#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char* argv[]){

    int myrank, nproces;
    int dim;
    int *vector = NULL;
    int *vector_parcial = NULL;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproces);

    MPI_Status status;

    if(myrank == 0){

        dim = 6000;
        srand(time(NULL));
        vector = (int*)malloc(dim*sizeof(int));

        for(int i = 0 ; i < dim ; i++){
            do{
                *(vector+i) = rand() % 2001 - 10000;
            }while(*(vector+i) == 0);
        }

        for(int i = 0 ; i < 100 ; i++){
            *(vector + i) = 0;
        }

        for(int i = dim - 1 ; i > 0 ; i--){
            int j = rand() % (i+1);
            int temp = *(vector+i);
            *(vector+i) = *(vector+j);
            *(vector+j) = temp;
        }

    }

    if(myrank == 0){
        for(int i = 1 ; i < nproces ; i++){
            MPI_Send(&dim, 1, MPI_INT, i, 10, MPI_COMM_WORLD);
        }
    }else{
        MPI_Recv(&dim, 1, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
    }


    int num_elementos = dim / nproces;
    int resto = dim % nproces;

    int elementos_base = num_elementos;
    if(myrank == 0) num_elementos += resto;

    vector_parcial = (int*)malloc(num_elementos*sizeof(int));

    if(myrank == 0){
        int pos = num_elementos;
        for(int i = 1 ; i < nproces ; i++){
            MPI_Send(vector+pos, elementos_base, MPI_INT, i, 15, MPI_COMM_WORLD);
            pos += elementos_base;
        }

        for(int i = 0 ; i < num_elementos ; i++){
            vector_parcial[i] = vector[i];
        }

    }else{
        MPI_Recv(vector_parcial, num_elementos, MPI_INT, 0, 15, MPI_COMM_WORLD, &status);
    }

    int cont_impares = 0 , cont_pares = 0, cont_19 = 0;

    for(int i = 0 ; i < num_elementos ; i++){
        if(vector_parcial[i] % 2 == 0){
            if(vector_parcial[i] != 0) cont_pares++;
        }else{
            cont_impares++;
        }

        int numero = abs(vector_parcial[i]);
        int suma = 0;
        while(numero > 0){
            suma += numero % 10;
            numero /= 10;
        }

        if (suma == 19) cont_19++;
    }

    if(myrank != 0){
        MPI_Send(&cont_impares, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&cont_pares, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&cont_19, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }else{
        int total_impares = cont_impares, total_pares = cont_pares, total_19 = cont_19;
        int aux1, aux2, aux3;

        for(int i = 1 ; i < nproces ; i++){
            MPI_Recv(&aux1, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            total_impares += aux1;
            MPI_Recv(&aux2, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            total_pares += aux2;
            MPI_Recv(&aux3, 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status);
            total_19 += aux3;
        }

        printf("Impares totales %d, Pares distintos de 0 totales %d, total de numeros cuyas cifras suman 19 %d\n", total_impares, total_pares, total_19);
    }

    free(vector);
    free(vector_parcial);

    MPI_Finalize();

    return 0;
}