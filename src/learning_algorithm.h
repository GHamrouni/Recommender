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

#ifndef LEARNING_ALGORITHM_H
#define LEARNING_ALGORITHM_H
#include "sparse_matrix.h"

struct learning_algorithm_params
{
	struct training_set* tset; 
	struct model_parameters params;
	sparse_matrix_t* social_matrix;
};
typedef struct learning_algorithm_params learning_algorithm_params_t ;


/************************************************************************/
/*                         Learning algorithm                           */
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
typedef struct learned_factors* (*learning_algorithm_t)(learning_algorithm_params_t* learning_param);

typedef void (*update_learning_t) (training_set_t * old_tset,training_set_t* new_tset,learned_factors_t* lfactors, model_parameters_t* params);

#endif /* LEARNING_ALGORITHM_H */
