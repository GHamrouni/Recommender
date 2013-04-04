#include "training_set.h"

double ** 
	calculate_simularities_matrix(training_set_t* tset);
double 
	estimate_rating(double** simularities_matrix,training_set_t* tset,size_t user,size_t item);