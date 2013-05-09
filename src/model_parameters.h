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

#ifndef MODEL_PARAMETERS_H
#define MODEL_PARAMETERS_H

#include <stddef.h>

#define BIAS 1
#define NEIGHBOURS_MF 2
#define SOCIAL 3

struct model_parameters
{
	size_t			users_number;
	size_t			items_number;

	size_t			dimensionality; /* dimensionality of the joint latent factor space */
	size_t			iteration_number;

	size_t			training_set_size;  /* The number of known ratings */

	float			lambda;          /* The constant lambda controls the extent of regularization */
	float			step;            /* step size in stochastic gradient descent algorithm */

	float			step_bias;
	float			lambda_bias;

	int				algoithm_type;

	int 			seed;
	int 			bin_width;
	int 			proj_family_size;
	float			betha;
};

typedef struct model_parameters model_parameters_t;

#endif /* MODEL_PARAMETERS_H */


