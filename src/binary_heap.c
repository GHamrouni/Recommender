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



#include "binary_heap.h"

#include <stdlib.h>
#include <assert.h>


binary_heap_t* 
init_binary_heap (
	size_t max_size,
	bh_value_cmp value_comparer
	)
{
	binary_heap_t* bheap = 
		malloc(sizeof(binary_heap_t) - 1 + sizeof(void*) * max_size);

	if (!bheap)
		return NULL;

	bheap->filled_elements = 0;
	bheap->max_size = max_size;
	bheap->value_comparer = value_comparer;
	
	return bheap;
}

static int 
getParentIndex(int i)
{
	return (i - 1) / 2;
}

static int 
getRChildIndex(int i)
{
	return 2 * i + 2;
}

static int 
getLChildIndex(int i)
{
	return 2 * i + 1;
}

void 
balance_heap(binary_heap_t* bheap)
{
	size_t i = bheap->filled_elements - 1;
	int index = 0;

	while (getParentIndex(i) >= 0 
		   && 
		   bheap->value_comparer(bheap->buffer[i], bheap->buffer[getParentIndex(i)]) < 0)
	{
		void* value = bheap->buffer[i];

		index = getParentIndex(i);
		bheap->buffer[i] = bheap->buffer[index];
		bheap->buffer[index] = value;

		i = index;
	}
}

void 
balance_children(size_t i, binary_heap_t* bheap)
{
	size_t mc;

	size_t rc = getRChildIndex(i);
	size_t lc = getLChildIndex(i);

	if (lc >= bheap->filled_elements)
		return;

	if (rc < bheap->filled_elements)
	{
		if (bheap->value_comparer(bheap->buffer[rc], bheap->buffer[lc]) < 0)
			mc = rc;
		else
			mc = lc;
	} else
		mc = lc;

	if (bheap->value_comparer(bheap->buffer[mc], bheap->buffer[i]) < 0)
	{
		void* value = bheap->buffer[mc];
		bheap->buffer[mc] = bheap->buffer[i];
		bheap->buffer[i] = value;

		balance_children(mc, bheap);
	}
}

void*
pop_binary_heap(binary_heap_t* bheap)
{
	void* value = NULL;

	if (bheap->filled_elements <= 0)
		return NULL;

	value = bheap->buffer[0];

	bheap->buffer[0] = bheap->buffer[bheap->filled_elements - 1];
	bheap->filled_elements--;

	balance_children(0, bheap);

	return value;
}

void
insert_binary_heap(void* value, binary_heap_t* bheap)
{
	if (bheap->filled_elements == 0)
	{
		bheap->filled_elements = 1;
		bheap->buffer[0] = value;

		return;
	}

	if (bheap->filled_elements < bheap->max_size)
	{
		bheap->buffer[bheap->filled_elements] = value;
		bheap->filled_elements++;

		balance_heap(bheap);

		return;
	}

	if (bheap->value_comparer(bheap->buffer[0], value) < 0)
	{
		bheap->buffer[0] = value;
		balance_children(0, bheap);
	}
}

void
free_binary_heap(binary_heap_t* bheap)
{
	if (bheap)
	{
		free(bheap);
	}
}
