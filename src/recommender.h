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

/************************************************************************/
/* Recommender system using matrix factorization (MF)                   */
/* Computing the product recommendation using latent factor models      */
/************************************************************************/

#ifndef RECOMMANDER_H
#define RECOMMANDER_H


#include "model_parameters.h"
#include "training_set.h"
#include "learned_factors.h"
#include "learning_algorithm.h"
#include "rating_estimator.h"
#include "recommended_items.h"
#include "sparse_matrix.h"

/************************************************************************/
/*                         Learning algorithms                          */
/************************************************************************/

struct learning_model
{
	learning_algorithm_t	learning_algorithm;
	rating_estimator_t		rating_estimator;
	model_parameters_t		parameters;
	update_learning_t		update_algorithm;
	sparse_matrix_t*		social_matrix;
};

typedef struct learning_model learning_model_t;

/*
 * learn:            Learn using training set and the model parameters
 *                               
 *
 * Arguments:
 *      tset        The training set contains the known rating of items
 *		model		The model (learning algorithm + rating estimator)
 *
 * Returns:
 *      Return the learned factors.
 *
 */
struct learned_factors*
learn(struct training_set* tset, struct learning_model * model);

/*
* estimate_rating_from_factors:  Return the approximates user�s rating of an item based on 
*                                some learned factors.
*
* Arguments:
*		user_index: The index of a user
*		user_index: The index of an item
*		lfactors  : Learned factors
*		model		The model (learning algorithm + rating estimator)
*
* Returns:
*		The estimated rating.
*
*/
double
	estimate_rating_from_factors(rating_estimator_parameters_t* estim_param, 
		struct learning_model * model);


/*
* recommend_items:  Return the k item that have the highest estimated rating for 
*					a particular user
*
* Arguments:
*		user_index:		The index of a user
*		items_number:	The number of items to recommend
*		lfactors  :		Learned factors
*		tset				The training set contains the known rating of items
*		model		The model (learning algorithm + rating estimator)
*
* Returns:
*      A set of items that matches the most the user's preferences.
*
*/
recommended_items_t*
	recommend_items(rating_estimator_parameters_t* estim_param,
		learning_model_t * model);

void update_learning(training_set_t * old_tset, training_set_t* new_tset, learned_factors_t* lfactors,
		struct learning_model * model);

#endif /* RECOMMANDER_H */
