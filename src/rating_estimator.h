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

#ifndef RATING_ESTIMATOR_H
#define RATING_ESTIMATOR_H
#include "training_set.h"
/************************************************************************/
/*                         Rating estimator                             */
/************************************************************************/

struct rating_estimator_parameters
{

	size_t user_index;
	size_t item_index; 
	struct learned_factors* lfactors;
	training_set_t* tset;
};

typedef struct rating_estimator_parameters rating_estimator_parameters_t;
/*
 * estimate_rating_from_factors:  Return the approximates user’s rating of an item based on 
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
typedef double (*rating_estimator_t)(rating_estimator_parameters_t* rating_estimator_param);

#endif /* RATING_ESTIMATOR_H */
