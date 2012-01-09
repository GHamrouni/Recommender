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

/*----------------------------------------------------------------------------------------------
 *
 *                                     Helper functions
 *
 *----------------------------------------------------------------------------------------------
 */

/*
 * init_training_set:  allocate space for the training set
 */
struct training_set* 
init_training_set(struct model_parameters params)
{
    struct training_set* tset = malloc(sizeof(struct training_set));
	unsigned int i = 0;
	unsigned int j = 0;

	tset->ratings_sum = 0;

    tset->ratings = malloc(sizeof(rating_t) * params.training_set_size);

    tset->current_rating_index = 0;
    tset->training_set_size = params.training_set_size;
    tset->dimensionality = params.dimensionality;

	tset->ratings_matrix = malloc(sizeof(rating_t*) * params.items_number);
	tset->implicit_feedback = malloc(sizeof(double*) * params.items_number);

	for (i = 0; i < params.items_number; i++)
	{
		tset->ratings_matrix[i] =  malloc(sizeof(rating_t) * params.users_number);
		tset->implicit_feedback[i] =  malloc(sizeof(double) * params.users_number);

		for (j = 0; j < params.users_number; j++)
		{
			tset->ratings_matrix[i][j].is_known = 0;
			tset->implicit_feedback[i][j] = 0;
		}
	}
         
    return tset;
}

/*
 * free_training_set:  delete the training set from memory
 */
void
free_training_set(training_set_t* tset)
{
    free(tset->ratings);
    free(tset);
}

/*
 * set_known_rating: fill the training set with a known user/item rating                            
 */
void set_known_rating(int user_index, int item_index, double _value, training_set_t* tset)
{
    tset->ratings[tset->current_rating_index].user_index = user_index;
    tset->ratings[tset->current_rating_index].item_index = item_index; 
    tset->ratings[tset->current_rating_index].value = _value;

	tset->ratings_matrix[item_index][user_index] = tset->ratings[tset->current_rating_index];

	tset->ratings_sum = tset->ratings_sum + _value;

    tset->current_rating_index++;
}


void set_implicit_feedback(int user_index, int item_index, training_set_t* tset)
{
	tset->implicit_feedback[item_index][user_index] = 1;
}

/*
 * user_ratings_average: get the average of the vector formed by 
 *                              the ratings of a user
 */
double 
user_ratings_average(int user_index, training_set_t* tset, struct model_parameters params)
{
	unsigned int i = 0;
	double sum = 0;
	double N = 0;

	for (i = 0; i < params.items_number; i++)
	{
		if (tset->ratings_matrix[i][user_index].is_known)
		{
			double f = tset->ratings_matrix[i][user_index].value;

			sum += f;

			N++;
		}
	}

	if (N == 0)
		return 0;

	return sum / N;
}

/*
 * item_ratings_average: get the average of the vector formed by 
 *                              the ratings of an item
 */
double 
item_ratings_average(int item_index, training_set_t* tset, struct model_parameters params)
{
	unsigned int i = 0;
	double sum = 0;
	double N = 0;

	for (i = 0; i < params.users_number; i++)
	{
		if (tset->ratings_matrix[item_index][i].is_known)
		{
		    double f = tset->ratings_matrix[item_index][i].value;

		    sum += f;

			N++;
		}
	}

	if (N == 0)
		return 0;

	return sum / N;
}

double implicit_feedback_magnitude(int user_index, training_set_t* tset, struct model_parameters params)
{
	unsigned int i = 0;
	double sum = 0;

	for (i = 0; i < params.items_number; i++)
	{
		double f = tset->implicit_feedback[i][user_index];

		sum += f * f;
	}

	return sqrtf(sum);
}

double implicit_feedback_sum(int user_index, training_set_t* tset, struct model_parameters params)
{
	unsigned int i = 0;
	double sum = 0;

	for (i = 0; i < params.items_number; i++)
	{
		double f = tset->implicit_feedback[i][user_index];

		sum += f;
	}

	return sum;
}