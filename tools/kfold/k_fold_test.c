#include "recommender.h"
#include "matrix_factorization.h"
#include "matrix_factorization_bias.h"
#include "sparse_matrix.h"
#include "red_black_tree.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "k_fold_parameters.h"
#include "Simulated_annealing.h"
#include "k_fold_rmse.h"
#include "find_minimum_tsearch.h"
#include "data_set.h"
#include "neighborMF.h"
#include "social_reg.h"

#if WIN32
#pragma warning(disable: 4996)
#endif

int parse_arguments (int argc, char** argv, k_fold_parameters_t *k_fold_params, void ** param_to_find, int* is_float)
{


	size_t file_path_length = 0;
	int bin_width_ratio;
	if (argc < 15)
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
			printf ("projection family number :  \n");
			printf ("bin width ratio \n");
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
	k_fold_params->params.training_set_size = k_fold_params->ratings_number;


	if (strcmp (argv[6], "x") == 0)
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
	if (strcmp (argv[7], "x") == 0)
	{
		*param_to_find = & (k_fold_params->params.iteration_number);
		*is_float = 0;
	}else
	{
	k_fold_params->params.iteration_number = atoi (argv[7]);
	if (k_fold_params->params.iteration_number < 1)
	{
		printf ("iteration_number must be a positif integer \n ");
		system ("pause");
		return (-1);
	}
	}

	if (strcmp (argv[8], "x") == 0)
	{
		*param_to_find = & (k_fold_params->params.lambda);
		*is_float = 1;
	}
	else
	{
		k_fold_params->params.lambda = (float) atof (argv[8]);
	}



	k_fold_params->params.step = (float) atof (argv[9]);

	if (strcmp (argv[10], "x") == 0)
	{
		*param_to_find = & (k_fold_params->params.lambda_bias);
		*is_float = 1;
	}
	else
	{
		k_fold_params->params.lambda_bias = (float) atof (argv[10]);
	}

	k_fold_params->params.step_bias = (float) atof (argv[11]);

	k_fold_params->params.proj_family_size = (int) atoi (argv[12]);

	bin_width_ratio = (int) atoi (argv[13]);

	if (strcmp (argv[14], "basic") == 0)
	{
		printf ("basic \n");
		k_fold_params->model.learning_algorithm  = learn_basic_mf;
		k_fold_params->model.rating_estimator = estimate_rating_basic_mf;
		k_fold_params->model.parameters.algoithm_type = 0;
	}
	else if (strcmp (argv[14], "bias") == 0)
	{
		printf ("bias \n");
		k_fold_params->model.learning_algorithm = learn_mf_bias;
		k_fold_params->model.rating_estimator   = estimate_rating_mf_bias;
		k_fold_params->model.update_algorithm = update_learning_with_training_set;
		k_fold_params->model.parameters.algoithm_type = BIAS;
	}
	else if (strcmp (argv[14], "MFneighbors") == 0)
	{
		printf ("MFneighbors \n");
		k_fold_params->model.learning_algorithm = learn_mf_neighbor;
		k_fold_params->model.rating_estimator   = estimate_rating_mf_neighbor;
		k_fold_params->model.update_algorithm = update_learning_with_training_set_neighborMF;
		k_fold_params->model.parameters.algoithm_type = NEIGHBOURS_MF;

	}else if (strcmp (argv[14], "social") == 0)
	{
		printf ("social \n");
		k_fold_params->model.learning_algorithm = learn_social;
		k_fold_params->model.rating_estimator   = estimate_rating_social;
		k_fold_params->params.algoithm_type = SOCIAL;
		if(argc!=18)
			return -1;
		k_fold_params->social_relations_file_path= malloc(strlen(argv[15]+1) + 1);
		strcpy(k_fold_params->social_relations_file_path,argv[15]);
		k_fold_params->social_relations_number = atoi(argv[16]);
		k_fold_params->params.betha=atoi(argv[17]);
	}
	else
	{
		printf ("type must be bias or basic");
		return -1;
	}
	k_fold_params->params.bin_width = (int)(k_fold_params->params.items_number * bin_width_ratio / 100.0);
	
	k_fold_params->params.seed = 4578;
	return 0;

}










int main (int argc, char** argv)
{

	learned_factors_t* learned = NULL;


	recommended_items_t* r_items = NULL;
	struct k_fold_parameters k_fold_params;

	void * param_to_find = NULL;
	int is_float;

	


	
	clock_t start = clock();
	clock_t end;

	if (parse_arguments (argc, argv, &k_fold_params, &param_to_find, &is_float) != 0)
	{
		exit (-1);
	}


	k_fold_params.model.parameters = k_fold_params.params;


	if (param_to_find != NULL)
		if (is_float)
		{
			d_find_minimum_tsearch ( (float*) param_to_find, 0.1, 1, 0.1, 40, &k_fold_params,&RMSE_mean);
		}
		else
		{
			i_find_minimum_tsearch ( (int*) param_to_find, 10, 40, 20, &k_fold_params,&RMSE_mean);
		}
	else
	{
		printf ("mean(RMSE)=%f \n", RMSE_mean (&k_fold_params) );
	}
	free_recommended_items (r_items);
	free_learned_factors (learned);
	
	k_fold_params.social_relations_file_path = NULL;
	free (k_fold_params.file_path);
	free (k_fold_params.social_relations_file_path);
	end = clock();
	printf ("Time : %f s \n", (double) (end - start) / CLOCKS_PER_SEC);
	system ("pause");
	

	return 0;
}
