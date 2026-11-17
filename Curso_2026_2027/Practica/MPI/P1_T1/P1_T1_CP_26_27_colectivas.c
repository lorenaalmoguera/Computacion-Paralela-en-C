#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


int main(int argc, char* argv[]){

    int myrank, nproces;
    int dim = 6000;
    int * vector = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproces);

    if(myrank == 0){
        srand(time(NULL));
        vector = (int*)malloc(dim*sizeof(int));

        for(int i = 0 ; i < dim ; i++){
            // Generamos valores distintos de cero para tener exactamente 100 ceros.
            do{
                *(vector+i) = rand() % 20001 - 10000;
            }while(*(vector+i) == 0);
        }
        
        for(int i = 0 ; i < 100 ; i++){
            *(vector+i) = 0;
        }

        for(int i = dim - 1 ; i > 0 ; i--){
            int j = rand() % (i+1);
            int temp = *(vector+i);
            *(vector+i) = *(vector+j);
            *(vector+j) = temp;
        }
    }

    MPI_Bcast(&dim, 1, MPI_INT, 0, MPI_COMM_WORLD);


    int * cnt_p = (int*)malloc(nproces*sizeof(int));
    int * disp_p = (int*)malloc(nproces*sizeof(int));

    int num_elementos = dim / nproces;
    if(myrank == 0){
        for(int i = 0 ; i < nproces ; i++){
            cnt_p[i] = num_elementos;
        }
        cnt_p[0] += dim % nproces;
        disp_p[0] = 0;
        for(int i = 1 ; i < nproces ; i++){
            disp_p[i] = disp_p[i-1] + cnt_p[i-1];
        }
    }

    MPI_Bcast(cnt_p, nproces, MPI_INT, 0, MPI_COMM_WORLD);
    // disp_p solo se utiliza en el proceso raiz de MPI_Scatterv.

    num_elementos = cnt_p[myrank];

    printf("Proceso: %d tiene %d elementos.\n", myrank, num_elementos);

    int *vector_parcial = (int*)malloc(num_elementos*sizeof(int));

    MPI_Scatterv(vector, cnt_p, disp_p, MPI_INT, vector_parcial, cnt_p[myrank], MPI_INT, 0, MPI_COMM_WORLD);

    int cont_impares = 0, cont_pares = 0, cont_19 = 0;
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

        if(suma == 19) cont_19++;
    }

    int total_impares, total_pares_0, total_cont_19;

    MPI_Reduce(&cont_impares, &total_impares, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cont_pares, &total_pares_0, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&cont_19, &total_cont_19, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(myrank == 0){
        printf("Impares totales %d, Pares distintos de 0 totales %d, total de numeros cuyas cifras suman 19 %d\n", total_impares, total_pares_0, total_cont_19);
    }

    free(vector);
    free(vector_parcial);
    free(cnt_p);
    free(disp_p);

    MPI_Finalize();
    return 0;
}
