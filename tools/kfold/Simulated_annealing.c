

#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
double E(double x){
	return x*x-3*x-6;
}

double simulated_annealing(double X0,double (*function)(double))
{

	    double x = X0;
		double e = 2.718281828;
		double lambda=0.7;
		double L = (*function)(x);
		double T ;
    for(T = 100; T > 0.00005; T *= lambda)
    {
		int i;
        for(i=0;i<200; i++)
        {
            double xNew = x + ((rand()/(double)RAND_MAX)*0.002 - 0.001);
            double LNew = (*function)(xNew);
            
            if(LNew < L || (rand()/(double)RAND_MAX) <= pow(e,-(LNew-L)/T))
            {
                L = LNew;
                x = xNew;
            }
        }
    }
	return x;

}


//int main(){

	//printf("%f",simulated_annealing(70.0,E));
//}