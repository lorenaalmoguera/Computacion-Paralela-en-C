#include <omp.h>
#include <stdio.h>

int main()
{
   int iam, np=0, i, prod_int=0;
   int data1[10];
   int data2[10];
   for (i=0;i<10;i++){
      data1[i] = i;
      data2[i] = i*2;
    }
 
#pragma omp parallel num_threads(4) shared(prod_int,data1,data2) private(np,iam,i)  default(none)
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();
    #pragma omp for schedule(static,2) reduction(+:prod_int)
    for (i=0;i<10;i++){
        prod_int += data1[i] * data2[i];    
    }
}//parallel
printf("\t\tFUERA valor de prod_int: %d\n",prod_int);
}
