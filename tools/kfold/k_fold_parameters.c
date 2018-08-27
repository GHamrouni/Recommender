
#include"k_fold_parameters.h"
#include "matrix_factorization_bias.h"
#pragma warning(disable: 4996)


k_fold_parameters_t initialize_k_fold_parameters(k_fold_parameters_t _k_fold_params)
{
		k_fold_parameters_t	k_fold_params;
		//k_fold_params->params->users_number=_k_fold_params->users_number;
		//k_fold_params->params->items_number=_k_fold_params->items_number;
		k_fold_params->K=_k_fold_params->K;
		k_fold_params->ratings_number=_k_fold_params->ratings_number;
		k_fold_params->file_path=(char*)malloc(strlen(_k_fold_params->file_path)+1);
		strcpy(k_fold_params->file_path,_k_fold_params->file_path);

		k_fold_params->params->items_number = _k_fold_params->params->items_number;
		k_fold_params->params->users_number = _k_fold_params->params->users_number;

		k_fold_params->params->training_set_size = (size_t) (_k_fold_params->ratings_number*((_k_fold_params->K-1)/_k_fold_params->K));

		k_fold_params->params->dimensionality = 30;
		k_fold_params->params->iteration_number = 30;

		k_fold_params->params->lambda = 0.055f;
		k_fold_params->params->step = 0.0095f;

		k_fold_params->params->lambda_bias = 0.02f;
		k_fold_params->params->step_bias = 0.001f;

		//Use the bias matrix factorization model
		
		k_fold_params->model->learning_algorithm = learn_mf_bias;
		k_fold_params->model->rating_estimator   = estimate_rating_mf_bias;

		//Use the basic matrix factorization model

		/*printf("basic \n");
		_model->learning_algorithm =learn_basic_mf;
		_model->rating_estimator = estimate_rating_basic_mf;*/


		k_fold_params->model->parameters= k_fold_params->params;
		return k_fold_params;
}