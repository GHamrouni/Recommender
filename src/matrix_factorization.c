/*-
* Copyright (c) 2012 Hamrouni Ghassen.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

/*
 *
 * Recommender system using matrix factorization (MF)
 * Computing the product recommendation using latent factor models
 *
 */

#include "matrix_factorization.h"
#include "utils.h"
#include "learning_algorithm.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>

/*
 * rate_item:  Return the approximates user�s rating of an item
 *
 * Arguments:
 *      user_vector     The user's vector, measure the extent to which a user
 *                         is interested in a factor
 *      item_vector     The item's vector, measure the extent to which the item
 *                         possesses those factors
 *      dim              The size of the vectors
 *
 * Returns:
 *      The dot product between the two vectors.
 *
 */
double
estimate_item_rating(double* user_vector, double* item_vector, size_t dim)
{
	double sum = 0;
	size_t i;

	for (i = 0; i < dim; i++)
		sum += user_vector[i] * item_vector[i];

	return sum;
}

/*
 * regularized_squared_error:  Return the the regularized squared
 *                               error on the set of known ratings
 *
 * Arguments:
 *      user_vector     The user's factor vector to be computed
 *      item_vector     The item's factor vector to be computed
 *      r               The known rating from the training set.
 *      lambda            The constant lambda controls the extent of regularization
 *      size              The size of the vectors
 *
 * Returns:
 *      Return the regularized squared error.
 *
 */
double
regularized_squared_error(
			  double* user_vector,
			  double* item_vector,
			  double r,
			  double lambda,
			  size_t size)
{
	double diff = 
		(r - estimate_item_rating(user_vector, item_vector, size));

	return  diff * diff + 
		lambda * (length2(user_vector, size) + length2(item_vector, size));
}


/************************************************************************/
/*                          Learning algorithms                         */
/************************************************************************/

void 
compute_factors(
		double* item_factors, 
		double* user_factors, 
		double lambda, 
		double step, 
		double predicted_error, 
		size_t dimensionality)
{
	size_t ii = 0;

	for (ii = 0; ii < dimensionality; ii++)
	{
		item_factors[ii] = item_factors[ii] +
		       	step * (predicted_error * user_factors[ii] - lambda * item_factors[ii]);

		user_factors[ii] = user_factors[ii] +
		       	step * (predicted_error * item_factors[ii] - lambda * user_factors[ii]);
	}
}

/*
 * Stochastic gradient descent
 */
struct learned_factors*
learn_basic_mf(learning_algorithm_params_t* learning_params)
{
	struct learned_factors* lfactors = 
		init_learned_factors(&learning_params->params);

	size_t r, k, i, u;

	double r_iu = 0;
	double r_iu_estimated = 0;

	double e_iu = 0;
	double* item_factors;
	double* user_factors;

	r = k = u = i = 0;

	if (!lfactors)
		return NULL;

	for (k = 0; k < learning_params->params.iteration_number; k++)
	{
		for (r = 0; r < learning_params->params.training_set_size; r++)
		{
			 r_iu = learning_params->tset->ratings->entries[r].value;

			 i = learning_params->tset->ratings->entries[r].row_i;
			 u = learning_params->tset->ratings->entries[r].column_j;

			 item_factors = lfactors->item_factor_vectors[i];
			 user_factors = lfactors->user_factor_vectors[u];

			 r_iu_estimated = 
				 estimate_item_rating(item_factors, user_factors, learning_params->params.dimensionality);

			 e_iu = r_iu - r_iu_estimated;

			 compute_factors(item_factors, 
					 user_factors, 
					 learning_params->params.lambda, 
					 learning_params->params.step, 
					 e_iu, 
					 learning_params->params.dimensionality);
		 }
	 }

	lfactors->dimensionality = learning_params->params.dimensionality;
	lfactors->items_number = learning_params->params.items_number;
	lfactors->users_number = learning_params->params.users_number;

	return lfactors;
}

/*
 * estimate_rating_from_factors:  Return the approximates user�s rating of an item based on 
 *                                some learned factors.
 */
double
estimate_rating_basic_mf(rating_estimator_parameters_t* estim_param)
{
	learned_factors_t* lfactors = estim_param->lfactors;
	size_t item_index = estim_param->item_index;
	size_t user_index = estim_param->user_index;
	assert(item_index < lfactors->items_number);
	assert(user_index < lfactors->users_number);

	return estimate_item_rating(lfactors->user_factor_vectors[user_index], 
			lfactors->item_factor_vectors[item_index], 
			lfactors->dimensionality);
}

