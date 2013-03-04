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

#include "recommended_items.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include "utils.h"

/*
 * Define a weak strict order between items.
 */
int 
cmp_recommended_item(const void* a, const void* b)
{
	recommended_item_t* item_a = (recommended_item_t*)a;
	recommended_item_t* item_b = (recommended_item_t*)b;

	if (item_a->rating < item_b->rating)
		return -1;

	if (item_a->rating > item_b->rating)
		return 1;

	return 0;
}

/*
 * Recommended item constructor
 */
recommended_item_t* 
new_recommended_item(size_t index, float _value)
{
	recommended_item_t* r_item = malloc(sizeof(recommended_item_t));

	if (!r_item)
		return NULL;

	r_item->index = index;
	r_item->rating = _value;

	return r_item;
}

/*
 * Recommended item destructor
 */
void 
destruct_recommended_item(void* v)
{
	recommended_item_t* _v =(recommended_item_t*) v;
	free(_v);
}

/*
 * Define a copy operator dest = src.
 */
void
copy_recommended_item(const void* src, void* dest)
{
	recommended_item_t* item_src  = (recommended_item_t*) src;
	recommended_item_t* item_dest = (recommended_item_t*) dest;

	item_dest->index = item_src->index;
	item_dest->rating = item_src->rating;
}

float
get_item_rating_from_node(const void* src)
{
	return ((recommended_item_t*) src)->rating;
}

/*
 * Create a new recommended items set
 */
recommended_items_t* 
init_recommended_items(size_t items_number)
{
	recommended_items_t* r_items = malloc(sizeof(recommended_items_t));

	if (!r_items)
		return NULL;

	r_items->items_number = items_number;
	r_items->filled_items_nb = 0;
	r_items->items = malloc(sizeof(recommended_item_t) * items_number);
	r_items->bheap = init_binary_heap(items_number, cmp_recommended_item);

	return r_items;
}

/*
 * free_recommended_items:  delete the recommended items from memory
 */
void 
free_recommended_items(recommended_items_t* items)
{
	if (items)
	{
		if (items->bheap)
			free_binary_heap(items->bheap);

		if (items->items)
			free(items->items); 

		free(items);
	}
}

void
insert_recommended_item(size_t index, float _value, recommended_items_t* items)
{
	recommended_item_t* item = NULL;

	if (!items)
		return;

	item = malloc(sizeof(recommended_item_t));

	if (item)
	{
		item->index = index;
		item->rating = _value;
	}

	insert_binary_heap(item, items->bheap);
}

