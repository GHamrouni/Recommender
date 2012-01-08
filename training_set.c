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

    tset->ratings = malloc(sizeof(rating_t) * params.training_set_size);

    tset->current_rating_index = 0;
    tset->training_set_size = params.training_set_size;
    tset->dimensionality = params.dimensionality;
         
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

    tset->current_rating_index++;
}
