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

#include "basic_mf.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

/*
* rate_item:  Return the approximates user’s rating of an item
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
estimate_item_rating(double* user_vector, double* item_vector, unsigned int dim)
{
    double sum = 0;
    unsigned int i;

    for (i = 0; i < dim; i++)
        sum += user_vector[i] * item_vector[i];

    return sum;
}


/*
* length2:  Return the squared length of a vector
*
* Arguments:
*      vector            A vector (array) of double
*      size              The size of the vector
*
* Returns:
*      Return the squared length of a vector.
*
*/
double
length2(double* vector, unsigned int size)
{
    double sum = 0;
    unsigned int i;

    for (i = 0; i < size; i++)
        sum += vector[i] * vector[i];

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
                          unsigned int size)
{
    double diff = (r - estimate_item_rating(user_vector, item_vector, size));

    return diff * diff + lambda * (length2(user_vector, size) + length2(item_vector, size));
}


/*----------------------------------------------------------------------------------------------
 *
 *                                     Learning algorithms
 *
 *----------------------------------------------------------------------------------------------
 */

void 
compute_factors(
                    double* item_factors, 
                    double* user_factors, 
                    double lambda, 
                    double step, 
                    double predicted_error, 
                    unsigned int dimensionality)
{
    unsigned int i = 0;

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
learn_basic_mf(struct training_set* tset, struct model_parameters params)
{
    struct learned_factors* lfactors = init_learned_factors(params);

    unsigned int r, k, i, u;

    double r_iu = 0;
    double r_iu_estimated = 0;

    double e_iu = 0;
    double* item_factors;
    double* user_factors;

    r = k = u = i = 0;

    for (k = 0; k < params.iteration_number; k++)
    {
        for (r = 0; r < params.training_set_size; r++)
        {
             r_iu = tset->ratings[r].value;

             i = tset->ratings[r].item_index;
             u = tset->ratings[r].user_index;

             item_factors = lfactors->item_factor_vectors[i];
             user_factors = lfactors->user_factor_vectors[u];

             r_iu_estimated = estimate_item_rating(item_factors, user_factors, params.dimensionality);

             e_iu = r_iu - r_iu_estimated;

             compute_factors(item_factors, user_factors, params.lambda, params.step, e_iu, params.dimensionality);
         }
     }

    lfactors->dimensionality = params.dimensionality;
    lfactors->items_number = params.items_number;
    lfactors->users_number = params.users_number;

    return lfactors;
}

/*
* estimate_rating_from_factors:  Return the approximates user’s rating of an item based on 
*                                some learned factors.
*/
double
estimate_rating_basic_mf(int user_index, int item_index, learned_factors_t* lfactors)
{
    return estimate_item_rating(lfactors->user_factor_vectors[user_index], 
                                lfactors->item_factor_vectors[item_index], 
                                lfactors->dimensionality);
}