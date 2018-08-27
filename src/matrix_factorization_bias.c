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

#include "matrix_factorization_bias.h"
#include "utils.h"
#include "training_set.h"
#include "float_tester.h"
#include "rlog.h"
#include <stdio.h>
#include "learning_algorithm.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>

/************************************************************************/
/*                          Learning algorithms                         */
/************************************************************************/
#define ABS(a) ((a)<0 ? -(a) : (a))

static void
compute_factors_bias (  size_t user_index,
                        size_t item_index,
                        learned_factors_t* lfactors,
                        double predicted_error,
                        const model_parameters_t* params)
{
	size_t i = 0;

	double* item_factors;
	double* user_factors;

	assert (is_valid (predicted_error) );

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	lfactors->user_bias[user_index] = lfactors->user_bias[user_index] +
	                                  params->step_bias * (predicted_error - params->lambda_bias *  lfactors->user_bias[user_index]);

	lfactors->item_bias[item_index] = lfactors->item_bias[item_index] +
	                                  params->step_bias * (predicted_error - params->step_bias *  lfactors->item_bias[item_index]);

	assert (is_valid (lfactors->user_bias[user_index]) );
	assert (is_valid (lfactors->item_bias[item_index]) );

	for (i = 0; i < params->dimensionality; i++)
	{
		double delta_i =  predicted_error * user_factors[i] - params->lambda * item_factors[i];
		double delta_u =  predicted_error * item_factors[i] - params->lambda * user_factors[i];
		item_factors[i] = item_factors[i] +
		                  params->step * delta_i;

		user_factors[i] = user_factors[i] +
		                  params->step * delta_u;

		assert (is_valid (item_factors[i]) );
		assert (is_valid (user_factors[i]) );
	}
}

static void
calculate_average_ratings (struct training_set* tset, learned_factors_t* lfactors)
{
	double average_rating = (double) tset->ratings_sum / ( (double) tset->training_set_size);
	average_rating = (average_rating - 1) / 4.0;
	lfactors->ratings_average = log ( (double) average_rating / (1 - average_rating) );
}

/*
 * Update the learned factors
 */
void
update_learned_factors_mf_bias (struct learned_factors* lfactors, struct training_set* tset, struct model_parameters * params)
{
	size_t r, k, i, u;

	double r_iu = 0;

	double e_iu = 0;
	double prediction;
	double sig_score;
	double loss		= 999999;
	double curr_loss = 0;
	double score;
	lfactors->dimensionality = params->dimensionality;
	lfactors->items_number = params->items_number;
	lfactors->users_number = params->users_number;

	r = k = u = i = 0;

	for (k = 0; k < params->iteration_number; k++)
	{
		if (curr_loss < loss)
		{
			params->step *= 1.0;
			params->step_bias *= 1.0;
			loss = curr_loss;
		}
		curr_loss = 0;
		for (r = 0; r < tset->training_set_size; r++)
		{
			r_iu = tset->ratings->entries[r].value;

			i = tset->ratings->entries[r].row_i;
			u = tset->ratings->entries[r].column_j;

			score = lfactors->ratings_average + lfactors->user_bias[u] + lfactors->item_bias[i] +
			        dot_product (lfactors->user_factor_vectors[u], lfactors->item_factor_vectors[i], lfactors->dimensionality);
			sig_score = 1 / (1 + exp (-score) );
			prediction = 1 + sig_score * 4;

			e_iu = r_iu - prediction;
			curr_loss += pow (e_iu, 2);
			if (e_iu)
			{
				compute_factors_bias (u, i, lfactors, e_iu * sig_score * (1 - sig_score) * 4 , params);
			}
		}
	}
}

/*
 * Stochastic gradient descent
 */
struct learned_factors*
learn_mf_bias (learning_algorithm_params_t* learning_param)
{
	struct training_set* tset = learning_param->tset;
	struct model_parameters params = learning_param->params;
	struct learned_factors* lfactors = init_learned_factors (&params);


	if (!lfactors)
	{
		return NULL;
	}

	lfactors->dimensionality = params.dimensionality;
	lfactors->items_number = params.items_number;
	lfactors->users_number = params.users_number;

	if (tset->ratings_matrix)
	{
		calculate_average_ratings (tset, lfactors);

		free_sparse_matrix (tset->ratings_matrix);
		tset->ratings_matrix = NULL;
	}

	update_learned_factors_mf_bias (lfactors, tset, &params);

	return lfactors;
}


/*
 * estimate_rating_from_factors:  Return the approximates user's rating of an item based on
 *                                some learned factors.
 */
double
estimate_rating_mf_bias (rating_estimator_parameters_t* estim_param)
{
	learned_factors_t* lfactors = estim_param->lfactors;
	size_t item_index = estim_param->item_index;
	size_t user_index = estim_param->user_index;

	double sum = 0;
	size_t i;

	double* item_factors;
	double* user_factors;

	double item_bias;
	double user_bias;

	double bias;

	assert (item_index < lfactors->items_number);
	assert (user_index < lfactors->users_number);

	item_bias = lfactors->item_bias[item_index];
	user_bias = lfactors->user_bias[user_index];

	bias = lfactors->ratings_average + item_bias + user_bias;

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	for (i = 0; i < lfactors->dimensionality; i++)
	{
		sum += user_factors[i] * item_factors[i] ;
	}
	return 1 + (1 / (1 + exp (-sum - bias) ) ) * 4;
}

void update_learning_with_training_set (training_set_t * old_tset, training_set_t* new_tset, learned_factors_t* lfactors,
                                        model_parameters_t* params)
{
	size_t r, k, i, u;

	double r_iu = 0;
	double e_iu = 0;
	double score,sig_score, prediction,curr_loss;

	curr_loss = 0.0;

	add_training_set (old_tset, new_tset);
	calculate_average_ratings (old_tset, lfactors);
	r = k = u = i = 0;

	for (k = 0; k < params->iteration_number; k++)
	{
		for (r = 0; r < new_tset->training_set_size; r++)
		{
			r_iu = new_tset->ratings->entries[r].value;

			i = new_tset->ratings->entries[r].row_i;
			u = new_tset->ratings->entries[r].column_j;

			score = lfactors->ratings_average + lfactors->user_bias[u] + lfactors->item_bias[i] +
			        dot_product (lfactors->user_factor_vectors[u], lfactors->item_factor_vectors[i], lfactors->dimensionality);
			sig_score = 1 / (1 + exp (-score) );
			prediction = 1 + sig_score * 4;

			e_iu = r_iu - prediction;
			curr_loss += pow (e_iu, 2);
			if (e_iu)
			{
				compute_factors_bias (u, i, lfactors, e_iu * sig_score * (1 - sig_score) * 4 , params);
			}
		}
	}
}

