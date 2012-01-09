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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>


/*----------------------------------------------------------------------------------------------
 *
 *                                     Learning algorithms
 *
 *----------------------------------------------------------------------------------------------
 */

void 
compute_factors_bias(
                    int user_index, 
					int item_index, 
					learned_factors_t* lfactors,
                    double lambda, 
                    double step, 
                    double predicted_error, 
                    unsigned int dimensionality)
{
    unsigned int i = 0;

	double* item_factors;
	double* user_factors;

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	lfactors->user_bias[user_index] = lfactors->user_bias[user_index] + 
		                              step * (predicted_error - lambda * lfactors->user_bias[user_index]);

	lfactors->item_bias[item_index] = lfactors->item_bias[item_index] + 
		                              step * (predicted_error - lambda * lfactors->item_bias[item_index]);

    for (i = 0; i < dimensionality; i++)
    {
        item_factors[i] = item_factors[i] + step * (predicted_error * user_factors[i] - lambda * item_factors[i]);
        user_factors[i] = user_factors[i] + step * (predicted_error * item_factors[i] - lambda * user_factors[i]);
    }
}

/*
 * Stochastic gradient descent
 */
struct learned_factors*
learn_mf_bias(struct training_set* tset, struct model_parameters params)
{
    struct learned_factors* lfactors = init_learned_factors(params);

    unsigned int r, k, i, u;

    double r_iu = 0;
    double r_iu_estimated = 0;

    double e_iu = 0;
	double step = params.step;

	double average_rating = tset->ratings_sum / ((double) tset->training_set_size);
	
	lfactors->ratings_average = average_rating;
	lfactors->dimensionality = params.dimensionality;
	lfactors->items_number = params.items_number;
	lfactors->users_number = params.users_number;

    r = k = u = i = 0;

	for (u = 0; u < params.users_number; u++)
	{
		lfactors->user_bias[u] = user_ratings_average(u, tset, params) - average_rating;
	}

	for (i = 0; i < params.items_number; i++)
	{
		lfactors->item_bias[i] = item_ratings_average(i, tset, params)- average_rating;
	}

    for (k = 0; k < params.iteration_number; k++)
    {
        for (r = 0; r < params.training_set_size; r++)
        {
             r_iu = tset->ratings[r].value;

             i = tset->ratings[r].item_index;
             u = tset->ratings[r].user_index;

             r_iu_estimated = estimate_rating_mf_bias(u, i, lfactors);

             e_iu = r_iu - r_iu_estimated;

             compute_factors_bias(u, i, lfactors, params.lambda, step, e_iu, params.dimensionality);
         }

		//step = step * 0.9;
     }

    return lfactors;
}

/*
* estimate_rating_from_factors:  Return the approximates user’s rating of an item based on 
*                                some learned factors.
*/
double
estimate_rating_mf_bias(int user_index, int item_index, learned_factors_t* lfactors)
{
	double sum = 0;
	unsigned int i;

	double* item_factors;
	double* user_factors;

	double item_bias = lfactors->item_bias[item_index];
	double user_bias = lfactors->user_bias[user_index];

	double bias = lfactors->ratings_average + item_bias + user_bias;

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	for (i = 0; i < lfactors->dimensionality; i++)
		sum += user_factors[i] * item_factors[i];

	return sum + bias;
} 