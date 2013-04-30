#ifndef SOCIAL_REG_H
#define SOCIAL_REG_H
#include "training_set.h"
#include "learned_factors.h"
#include "rating_estimator.h"


struct learned_factors*
	learn_social(struct training_set* tset, struct model_parameters params,sparse_matrix_t* social_matrix);


double
estimate_rating_social(rating_estimator_parameters_t* estim_param);


#endif