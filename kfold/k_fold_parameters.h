#ifndef K_FOLD_PARAMERTES_H
#define K_FOLD_PARAMERTES_H

#include "../recommender.h"



#pragma warning(disable: 4820)

struct k_fold_parameters
{
	int ratings_number;
	double K;
	char* file_path;
	model_parameters_t params;
	learning_model_t model;

};
typedef struct k_fold_parameters k_fold_parameters_t;


#endif