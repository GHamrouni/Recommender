#ifndef FIND_MINIMUM_TSEARCH_H
#define FIND_MINIMUM_TSEARCH_H
#include "training_set.h"
#include "k_fold_parameters.h"





// Ternary search algorithm 
// Find the minimum of a unimodal function.
// Complexity = O(log(n))
// Returns double
double d_find_minimum_tsearch(float* param_to_find,
	double min,
	double max,
	double precision, 
	size_t max_iter,
	k_fold_parameters_t *k_fold_params,
	double (*function)(k_fold_parameters_t*));


// Ternary search algorithm
// Returns int
int i_find_minimum_tsearch(int* param_to_find,
			int min,
			int max,
			int max_iter,
			k_fold_parameters_t *k_fold_params,
			double (*function)(k_fold_parameters_t*));

#endif