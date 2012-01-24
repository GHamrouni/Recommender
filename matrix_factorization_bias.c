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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>

/************************************************************************/
/*                          Learning algorithms                         */
/************************************************************************/

void 
compute_factors_bias(
					int user_index, 
					int item_index, 
					learned_factors_t* lfactors,
					double predicted_error,
					model_parameters_t* params)
{
	unsigned int i = 0;

	double* item_factors;
	double* user_factors;

	assert (is_valid(predicted_error));	

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	lfactors->user_bias[user_index] = lfactors->user_bias[user_index] + 
	params->step_bias * (predicted_error - params->lambda_bias * lfactors->user_bias[user_index]);

	lfactors->item_bias[item_index] = lfactors->item_bias[item_index] + 
	params->step_bias * (predicted_error - params->lambda_bias * lfactors->item_bias[item_index]);

	assert (is_valid(lfactors->user_bias[user_index]));
	assert (is_valid(lfactors->item_bias[item_index]));

	for (i = 0; i < params->dimensionality; i++)
	{
		item_factors[i] = item_factors[i] + params->step * (predicted_error * user_factors[i] - params->lambda * item_factors[i]);
		user_factors[i] = user_factors[i] + params->step * (predicted_error * item_factors[i] - params->lambda * user_factors[i]);

		assert (is_valid(item_factors[i]));
		assert (is_valid(user_factors[i]));
	}
}

void calculate_average_ratings(struct training_set* tset, learned_factors_t* lfactors, model_parameters_t params)
{
	unsigned int i;
	float average_rating = (float) tset->ratings_sum / ((float) tset->training_set_size);
	double* nb_ratings_per_user = NULL;

	if (!tset->ratings_matrix) return;

	nb_ratings_per_user = malloc(sizeof(double) * params.users_number);

	if (!nb_ratings_per_user)
		return;

	memset(nb_ratings_per_user, 0, sizeof(double) * params.users_number);

	lfactors->ratings_average = average_rating;

	for (i = 0; i < params.items_number; i++)
	{
		lfactors->item_bias[i] = item_ratings_average(i, tset)- average_rating;
	}

	for (i = 0; i < tset->ratings_matrix->nonzero_entries_nb; i++)
	{
		nb_ratings_per_user[tset->ratings_matrix->column_index[i]] += 1; 
	}

	for (i = 0; i < tset->ratings_matrix->nonzero_entries_nb; i++)
	{
		lfactors->user_bias[tset->ratings_matrix->column_index[i]] += 
			tset->ratings_matrix->values[i];
	}

	for (i = 0; i < tset->users_number; i++)
	{
		if (nb_ratings_per_user[i] > 0)
			lfactors->user_bias[i] = 
			lfactors->user_bias[i]/(nb_ratings_per_user[i]) - average_rating;
	}

	free(nb_ratings_per_user);
}

/*
 * Update the learned factors
 */
void
update_learned_factors_mf_bias(struct learned_factors* lfactors, struct training_set* tset, struct model_parameters params)
{
	unsigned int r, k, i, u;

	double r_iu = 0;

	double e_iu = 0;
	double step = params.step;

	lfactors->dimensionality = params.dimensionality;
	lfactors->items_number = params.items_number;
	lfactors->users_number = params.users_number;

	r = k = u = i = 0;

	for (k = 0; k < params.iteration_number; k++)
	{
		double max_error = 0;

		for (r = 0; r < params.training_set_size; r++)
		{
			r_iu = tset->ratings->entries[r].value;

			i = tset->ratings->entries[r].row_i;
			u = tset->ratings->entries[r].column_j;

			e_iu = estimate_error_mf_bias(r_iu, u, i, lfactors);

			assert (is_valid(e_iu));

			max_error = fmax(max_error, fabs(e_iu));

			compute_factors_bias(u, i, lfactors, e_iu, &params);
		}

		if (max_error < step)
			break;
	}
}

/*
 * Stochastic gradient descent
 */
struct learned_factors*
learn_mf_bias(struct training_set* tset, struct model_parameters params)
{
	struct learned_factors* lfactors = init_learned_factors(params);

	lfactors->dimensionality = params.dimensionality;
	lfactors->items_number = params.items_number;
	lfactors->users_number = params.users_number;

	calculate_average_ratings(tset, lfactors, params);

	free_sparse_matrix(tset->ratings_matrix);
	tset->ratings_matrix = NULL;

	update_learned_factors_mf_bias(lfactors, tset, params);

	return lfactors;
}

double
estimate_error_mf_bias(double r_iu, unsigned int user_index, unsigned int item_index, learned_factors_t* lfactors)
{
	double sum = 0.0;
	unsigned int i;

	double* item_factors;
	double* user_factors;

	double item_bias;
	double user_bias;

	double bias;

	assert(item_index < lfactors->items_number);
	assert(user_index < lfactors->users_number);

	item_bias = lfactors->item_bias[item_index];
	user_bias = lfactors->user_bias[user_index];

	bias = lfactors->ratings_average + item_bias + user_bias;

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	for (i = 0; i < lfactors->dimensionality; i++)
		sum += user_factors[i] * item_factors[i];

	assert(is_valid(sum));

	return r_iu - (sum + bias);
}

/*
 * estimate_rating_from_factors:  Return the approximates user’s rating of an item based on 
 *                                some learned factors.
 */
double
estimate_rating_mf_bias(unsigned int user_index, unsigned int item_index, learned_factors_t* lfactors)
{
	double sum = 0;
	unsigned int i;

	double* item_factors;
	double* user_factors;

	double item_bias;
	double user_bias;

	double bias;

	assert(item_index < lfactors->items_number);
	assert(user_index < lfactors->users_number);

	item_bias = lfactors->item_bias[item_index];
	user_bias = lfactors->user_bias[user_index];

	bias = lfactors->ratings_average + item_bias + user_bias;

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	for (i = 0; i < lfactors->dimensionality; i++)
		sum += user_factors[i] * item_factors[i];

	return sum + bias;
}

