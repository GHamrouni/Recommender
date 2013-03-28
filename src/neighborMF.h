#ifndef NEIGHBORMF_H
#define NEIGHBORMF_H


#include "model_parameters.h"
#include "training_set.h"
#include "learned_factors.h"

/************************************************************************/
/*                         Learning algorithms                          */
/************************************************************************/

struct learned_factors*
learn_mf_neighbor(struct training_set* tset, struct model_parameters params);

/*
 * Update the learned factors
 */
void
update_learned_factors_mf_neighbor(learned_factors_t* lfactors, training_set_t* tset, model_parameters_t params);


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
estimate_rating_(size_t user_index, size_t item_index, learned_factors_t* lfactors);

/*
 * Estimate the error between the rating r_ui and the predicted rating
 */
double
estimate_error_mf_neighbor
	(double r_iu, size_t user_index, size_t item_index, learned_factors_t* lfactors,training_set_t * tset);

double
calculate_rating (size_t user_index, size_t item_index, learned_factors_t* lfactors,training_set_t * tset);

#endif 