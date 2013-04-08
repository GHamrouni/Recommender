#ifndef NEIGHBORMF_H
#define NEIGHBORMF_H


#include "model_parameters.h"
#include "training_set.h"
#include "learned_factors.h"
#include "rating_estimator.h"
/************************************************************************/
/*                         Learning algorithms                          */
/************************************************************************/

/*
 * learn:            Learn using training set and the model parameters
 *                               
 *
 * Arguments:
 *      tset				The training set contains the known rating of items
 *      params				Parameters of the model
 *
 * Returns:
 *      Return the learned factors.
 *
 */
struct learned_factors*
learn_mf_neighbor(struct training_set* tset, struct model_parameters params);

/*
 * Update the learned factors
 * Arguments :
 *		lfactors			The leaned factors to be learned
 *		tset :				The training set contains the known rating of items
 *		params				Parameters of the model
 */
void
update_learned_factors_mf_neighbor(training_set_t* tset, model_parameters_t params);





/*
 * Estimate the error between the rating r_ui and the predicted rating
 *		r_iu				The real rating
 *		estim_param			The rating estimator parameters
 */
double
estimate_error_mf_neighbor (double r_iu, rating_estimator_parameters_t* estim_param);


/*
 * estimate_rating_from_factors:  Return the approximates user’s rating of an item based on 
 *                                some learned factors.
 *
 * Arguments:
 *      user_index: The index of a user
 *      user_index: The index of an item
 *      lfactors  : Learned factors
 *
 * Returns:
 *      The estimated rating.
 *
 */
double
	estimate_rating_mf_neighbor (rating_estimator_parameters_t* estim_param);
void update_learning_with_training_set_neighborMF(training_set_t * old_tset,training_set_t* new_tset,learned_factors_t* lfactors,
		const model_parameters_t* params);

#endif 

