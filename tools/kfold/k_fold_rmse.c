#include"k_fold_rmse.h"
#include "data_set.h"
#include "learned_factors.h"
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include <stdarg.h>
#include  "rlog.h"
#include "../serialization/serialize_training_set.h"
#include "../serialization/redis_parameters.h"
#include "nearest_neighbors.h"
#include "neighborMF.h"
#include"sparse_matrix_hash.h"
#include "items_rated_by_user.h"
#include "rating_estimator.h"
#define ABS(a) ((a)<0 ? -(a) : (a))
double RMSE_mean (k_fold_parameters_t k_fold_params)
{
	double RMSE_sum;
	int index;
	learned_factors_t *learned;
	training_set_t* tset = NULL;
	training_set_t* validation_set = NULL;
	training_set_t* second_tset = NULL;

	k_fold_params.model.parameters = k_fold_params.params;
	RMSE_sum = 0;
	for (index = 0; index < k_fold_params.K; index++)
	{
		extract_data_2_tset (k_fold_params, &tset, &validation_set,&second_tset, index);
		compile_training_set (tset);
		learned = learn (tset, k_fold_params.model);
		update_learning (tset,second_tset,learned,k_fold_params.model);
		RMSE_sum += RMSE (learned,validation_set,k_fold_params,tset);
		
		free_learned_factors(learned);
		free_training_set (tset);
		free_training_set (validation_set);
	}
	
	return (RMSE_sum / k_fold_params.K);
}



double RMSE (learned_factors_t* learned, training_set_t * _validation_set,
             k_fold_parameters_t _k_fold_params,training_set_t * tset)
{
	unsigned int i;
	double sum = 0;
	double a;
	size_t s;
	size_t u;
	int n=0;
	rating_estimator_parameters_t* estim_param=malloc(sizeof(rating_estimator_parameters_t));
	estim_param->lfactors=learned;
	estim_param->tset=tset;
	for (s = 0; s < _validation_set->training_set_size; s++)
	{
		i = _validation_set->ratings->entries[s].row_i;
		u = _validation_set->ratings->entries[s].column_j;
		estim_param->item_index = i;
		estim_param->user_index = u;
		a = estimate_rating_from_factors (estim_param,_k_fold_params.model);
		sum += pow (_validation_set->ratings->entries[s].value -
			a , 2) / ((double)_validation_set->training_set_size);
	}
	RLog ("RMSE = %f \n", sqrtf (sum) );
	free(estim_param);
	return (sqrtf (sum) );
}
