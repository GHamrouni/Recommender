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

#include "learned_factors.h"
#include "model_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>

struct learned_factors* 
init_learned_factors(struct model_parameters params)
{
	struct learned_factors* lfactors = 
		malloc(sizeof(struct learned_factors));

	size_t i = 0;
	size_t j = 0;

	if (!lfactors)
		return NULL;

	lfactors->item_factor_vectors =
	       	malloc(sizeof(double*) * params.items_number);

	lfactors->user_factor_vectors =
	       	malloc(sizeof(double*) * params.users_number);

	lfactors->item_bias =
	       	malloc(sizeof(double) * params.items_number);

	lfactors->user_bias =
	       	malloc(sizeof(double) * params.users_number);

	lfactors->ratings_average = 0;

	if (!lfactors->item_factor_vectors ||
		!lfactors->user_factor_vectors ||
		!lfactors->item_bias ||
		!lfactors->user_bias)
	{
		return lfactors;
	}

	for (i = 0; i < params.items_number; i++)
	{
		lfactors->item_factor_vectors[i] =  
			malloc(sizeof(double) * params.dimensionality);
		
		if (lfactors->item_factor_vectors[i])
		{
			for (j = 0; j < params.dimensionality; j++)
			{
				lfactors->item_factor_vectors[i][j] = 0.1;
			}
		}

		lfactors->item_bias[i] = 0.0;
	}

	for (i = 0; i < params.users_number; i++)
	{
		lfactors->user_factor_vectors[i] =  
			malloc(sizeof(double) * params.dimensionality);

		if (lfactors->user_factor_vectors[i])
		{
			for (j = 0; j < params.dimensionality; j++)
			{
				lfactors->user_factor_vectors[i][j] = 0.1;
			}
		}

		lfactors->user_bias[i] = 0.0;
	}

	return lfactors;
}

/*
 * free_learned_factors:  delete the learned factors from memory
 */
void 
free_learned_factors(learned_factors_t* lfactors)
{
	size_t i;

	if (!lfactors)
		return;

	for (i = 0; i < lfactors->items_number; i++)
		free(lfactors->item_factor_vectors[i]);

	free(lfactors->item_factor_vectors);

	for (i = 0; i < lfactors->users_number; i++)
		free(lfactors->user_factor_vectors[i]);

	free(lfactors->user_factor_vectors);

	free(lfactors->item_bias);
	free(lfactors->user_bias);

	free(lfactors);
}



void save_learned_factors (learned_factors_t* lfactors, char * file_path)
{
	FILE* file;
	size_t i;
	file = fopen (file_path, "wb");

	assert (file);
	fwrite (& (lfactors->items_number), sizeof (size_t), 1, file);
	fwrite (& (lfactors->users_number), sizeof (size_t), 1, file);
	fwrite (& (lfactors->dimensionality), sizeof (size_t), 1, file);
	fwrite (& (lfactors->ratings_average), sizeof (float), 1, file);

	fwrite (lfactors->item_bias, sizeof (double), lfactors->items_number, file);
	fwrite (lfactors->user_bias, sizeof (double), lfactors->users_number, file);


	for (i = 0; i < lfactors->items_number; i++)
	{
		if (fwrite (lfactors->item_factor_vectors[i], sizeof (double), lfactors->dimensionality, file) == 0)
		{
			printf ("");
		}
	}

	for (i = 0; i < lfactors->users_number; i++)
	{
		fwrite (lfactors->user_factor_vectors[i], sizeof (double), lfactors->dimensionality, file);
	}
	fclose (file);
}


learned_factors_t* load_learned_factors (char * file_path)
{
	FILE* file;
	size_t i;
	learned_factors_t* lfactors = malloc (sizeof (learned_factors_t) );
	file = fopen (file_path, "rb");

	assert (file);
	fread (&lfactors->items_number, sizeof (size_t), 1, file);
	fread (&lfactors->users_number, sizeof (size_t), 1, file);
	fread (&lfactors->dimensionality, sizeof (size_t), 1, file);
	fread (&lfactors->ratings_average, sizeof (float), 1, file);

	lfactors->item_bias = malloc (sizeof (double) * lfactors->items_number);
	lfactors->user_bias = malloc (sizeof (double) * lfactors->users_number);
	fread (lfactors->item_bias, sizeof (double), lfactors->items_number, file);
	fread (lfactors->user_bias, sizeof (double), lfactors->users_number, file);




	lfactors->item_factor_vectors =
	    malloc (sizeof (double*) * lfactors->items_number);
	for (i = 0; i < lfactors->items_number; i++)
	{
		lfactors->item_factor_vectors[i] = malloc (sizeof (double) * lfactors->dimensionality);
		if (fread (lfactors->item_factor_vectors[i], sizeof (double), lfactors->dimensionality, file) < lfactors->dimensionality)
		{
			printf ("");
		}
	}

	lfactors->user_factor_vectors =
	    malloc (sizeof (double*) * lfactors->users_number);
	for (i = 0; i < lfactors->users_number; i++)
	{
		lfactors->user_factor_vectors[i] = malloc (sizeof (double) * lfactors->dimensionality);
		if (fread (lfactors->user_factor_vectors[i], sizeof (double), lfactors->dimensionality, file) < lfactors->dimensionality)
		{
			printf ("");
		}
	}
	fclose (file);
	return lfactors;
}