#include"k_fold_rmse.h"
#include "data_set.h"
#include"../learned_factors.h"
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

#include <memory.h>
#include "../Serialization/Serialize_sparse_matrix.h"
#include "../Serialization/serialize_training_set.h"
#include "../Serialization/serialize_factors.h"
#include "../sparse_matrix.h"
#include "../rlog.h"


double RMSE_mean (k_fold_parameters_t k_fold_params)
{
	double RMSE_sum;
	int index;
	learned_factors_t *learned;
	training_set_t* tset = NULL;
	training_set_t* validation_set = NULL;
	training_set_t* vs = NULL;
	k_fold_params.model.parameters = k_fold_params.params;
	RMSE_sum = 0;
	for (index = 0; index < k_fold_params.K; index++)
	{
		extract_data (k_fold_params, &tset, &validation_set, index);
		compile_training_set (validation_set);
		compile_training_set (tset);
		
		//memcmp(t->entries,tset->ratings->entries,sizeof(size_t)*t->size);
		save_training_set(validation_set);
		
		vs=load_training_set();
		
		learned = learn (tset, k_fold_params.model);
		
		//l=load_learned_factors();
		RMSE_sum += RMSE (learned, validation_set, k_fold_params);
	}
	free (tset);
	free (validation_set);
	return (RMSE_sum / k_fold_params.K);
}



double RMSE (learned_factors_t* learnedf, training_set_t * _validation_set,
             k_fold_parameters_t _k_fold_params)
{
	unsigned int i;
	double sum = 0;
	size_t s;
	size_t u;

	for (s = 0; s < _validation_set->training_set_size; s++)
	{
		i = _validation_set->ratings->entries[s].row_i;
		u = _validation_set->ratings->entries[s].column_j;

		sum += pow (_validation_set->ratings->entries[s].value -
		            estimate_rating_from_factors (u, i, learnedf, _k_fold_params.model), 2) / (_k_fold_params.ratings_number / _k_fold_params.K);
	}

	RLog ("RMSE = %f \n", sqrt (sum) );
	return (sqrtf (sum) );
}