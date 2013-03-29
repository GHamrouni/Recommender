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
double RMSE_mean (k_fold_parameters_t k_fold_params)
{
	double RMSE_sum;
	int index,item_i,i,item_j;
	learned_factors_t *learned;
	training_set_t* tset = NULL;
	training_set_t* validation_set = NULL;
	double** simularities_matrix=NULL;
	double sum_numerator,sum_denum;
	float rating;


	k_fold_params.model.parameters = k_fold_params.params;
	RMSE_sum = 0;
	for (index = 0; index < k_fold_params.K; index--)
	{
		extract_data (k_fold_params, &tset, &validation_set, index);
		compile_training_set (tset);
//		compile_training_set (validation_set);
		learned = learn (tset, k_fold_params.model);
		//compile_training_set (tset);
		//simularities_matrix=calculate_simularities_matrix(tset);

		
		
		//RMSE_sum += RMSE (learned, validation_set, k_fold_params);
		//
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
	unsigned int i,item_j;
	double sum = 0;
	size_t s;
	size_t u;
	double sum_numerator=0;

	for (s = 0; s < _validation_set->training_set_size; s++)
	{
		i = _validation_set->ratings->entries[s].row_i;
		u = _validation_set->ratings->entries[s].column_j;
		sum += pow (_validation_set->ratings->entries[s].value -
		            calculate_rating (u, i, learned, tset) , 2) / (_k_fold_params.ratings_number / _k_fold_params.K);
	}
	RLog ("RMSE = %f \n", sqrtf (sum) );
	return (sqrtf (sum) );
}

double new_RMSE_para(double** simularities_matrix,learned_factors_t* factors, training_set_t * _validation_set,
	training_set_t * tset,k_fold_parameters_t _k_fold_params)
{
	unsigned int i;
	double sum = 0;
	size_t s;
	size_t u;
	double estimated_rating;
	for (s = 0; s < _validation_set->training_set_size; s++)
	{
		i = _validation_set->ratings->entries[s].row_i;
		u = _validation_set->ratings->entries[s].column_j;
		estimated_rating = estimate_rating_from_factors (u, i, factors, _k_fold_params.model);
		estimated_rating += estimate_rating(simularities_matrix,tset,u,i);
		estimated_rating /= 2.0;
		sum += pow (_validation_set->ratings->entries[s].value -
		           estimated_rating, 2) / (_k_fold_params.ratings_number / _k_fold_params.K);
	}

	RLog ("RMSE = %f \n", sqrtf (sum) );
	return (sqrtf (sum) );
}


double new_RMSE_seq(double** simularities_matrix, training_set_t * _validation_set,
	training_set_t * tset,k_fold_parameters_t _k_fold_params)
{
	unsigned int i;
	double sum = 0;
	size_t s;
	size_t u;
	double estimated_rating;
	for (s = 0; s < _validation_set->training_set_size; s++)
	{
		i = _validation_set->ratings->entries[s].row_i;
		u = _validation_set->ratings->entries[s].column_j;
		estimated_rating = estimate_rating(simularities_matrix,tset,u,i);
		sum += pow (_validation_set->ratings->entries[s].value -
		           estimated_rating, 2) / (_k_fold_params.ratings_number / _k_fold_params.K);
	}

	RLog ("RMSE = %f \n", sqrtf (sum) );
	return (sqrtf (sum) );
}

void fill_rating(training_set_t * tset, k_fold_parameters_t _k_fold_params,learned_factors_t * learned)
{
	int s,i,u;
	for (s = 0; s < tset->training_set_size; s++)
	{
		i = tset->ratings->entries[s].row_i;
		u = tset->ratings->entries[s].column_j;
		if(get_element(i,u,tset->ratings_matrix)==0)
		{
		insert_value(tset->ratings_matrix, i,u,estimate_rating_from_factors (u, i, learned, _k_fold_params.model));
		}
	}
}