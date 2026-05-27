#include <omp.h>
#include <stdio.h>
   int tp=14;

int main()
{
   int iam, np=0, i;
#pragma omp threadprivate(tp)
//#pragma omp parallel num_threads(4) copyin(tp)  private(np,iam,i)  default(none)
#pragma omp parallel num_threads(2)   private(np,iam,i)  default(none)
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();
    omp_set_nested(1); 
    printf("\t\tDENTRO 1 valor de tp: %d - para  %d\n",tp,iam);
    #pragma omp master	
      tp=23;
    #pragma omp parallel num_threads(2) copyin(tp)
    printf("\t\tDENTRO 2 valor de tp: %d - para  %d\n",tp,iam);

}//parallel
printf("\t\tFUERA valor de tp: %d - para  %d\n",tp,np);
}
