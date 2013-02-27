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

#ifndef RECOMMENDED_ITEMS_H
#define RECOMMENDED_ITEMS_H

#include "model_parameters.h"
#include "binary_heap.h"

struct recommended_item
{
	size_t	index;
	float	rating;
};

typedef struct recommended_item recommended_item_t;

struct recommended_items
{
	size_t				items_number;

	size_t				filled_items_nb;

	recommended_item_t*	items;

	binary_heap_t*		bheap;
};

typedef struct recommended_items recommended_items_t;

/*
 * Create a new recommended items set
 */
recommended_items_t* 
init_recommended_items(size_t items_number);

/*
 * free_recommended_items:  delete the recommended items from memory
 */
void 
free_recommended_items(recommended_items_t* items);


void
insert_recommended_item(size_t index, float _value, recommended_items_t* items);

#endif /* RECOMMENDED_ITEMS_H */

