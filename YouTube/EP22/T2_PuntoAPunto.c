#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char* argv[]){


    int myrank, nproces, dim;
    int *vector = NULL;
    double *resultado = NULL;

    MPI_Init(&argc, &argv);

    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproces);

    if(myrank == 0){
        dim = atoi(argv[1]);

        srand(time(NULL));
        vector = (int*)malloc(dim*sizeof(int));
        resultado = (double*)malloc(dim*sizeof(double));

        for(int i = 0 ; i < dim ; i++){
            *(vector+i) = rand() % 100001;
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
    if(myrank == 0) elementos_base += resto;

    int * vector_parcial = (int*)malloc(elementos_base*sizeof(int));
    double * resultado_parcial = (double*)malloc(elementos_base*sizeof(double));

    if(myrank == 0){
        int pos = elementos_base;
        for(int i = 1 ; i < nproces ; i++){
            MPI_Send(vector+pos, num_elementos, MPI_INT, i, 15, MPI_COMM_WORLD);
        }

        for(int i = 0 ; i < elementos_base ; i++){
            vector_parcial[i] = vector[i];
        }

    }else{
        MPI_Recv(vector_parcial, elementos_base, MPI_INT, 0, 15, MPI_COMM_WORLD, &status);
    }


    int mi_maximo = 0;
    for(int i = 0 ; i < elementos_base ; i++){
        if(vector_parcial[i] > mi_maximo) mi_maximo = vector_parcial[i];
    }

    int maximo = mi_maximo;

    if(myrank != 0){
        MPI_Send(&mi_maximo, 1, MPI_INT, 0, 11, MPI_COMM_WORLD);
        MPI_Recv(&maximo, 1, MPI_INT, 0, 12, MPI_COMM_WORLD, &status);
    }else{
        int aux;
        for(int i = 1 ; i < nproces ; i++){
            MPI_Recv(&aux, 1, MPI_INT, i, 11, MPI_COMM_WORLD, &status);
            if(aux > maximo) maximo = aux;
        }

        for(int i = 1 ; i < nproces ; i++){
            MPI_Send(&maximo, 1, MPI_INT, i, 12, MPI_COMM_WORLD);
        }
    }

    int mi_cont = 0;

    for(int i = 0 ; i < elementos_base ; i++){
        if(maximo == 0){
            resultado_parcial[i] = 0.0;
        }else{
            resultado_parcial[i] = (double)vector_parcial[i] / maximo;
        }
        if(resultado_parcial[i] > 0.5) mi_cont++;
    }

    int contador;
        
    if(myrank != 0){
        MPI_Send(&mi_cont, 1, MPI_INT, 0, 13, MPI_COMM_WORLD);
    }else{
        contador = mi_cont;
        for(int i = 1 ; i < nproces ; i++){
            MPI_Recv(&mi_cont,1, MPI_INT, i, 13, MPI_COMM_WORLD, &status);
            contador+=mi_cont;
        }
    }


    if(myrank == 0){
        for(int i = 0 ; i < elementos_base ; i++){
            resultado[i] = resultado_parcial[i];
        }

        int pos = elementos_base;

        for(int i = 1 ; i < nproces ; i++){
            MPI_Recv(resultado + pos, num_elementos, MPI_DOUBLE, i, 16, MPI_COMM_WORLD, &status);
            pos += num_elementos;
        }
    }else{
        MPI_Send(resultado_parcial, elementos_base, MPI_DOUBLE, 0, 16, MPI_COMM_WORLD);
    }
    

    if(myrank == 0){
        for(int i = 0 ; i < dim ; i++){
            printf("%.2f ", resultado[i]);
            
        }
        printf("\nTotal de valores mayores a 0.5: %d\n", contador);
        printf("Maximo del vector: %d\n", maximo);

        char * nombre = "resultado_colectivas.bin";
        if(argc == 3){
            nombre = argv[2];
        }
        FILE *fichero = fopen(nombre, "wb");
        fwrite(resultado, sizeof(double), dim, fichero);
        fclose(fichero);
    }

    free(resultado);
    free(vector);
    free(vector_parcial);
    free(resultado_parcial);

    MPI_Finalize();

    return 0;
}