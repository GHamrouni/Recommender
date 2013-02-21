#include "../recommender.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "../matrix_factorization.h"
#include "../matrix_factorization_bias.h"
#include "../sparse_matrix.h"
#include "../red_black_tree.h"
#include "k_fold_parameters.h"
#include "Simulated_annealing.h"
#include "k_fold_rmse.h"
#include "find_minimum_tsearch.h"
#include "data_set.h"
#pragma warning(disable: 4996)


int parse_arguments (int argc, char** argv, k_fold_parameters_t *k_fold_params, void ** param_to_find, int* is_float)
{


	size_t file_path_length = 0;
	if (argc != 13)
	{
		printf ("Incorrect arguments \n");
		printf ("usage: kfold 100000 2 943 1682 u.data 30 30 0.055 0.0095 0.02 0.001 basic \n kfold -h for help \n");
		if (strcmp (argv[1], "-h") )
		{
			printf ("kfold ratings_number K users_number items_number file_path \n");
			printf ("ratings_number : Number of ratings in the sample \n");
			printf ("K : Number of subsamples \n");
			printf ("users_number : number of users in the sample \n");
			printf ("items_number : number of items in the sample \n");
			printf ("file_path : the path of the samples file \n");
			printf ("dimentionality : dimentionality \n");
			printf ("iteration_number : the desired iteration's number \n");
			printf ("lambda : lambda \n");
			printf ("step : step \n");
			printf ("lambda_bias : lambda_bias \n");
			printf ("step_bias : step_bias \n");
			printf ("type : basic or bias \n");
		}
		system ("pause");
		return (-1);
	}

	k_fold_params->ratings_number = atoi (argv[1]);
	k_fold_params->K = atoi (argv[2]);
	if (k_fold_params->K < 2)
	{
		printf ("K must be greater or equal than 2 \n ");
		system ("pause");
		return (-1);
	}
	k_fold_params->params.users_number = atoi (argv[3]);
	if (k_fold_params->params.users_number < 1)
	{
		printf ("users_number must be a positif integer \n ");
		system ("pause");
		return (-1);
	}
	k_fold_params->params.items_number = atoi (argv[4]);
	if (k_fold_params->params.items_number < 1)
	{
		printf ("items_number must be a positif integer \n ");
		system ("pause");
		return (-1);
	}

	file_path_length = strlen (argv[5]) + 1;

	k_fold_params->file_path = (char*) malloc (file_path_length);
	strcpy (k_fold_params->file_path, argv[5]);
	k_fold_params->params.training_set_size = (size_t) (k_fold_params->ratings_number * ( (k_fold_params->K - 1) / k_fold_params->K) );


	if (strcmpi (argv[6], "x") == 0)
	{
		*param_to_find = & (k_fold_params->params.dimensionality);
		*is_float = 0;
	}
	else
	{
		k_fold_params->params.dimensionality = atoi (argv[6]);
		if (k_fold_params->params.dimensionality < 1)
		{
			printf ("dimensionality must be a positif integer \n ");
			system ("pause");
			exit (-1);
		}
	}
	k_fold_params->params.iteration_number = atoi (argv[7]);
	if (k_fold_params->params.iteration_number < 1)
	{
		printf ("iteration_number must be a positif integer \n ");
		system ("pause");
		return (-1);
	}


	if (strcmpi (argv[8], "x") == 0)
	{
		*param_to_find = & (k_fold_params->params.lambda);
		*is_float = 1;
	}
	else
	{
		k_fold_params->params.lambda = (float) atof (argv[8]);
	}



	k_fold_params->params.step = (float) atof (argv[9]);

	if (strcmpi (argv[10], "x") == 0)
	{
		*param_to_find = & (k_fold_params->params.lambda_bias);
		*is_float = 1;
	}
	else
	{
		k_fold_params->params.lambda_bias = (float) atof (argv[10]);
	}

	k_fold_params->params.step_bias = (float) atof (argv[11]);



	if (strcmpi (argv[12], "basic") == 0)
	{
		printf ("basic \n");
		k_fold_params->model.learning_algorithm  = learn_basic_mf;
		k_fold_params->model.rating_estimator = estimate_rating_basic_mf;
	}
	else if (strcmpi (argv[12], "bias") == 0)
	{
		printf ("bias \n");
		k_fold_params->model.learning_algorithm = learn_mf_bias;
		k_fold_params->model.rating_estimator   = estimate_rating_mf_bias;

	}
	else
	{
		printf ("type must be bias or basic");
		return -1;
	}
	return 0;

}










int main (int argc, char** argv)
{

	learned_factors_t* learned = NULL;


	recommended_items_t* r_items = NULL;
	struct k_fold_parameters k_fold_params;

	double A, B;
	void * param_to_find = NULL;
	int is_float;// 1 if param_to_find is float

	char* file_path = NULL;


	//Model configuration
	//Setup model parameters
	clock_t start = clock();
	clock_t end;
	A = 0.1;
	B = 0.15;

	if (parse_arguments (argc, argv, &k_fold_params, &param_to_find, &is_float) != 0)
	{
		exit (-1);
	}


	k_fold_params.model.parameters = k_fold_params.params;


	if (param_to_find != NULL)
		if (is_float)
		{
			A = d_find_minimum_tsearch ( (float*) param_to_find, 0.01, 0.15, 0.00001, 10, &k_fold_params,&RMSE_mean);
		}
		else
		{
			A = i_find_minimum_tsearch ( (int*) param_to_find, 10, 30, 10, &k_fold_params,&RMSE_mean);
		}
	else
	{
		printf ("mean(RMSE)=%f \n", RMSE_mean (k_fold_params) );
	}
	free_recommended_items (r_items);
	free_learned_factors (learned);
	//free_training_set(tset);

	free (file_path);
	end = clock();
	printf ("Time : %f s \n", (double) (end - start) / CLOCKS_PER_SEC);
	system ("pause");
	//	_CrtDumpMemoryLeaks();

	return 0;
}