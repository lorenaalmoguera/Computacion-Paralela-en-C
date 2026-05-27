#include <omp.h>
#include <stdio.h>
int main()
{
   int iam, np=0;
   int fp=14;
#pragma omp parallel num_threads(4) firstprivate(fp) private(np,iam) default(none)
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();

    fp += ++iam;

    printf("\t\tDENTRO valos de fp: %d - para  %d\n",fp,iam);

}//parallel
printf("\t\tFUERA valor de fp: %d - para  %d\n",fp,np);
}
