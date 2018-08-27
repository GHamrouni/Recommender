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

#include "training_set.h"
#include "model_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include <assert.h>

/*----------------------------------------------------------------------------------------------
 *
 *                                     Helper functions
 *
 *---------------------------------------------------------------------------------------------- */

/*
 * init_training_set:  allocate space for the training set
 */
struct training_set* 
init_training_set(struct model_parameters * params)
{
	struct training_set* tset = malloc(sizeof(struct training_set));

	if (!tset)
		return NULL;

	tset->ratings_sum = 0;

	tset->ratings = init_coo_matrix(params->training_set_size);

	tset->training_set_size = params->training_set_size;
	tset->dimensionality = params->dimensionality;

	tset->ratings_matrix = NULL;

	tset->users_number = params->users_number;
	tset->items_number = params->items_number;
     
	return tset;
}

void
compile_training_set(training_set_t* tset)
{
	assert(tset->ratings);

	sort_coo_matrix(tset->ratings);

	tset->ratings_matrix = 
		init_sparse_matrix(tset->ratings, 
				tset->items_number, 
				tset->users_number);
}

/*
 * free_training_set:  delete the training set from memory
 */
void
free_training_set(training_set_t* tset)
{
    free_coo_matrix(tset->ratings);

	if (tset->ratings_matrix)
		free_sparse_matrix(tset->ratings_matrix);

    free(tset);
}

/*
 * set_known_rating: fill the training set with a known user/item rating                            
 */
void 
set_known_rating(size_t user_index, size_t item_index, float _value, training_set_t* tset)
{
	insert_coo_matrix(_value, item_index, user_index, tset->ratings);

	assert(user_index < tset->users_number);
	assert(item_index < tset->items_number);

	tset->ratings_sum = tset->ratings_sum + _value;
}

/*
 * user_ratings_average: get the average of the vector formed by 
 *                              the ratings of a user
 */
double 
user_ratings_average(size_t user_index, training_set_t* tset)
{
	assert(tset->ratings_matrix);

	return column_values_average(user_index, tset->ratings_matrix);
}

/*
 * item_ratings_average: get the average of the vector formed by 
 *                              the ratings of an item
 */
double 
item_ratings_average(size_t item_index, training_set_t* tset)
{
	assert(tset->ratings_matrix);

	return row_values_average(item_index, tset->ratings_matrix);
}


void
add_user (training_set_t* tset)
{
	add_column (tset->ratings_matrix);
	tset->items_number++;
}

void
add_item (training_set_t* tset)
{
	add_row (tset->ratings_matrix);
	tset->users_number++;
}

void add_rating (size_t user_index, size_t item_index, float _value, training_set_t* tset)
{
	insert_value (tset->ratings_matrix,  user_index, item_index , _value);
	tset->training_set_size++;
	tset->ratings_sum += _value;
}


void add_training_set(training_set_t * tset, training_set_t * new_tset)
{
	size_t r;
	double ratings_sum = tset->ratings_sum;
	insert_coo_to_coo(tset->ratings,new_tset->ratings);
	tset->training_set_size += new_tset->training_set_size;
	for (r = 0; r < new_tset->training_set_size; r++)
	{
		ratings_sum += new_tset->ratings->entries[r].value;
	}
	tset->ratings_sum = ratings_sum;
	compile_training_set(tset);

}
