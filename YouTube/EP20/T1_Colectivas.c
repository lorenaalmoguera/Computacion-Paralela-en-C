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

    MPI_Bcast(&dim, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int * send_counts = (int*)malloc(nproces*sizeof(int));
    int * despl_counts = (int*)malloc(nproces*sizeof(int));


    int num_elementos = dim / nproces;

    if(myrank == 0){
        for(int i = 0 ; i < nproces ; i++){
            send_counts[i] = num_elementos;
        }
        send_counts[0] += dim % nproces;
        despl_counts[0] = 0;
        for(int i = 1 ; i < nproces ; i++ ){
            despl_counts[i] = despl_counts[i-1] + send_counts[i-1];
        }

    }

    MPI_Bcast(send_counts, nproces, MPI_INT, 0, MPI_COMM_WORLD);

    num_elementos = send_counts[myrank];

    printf("Proceso: %d tiene %d elementos.\n", myrank, num_elementos);

    vector_parcial = (int*)malloc(num_elementos*sizeof(int));

    MPI_Scatterv(vector, send_counts, despl_counts, MPI_INT, vector_parcial, send_counts[myrank], MPI_INT, 0, MPI_COMM_WORLD);


    int cont_impares = 0 , cont_pares = 0 , cont_19 = 0;

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

    int total_impares, total_pares_0, total_cont_19;

    MPI_Reduce(&cont_impares, &total_impares, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cont_pares, &total_pares_0, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cont_19, &total_cont_19, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    if(myrank == 0) printf("Impares total %d, Pares distintos de 0 totales %d, totale de numeros cuyas cifras suman 19 %d\n", total_impares, total_pares_0, total_cont_19);

    free(vector);
    free(vector_parcial);
    free(send_counts);
    free(despl_counts);

    MPI_Finalize();

    return 0;
}