//#include "../recommender.h"
//
//#include <stdio.h>
//#include <assert.h>
//#include <stdlib.h>
//#include <memory.h>
//#include <math.h>
//#include <time.h>
//#include <string.h>
//#include "../matrix_factorization.h"
//#include "../matrix_factorization_bias.h"
//#include "../sparse_matrix.h"
//#include "../red_black_tree.h"
//#include "k_fold_parameters.h"
//#pragma warning(disable: 4996)
//
//
//
//
//
//
///*
//* RMSE : Calculate the RMSE
//*
//*Arguments : 
//*		learned : the estimated ratrings
//*		model : the learing model
//*		_validation_set : the exepected ratings values
//*Returns :
//*		return the RMSE
//**/
//double RMSE(learned_factors_t* learned,learning_model_t model,training_set_t * _validation_set,
//	k_fold_parameters_t _k_fold_params) 
//{
//	unsigned int i;
//	double sum = 0;
//	size_t s;
//	size_t u;
//
//	for (s = 0; s < _validation_set->training_set_size; s++) 
//	{
//		i = _validation_set->ratings->entries[s].row_i;
//		u = _validation_set->ratings->entries[s].column_j;
//
//		sum += pow(_validation_set->ratings->entries[s].value - 
//			estimate_rating_from_factors(u, i, learned, model), 2) / (_k_fold_params.ratings_number/_k_fold_params.K);
//	}
//
//	printf("RMSE = %f \n", sqrtf(sum));
//	return (sqrtf(sum));
//}
//
//
///*
//* read_file : read the file and outputs 2 training_sets
//* _tset : the traing set that's going to be read
//* _validation_set : the validation set 
//* _model : the learning model 
//* index : the index of the validation part
//* Returns
//*		0 if success
//*		-1 else
//*/
//int read_file(struct k_fold_parameters _k_fold_params, training_set_t** _t_set,training_set_t ** _validation_set,
//	int index){
//		FILE* file;
//		unsigned int i, j, l,ss;
//		double m;
//
//
//		//Learning
//		//Initialize a training set
//		*_t_set = init_training_set(_k_fold_params.params);
//		_k_fold_params.params.training_set_size = (size_t) (_k_fold_params.ratings_number/_k_fold_params.K);
//		*_validation_set=init_training_set(_k_fold_params.params);
//		file = fopen(_k_fold_params.file_path, "r"); 
//
//		assert(file);
//
//		if (!file)
//			return 1;
//
//		ss=0;
//
//		while (!feof(file)) {
//			if (fscanf(file, "%d %d %lf %d", &i, &j, &m, &l) != 4)
//				break;
//			++ss;
//			if((ss<=index*_k_fold_params.ratings_number/_k_fold_params.K)||(ss>(index+1)*_k_fold_params.ratings_number/_k_fold_params.K))
//				set_known_rating(i - 1, j - 1, (float) m, *_t_set);
//			else 
//				set_known_rating(i-1,j-1,(float)m,*_validation_set);
//		}
//
//		fclose(file);
//		return 0;
//}
//
//int main(int argc, char** argv) {
//	struct k_fold_parameters k_fold_params;
//	learned_factors_t* learned = NULL;
//	training_set_t* tset = NULL;
//	int index;
//	double RMSE_sum = 0;
//	recommended_items_t* r_items = NULL;
//	int dim,mindim=0;
//	double min=10;
//
//
//	//learning_model_t model = {0};
//	training_set_t * validation_set;
//
//	char* file_path = NULL;
//	size_t file_path_length = 0;
//
//	//Model configuration
//	//Setup model parameters
//	clock_t start = clock();
//
//	if(argc != 13)
//	{
//		printf("Incorrect arguments \n");
//		printf("usage: kfold 100000 2 943 1682 u.data 30 30 0.055 0.0095 0.02 0.001 basic \n kfold -h for help \n");
//		if(strcmp(argv[1],"-h")){
//			printf("kfold ratings_number K users_number items_number file_path \n");
//			printf("ratings_number : Number of ratings in the sample \n");
//			printf("K : Number of subsamples \n");
//			printf("users_number : number of users in the sample \n");
//			printf("items_number : number of items in the sample \n");
//			printf("file_path : the path of the samples file \n");
//			printf("dimentionality : dimentionality \n");
//			printf("iteration_number : the desired iteration's number \n");
//			printf("lambda : lambda \n");
//			printf("step : step \n");
//			printf("lambda_bias : lambda_bias \n");
//			printf("step_bias : step_bias \n");
//			printf("type : basic or bias \n");
//		}
//		system("pause");
//		exit(-1);
//	}
//
//	k_fold_params.ratings_number=atoi(argv[1]);
//	k_fold_params.K = atoi(argv[2]);
//	if(k_fold_params.K<2)
//	{
//		printf("K must be greater or equal than 2 \n ");
//		system("pause");
//		exit(-1);
//	}
//	k_fold_params.params.users_number = atoi(argv[3]);
//	if(k_fold_params.params.users_number <1)
//	{
//		printf("users_number must be a positif integer \n ");
//		system("pause");
//		exit(-1);
//	}
//	k_fold_params.params.items_number = atoi(argv[4]);
//	if(k_fold_params.params.items_number<1)
//	{
//		printf("items_number must be a positif integer \n ");
//		system("pause");
//		exit(-1);
//	}
//
//	file_path_length = strlen(argv[5]) + 1;
//
//	k_fold_params.file_path = (char*) malloc(file_path_length);
//	strcpy(k_fold_params.file_path, argv[5]);
//	k_fold_params.params.training_set_size = (size_t) (k_fold_params.ratings_number*((k_fold_params.K-1)/k_fold_params.K));
//
//	k_fold_params.params.dimensionality = atoi(argv[6]);
//	if(k_fold_params.params.dimensionality<1)
//	{
//		printf("dimensionality must be a positif integer \n ");
//		system("pause");
//		exit(-1);
//	}
//	k_fold_params.params.iteration_number = atoi(argv[7]);
//	if(k_fold_params.params.iteration_number<1)
//	{
//		printf("iteration_number must be a positif integer \n ");
//		system("pause");
//		exit(-1);
//	}
//
//
//
//	k_fold_params.params.lambda = (float)atof(argv[8]);
//
//
//
//	k_fold_params.params.step = (float)atof(argv[9]);
//
//	k_fold_params.params.lambda_bias = (float)atof(argv[10]);
//	k_fold_params.params.step_bias = (float)atof(argv[11]);
//
//
//
//	if(strcmpi(argv[12],"basic")==0)
//	{
//		printf("basic \n");
//		k_fold_params.model.learning_algorithm  =learn_basic_mf;
//		k_fold_params.model.rating_estimator = estimate_rating_basic_mf;
//	}
//	else if(strcmpi(argv[12],"bias")==0)
//	{
//		printf("bias \n");
//		k_fold_params.model.learning_algorithm = learn_mf_bias;
//		k_fold_params.model.rating_estimator   = estimate_rating_mf_bias;
//
//	}else
//		printf("type must be bias or basic");
//
//
//	
//
//	//for(dim=11;dim<=29;dim+=1){
//
//	//	k_fold_params.params.dimensionality=dim;
//	//k_fold_params.model.parameters= k_fold_params.params;
//	//RMSE_sum=0;
//	//for(index = 0;index < k_fold_params.K; index++){
//	//	read_file(k_fold_params, &tset, &validation_set, index);
//
//	//	compile_training_set(validation_set);
//	//	compile_training_set(tset);
//
//	//	learned = learn(tset, k_fold_params.model);
//	//	RMSE_sum += RMSE(learned,k_fold_params.model,validation_set,k_fold_params);
//	//}
//	//printf("mean(RMSE)=%f \n",RMSE_sum/k_fold_params.K);
//	//if(min>RMSE_sum/k_fold_params.K){
//	//	min=RMSE_sum/k_fold_params.K;
//	//	mindim=dim;
//	//}
//	
//	//}
//	
//		k_fold_params.model.parameters= k_fold_params.params;
//		RMSE_sum=0;
//		for(index = 0;index < k_fold_params.K; index++){
//		read_file(k_fold_params, &tset, &validation_set, index);
//
//		compile_training_set(validation_set);
//		compile_training_set(tset);
//
//		learned = learn(tset, k_fold_params.model);
//		RMSE_sum += RMSE(learned,k_fold_params.model,validation_set,k_fold_params);
//	}
//	
//	printf("mean(RMSE) %f \n ",RMSE_sum/k_fold_params.K);
//
//
//	//printf("%d %d",min,mindim);
//	free_recommended_items(r_items);
//	free_learned_factors(learned);
//	free_training_set(tset);
//
//	free(file_path);
//
//	system("pause");
//	//	_CrtDumpMemoryLeaks();
//
//	return 0;
//}