#include "find_minimum_tsearch.h"
#include "data_set.h"
#include "k_fold_rmse.h"
#include <stdio.h>

#define ABS(a) ((a)<0 ? -(a) : (a))
#include <stdarg.h>

void RLog(const char *format, ...)
{
	va_list arglist;

	va_start( arglist, format );
	vprintf( format, arglist );
	va_end( arglist );
}

int i_find_minimum_tsearch(int* param_to_find,
			int min,
			int max,
			int max_iter,
			k_fold_parameters_t *k_fold_params,
			double (*function)(k_fold_parameters_t*))
{
		double RMSE_meanD,RMSE_meanC;
		int leftThird,rightThird,left,right;
		int iter=0;
		////////////////////////////////////////
		left=min;
		right=max;
		do{
			iter++;
			RMSE_meanD=0;
			RMSE_meanC=0;
			leftThird = (int)((2*left + right) / 3.0);
			rightThird = (int)((left + 2*right) / 3.0);

			
			*param_to_find=(int)leftThird;
			k_fold_params->model.parameters= k_fold_params->params;
			RMSE_meanC=function(k_fold_params);
			printf("%d mean(RMSE)=%f \n",leftThird,RMSE_meanC);

			*param_to_find=(int)rightThird;
			k_fold_params->model.parameters= k_fold_params->params;
			RMSE_meanD=function(k_fold_params);
			printf("%d mean(RMSE)=%f \n",rightThird,RMSE_meanD);

			if(RMSE_meanC>=RMSE_meanD)
			{
				left=leftThird;
			}else right=rightThird;


		}while((ABS(leftThird - rightThird)>1)&&(iter<max_iter));
		if(RMSE_meanD<RMSE_meanC)
			return rightThird;
		else
			return leftThird;

		/////////////////////////////////////////////

}




// Ternary search algorithm 
double d_find_minimum_tsearch(float* param_to_find,
					   double min,
					   double max,
					   double precision, 
					   size_t max_iter,
					   k_fold_parameters_t *k_fold_params,
					   double (*function)(k_fold_parameters_t*))
{

	double RMSE_meanD, RMSE_meanC, left, right;

	double leftThird, rightThird;

	double result;

	size_t iter = 0;
	////////////////////////////////////////
	left = min;
	right = max;
	
	do
	{
		iter++;

		RMSE_meanD = 0;
		RMSE_meanC = 0;

		leftThird = (2*left + right) / 3.0;
		rightThird = (left + 2*right) / 3.0;

		*param_to_find = (float) leftThird;
		k_fold_params->model.parameters = k_fold_params->params;
		RMSE_meanC = (function)(k_fold_params);

		RLog("%f mean(RMSE)=%f \n", leftThird, RMSE_meanC);

		*param_to_find = (float) rightThird;
		k_fold_params -> model.parameters = k_fold_params->params;
		RMSE_meanD = (function)(k_fold_params);

		RLog("%f mean(RMSE)=%f \n", rightThird, RMSE_meanD);

		if (RMSE_meanC >= RMSE_meanD) 
		{
			left = leftThird;
		}
		else 
		{
			right = rightThird;
		}
	} 
	while (ABS(leftThird - rightThird) > precision && iter < max_iter);

	result = (left + right)/2.0;

	return result;
}


