#include <omp.h>
#include <stdio.h>
int main()
{
   int iam =0, np = 1;
   int datos[100],i;

#pragma omp parallel num_threads(2) private(iam,np,i)
{
#if defined (_OPENMP)
    np = omp_get_num_threads();
    iam = omp_get_thread_num();
#endif
    printf("Hola Mundo: soy el proceso %d de %d procesos.\n",iam,np);
    #pragma omp for schedule(dynamic,2)
    for(i=0;i<10;i++)
     {
       printf("Hilo %d, realiza iteracion  %d \n",iam,i);
     }
}//parallel

}
