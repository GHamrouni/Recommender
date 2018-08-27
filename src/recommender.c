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

#include "recommender.h"
#include "rating_estimator.h"
#include "learning_algorithm.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>

/************************************************************************/
/*                          Learning algorithms                         */
/************************************************************************/

/*
 * Stochastic gradient descent
 */
struct learned_factors*
learn(struct training_set* tset, struct learning_model *model)
{
	learning_algorithm_params_t learning_param;
	assert (model->learning_algorithm && model->rating_estimator);
	assert(tset->ratings_matrix);
	learning_param.params = model->parameters;
	learning_param.tset=tset;
	learning_param.social_matrix=model->social_matrix;
	return model->learning_algorithm(&learning_param);
}

/*
 * estimate_rating_from_factors:  Return the approximates user�s rating of an item based on 
 *                                some learned factors.
 */
double
	estimate_rating_from_factors(rating_estimator_parameters_t* estim_param, 
		struct learning_model * model)
{
	return model->rating_estimator(estim_param);
}

/*
 * recommend_items:  Return the k item that have the highest estimated rating for 
 *					a particular user (Nearest neighbor search)
 */
recommended_items_t*
	recommend_items(rating_estimator_parameters_t* estim_param,
		learning_model_t * model)
{
	size_t j;
	recommended_items_t* r_items = init_recommended_items(estim_param->lfactors->items_number);

	assert (model->learning_algorithm && model->rating_estimator);

	for (j = 0; j < estim_param->tset->items_number; j++)
	{
		estim_param->item_index = j;
		insert_recommended_item(j, (float) model->rating_estimator(estim_param), r_items);
	}

	j = 0;
	while (r_items->bheap->filled_elements)
	{
		recommended_item_t *v = NULL;
		v = pop_binary_heap(r_items->bheap);

		if (v != NULL)
		{
			r_items->items[j] = *(v);
			free(v);
		}

		j++;
	}

	return r_items;
}



void
update_learning(training_set_t * old_tset, training_set_t* new_tset, learned_factors_t* lfactors,
		struct learning_model * model)
{
	assert (model->update_algorithm);

	model->update_algorithm(old_tset, new_tset, lfactors, &model->parameters);
}
