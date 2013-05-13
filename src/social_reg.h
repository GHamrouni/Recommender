#ifndef SOCIAL_REG_H
#define SOCIAL_REG_H
#include "training_set.h"
#include "learned_factors.h"
#include "rating_estimator.h"
#include "learning_algorithm.h"

struct learned_factors*
	learn_social(learning_algorithm_params_t learning_param);


double
estimate_rating_social(rating_estimator_parameters_t* estim_param);


#endif
