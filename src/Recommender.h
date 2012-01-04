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

/*-------------------------------------------------------------------------------------
 *
 *                                Learning algorithms
 *
 *-------------------------------------------------------------------------------------
 */

struct training_set
{
    double**         ratings;            //Known ratings
    unsigned int     training_set_size;  //The number of ratings
	unsigned int     dimensionality; //dimensionality of the joint latent factor space

	unsigned int     users_number;
	unsigned int     items_number;
};

struct model_parameters
{
    unsigned int     users_number;
    unsigned int     items_number;

    double            lambda;          //The constant lambda controls the extent of regularization
    double            step;            //step size in stochastic gradient descent algorithm

    unsigned int    dimensionality; //dimensionality of the joint latent factor space
    unsigned int    iteration_number;
};

struct learned_factors
{
    double**         user_factor_vectors;
    double**         item_factor_vectors;

	unsigned int     dimensionality;

	unsigned int     users_number;
	unsigned int     items_number;
};

typedef struct training_set       training_set_t;

typedef struct model_parameters   model_parameters_t;

typedef struct learned_factors    learned_factors_t;

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
learn(struct training_set* tset, struct model_parameters* params);


/*------------------------------------------------------------------------------------
 *
 *                          Rating estimation/computaion
 *
 *------------------------------------------------------------------------------------
 */

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
estimate_item_rating(double* user_vector, double* item_vector, unsigned int dim);

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
                          unsigned int size);


/*------------------------------------------------------------------------------------
 *
 *                                Helper functions
 *
 *------------------------------------------------------------------------------------
 */

/*
 * init_training_set:  allocate space for the training set
 *                               
 *
 * Arguments:
 *      params        Parameters of the model
 *
 * Returns:
 *      Return an initialized training set.
 *
 */
struct training_set* 
init_training_set(struct model_parameters* params);

/*
 * free_training_set:  delete the training set from memory
 */
void
free_training_set(training_set_t* tset);

/*
 * free_learned_factors:  delete the learned factors from memory
 */
void 
free_learned_factors(learned_factors_t* lfactors);

/*
 * set_known_rating: fill the training set with a known user/item rating
 *                               
 *
 * Arguments:
 *      user_index: The index of a user
 *		user_index: The index of an item
 *		value:      The rating of an item
 *      tset:       The training set to be filled
 *
 */
void set_known_rating(int user_index, int item_index, double value, training_set_t* tset);


/*
* estimate_rating_from_factors:  Return the approximates user’s rating of an item based on 
*                                some learned factors.
*
* Arguments:
*      user_index: The index of a user
*	   user_index: The index of an item
*      lfactors  : Learned factors
*
* Returns:
*      The estimated rating.
*
*/
double
estimate_rating_from_factors(int user_index, int item_index, learned_factors_t* lfactors);