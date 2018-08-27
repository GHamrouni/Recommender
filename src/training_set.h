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

#ifndef TRAINING_SET_H
#define TRAINING_SET_H

#include "model_parameters.h"
#include "sparse_matrix.h"

/*
 * The training set is obtained from the past users 
 * behavior or from items ratings.  
 */
struct training_set
{
	size_t			training_set_size;  /* The number of known ratings */
	size_t			dimensionality;     /* dimensionality of the joint latent factor space */

	size_t			users_number;
	size_t			items_number;

	double			ratings_sum;

	coo_matrix_t*		ratings;            /* Known ratings */
	sparse_matrix_t*	ratings_matrix;
};

typedef struct training_set training_set_t;

/************************************************************************/
/*                          Helper functions                            */
/************************************************************************/

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
init_training_set(model_parameters_t* params);

void
compile_training_set(training_set_t* tset);

/*
 * free_training_set:  delete the training set from memory
 */
void
free_training_set(training_set_t* tset);

/*
 * set_known_rating: fill the training set with a known user/item rating
 *                               
 *
 * Arguments:
 *      user_index: The index of a user
 *      user_index: The index of an item
 *      value:      The rating of an item
 *      tset:       The training set to be filled
 *
 */
void 
set_known_rating(size_t user_index, size_t item_index,
	       	float value, training_set_t* tset);

/*
 * user_ratings_average: get the average of the vector formed by 
 *                              the ratings of a user
 *                               
 *
 * Arguments:
 *      user_index: The index of a user
 *      tset:       The training set to be filled
 *      params      Parameters of the model
 *
 */
double 
user_ratings_average(size_t user_index, training_set_t* tset);

/*
 * item_ratings_average: get the average of the vector formed by 
 *                              the ratings of an item
 *                               
 *
 * Arguments:
 *      item_index: The index of an item
 *      tset:       The training set to be filled
 *      params      Parameters of the model
 *
 */
double 
item_ratings_average(size_t item_index, training_set_t* tset);


/*
 * add_rating: add a new known rating to the training set
 *                              
 *                               
 * Arguments:
 *      item_index: The index of an item
 *      user_index: The index of a user
 *      value:     The rating of an item
 *      tset:       The training set to be filled
 *
 */
void
add_rating(size_t user_index, size_t item_index, float _value, training_set_t* tset);


/*
 * add_user : add a new user to the training set
 *                              
 *                               
 * Arguments
 *      tset:       The training set to be filled
 *
 */
void
add_user(training_set_t* tset);

/*
 * add_user : add a new user to the training set
 *                              
 *                               
 * Arguments
 *      tset:       The training set to be filled
 *
 */
void
add_item(training_set_t* tset);


void add_training_set(training_set_t * tset, training_set_t * new_tset);
#endif /* TRAINING_SET_H */
