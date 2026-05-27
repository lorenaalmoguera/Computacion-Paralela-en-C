#include <omp.h>
#include <stdio.h>

int main()
{
   int iam, np=0, i;
   int lp=14;
#pragma omp parallel num_threads(4)  private(np,iam,i) shared(lp) default(none)
{
    iam = omp_get_thread_num();  
    np = omp_get_num_threads();

    #pragma omp for schedule(static,250) lastprivate(lp)	
    for (lp=0;lp<1000;lp++){
        i++;
        if((iam==2) && (lp==748)){
           printf("A punto de acabar\n");
           sleep(5); 
        } 
    }	

    printf("\t\tDENTRO valos de lp: %d - para  %d\n",lp,iam);

}//parallel
printf("\t\tFUERA valor de lp: %d - para  %d\n",lp,np);
}
