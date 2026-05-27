#include <omp.h>
#include <stdio.h>

int main()
{
   int iam, np=0, i, tp;
 
#pragma omp parallel num_threads(4)  private(np,iam,i,tp)  default(none)
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();
    tp = (iam + 10) * 2;
    printf("\tDENTRO valor de tp: %d - en proceso %d\n",tp,iam);
    
    #pragma omp single copyprivate(tp)
     {
     tp = (iam + 1) * 100; 
     printf("\tDENTRO SINGLE valor de tp: %d - en proceso %d\n",tp,iam);
     }

    printf("\tTRAS SINGLE valor de tp: %d - en proceso %d\n",tp,iam);


}//parallel
}
