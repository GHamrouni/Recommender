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

#ifndef MATRIX_FACTORIZATION_WITH_BIAS_H
#define MATRIX_FACTORIZATION_WITH_BIAS_H


#include "model_parameters.h"
#include "training_set.h"
#include "learned_factors.h"
#include "rating_estimator.h"
#include "learning_algorithm.h"
/************************************************************************/
/*                         Learning algorithms                          */
/************************************************************************/

/*
 * learn:            Learn using training set and the model parameters
 *                               
 *
 * Arguments:
 *      tset        The training set contains the known rating of items
 *      params        Parameters of the model
 *
 * Returns:
 *      Return the learned factors.
 *
 */

struct learned_factors*
learn_mf_bias(learning_algorithm_params_t* learning_param);

/*
 * Update the learned factors
 */
void
update_learned_factors_mf_bias(learned_factors_t* lfactors, training_set_t* tset, model_parameters_t* params);


/*
 * estimate_rating_from_factors:  Return the approximates user�s rating of an item based on 
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
estimate_rating_mf_bias(rating_estimator_parameters_t* estim_param);

/*
 * Estimate the error between the rating r_ui and the predicted rating
 */
double
estimate_error_mf_bias(double r_iu, size_t user_index, size_t item_index, learned_factors_t* lfactors);

void update_learning_with_training_set(training_set_t * old_tset,training_set_t* new_tset,learned_factors_t* lfactors,
		model_parameters_t* params);

#endif /* MATRIX_FACTORIZATION_WITH_BIAS_H */

