#include <omp.h>
#include <stdio.h>
int main()
{
   int iam =22, np = 33;
#pragma omp parallel num_threads(4) private(np) default(none)
{
    #pragma omp single
    printf("\t\tDENTRO valor de iam y np: %d -  %d\n",iam,np);

    iam = omp_get_thread_num();  
    np = omp_get_num_threads();
    printf("\tSoy el thread %d de un grupo de %d\n",iam,np);
}//parallel
printf("\t\tFUERA valor de iam y np: %d -  %d\n",iam,np);
}
