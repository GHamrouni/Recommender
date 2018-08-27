/*-
* Copyright (c) 2012 Hamrouni Ghassen.
*
*  2013-02  Modified for dynamic resizing by Ahmed Ben Romdhane
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

#ifndef LEARNED_FACTORS_H
#define LEARNED_FACTORS_H

#include "model_parameters.h"
#include "items_rated_by_user.h"

struct learned_factors
{
	size_t		users_number;
	size_t		items_number;
	size_t		dimensionality;

	double*         user_bias;
	double*         item_bias;

	float           ratings_average;

	double**        user_factor_vectors;
	double**        item_factor_vectors;

	double** x;
	double** y;

	items_rated_by_user_t * R,*R_K;
};


typedef struct learned_factors learned_factors_t;

/*
 * Create a new factors set
 */
learned_factors_t* 
init_learned_factors(model_parameters_t* params);

/*
 * free_learned_factors:  delete the learned factors from memory
 */
void 
free_learned_factors(learned_factors_t* lfactors);

#endif /* LEARNED_FACTORS_H */

