#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){
    
    // Parte 1
    /*
    int arr[] = {10, 20, 30, 40, 50};
    int *p = arr;

    printf("%d\n", *p);
    printf("%d\n", *(p+2));
    printf("%d\n", *(p+4));
    printf("%d\n", *p+2);

    *(p+1) = 99; // arr[1] = 99
    *(p+3) = *(p+0) + *(p+2); // 10 + 30 = 40
    
    for(int i = 0 ; i < 5 ; i++){
        printf("%d ", arr[i]);
    }

    printf("\n");
    */

    // Parte 2
    /*
    
        double v[] = {1.0, 2.0, 3.0};
        double *q = v; // 1.0

        for(int i = 0 ; i < 3 ; i++) *(q + i) = *(q + i) * 2; // -> 2 --> 2, 4, 6
        for(int i = 0 ; i < 3 ; i++) printf("%.1f ", v[i]);

        printf("\n");
    */

    // Parte 3
    
    // Matriz dinámica 3X3
    double **m = (double**)malloc(3 * sizeof(double*));
    for(int i = 0; i < 3; i++) m[i] = (double*)malloc(3 * sizeof(double));

    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            *(*(m+i)+j) = 0;
        }
    }

    *(*(m + 0) + 0) = 1.0;
    *(*(m + 0) + 2) = 3.0;
    *(*(m + 1) + 1) = 5.0;
    *(*(m + 2) + 0) = 7.0;

    // m[0,0] -> si = 1.0 ; m[0, 1] -> no ; m[0,2] -> si = 3.0
    // m[1,0] -> no ; m[1, 1] -> si = 5.0 ; m[1,2] -> no
    // m[2,0] -> si = 7.0 ; m[2, 1] -> no ; m[2, 2] -> no


    printf("Parte 3:\n");
    printf("m[0][0] = %.1f\n", *(*(m + 0) + 0));
    printf("m[0][2] = %.1f\n", *(*(m + 0) + 2));
    printf("m[1][1] = %.1f\n", *(*(m + 1) + 1));
    printf("m[2][0] = %.1f\n", *(*(m + 2) + 0));

    printf("\n\n");

    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            printf("M[%d, %d] = %.1f  \t", i, j, *(*(m+i)+j));
        }
        printf("\n");
    }
    
    return 0;
}