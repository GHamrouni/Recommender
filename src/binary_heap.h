/*-
* Copyright (c) 2012, 2013 Hamrouni Ghassen.
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


#ifndef _BINARY_HEAP_H_
#define _BINARY_HEAP_H_

#include <stddef.h>

/*
 * Define a weak strict order between values.
 */
typedef int (*bh_value_cmp)(const void *, const void *);

/*
 * Define value destructor/it will be called when releasing a value.
 */
typedef void (*bh_value_destructor)(void *);

/*
 * binary_heap:  The heap is tree based data structure                           
 *
 * Members:
 *      max_size        The number of elements in the binary heap
 *		filled_elements The number of inserted elements so far
 *		buffer			The buffer is a dynamically allocated array 
 *						containing the heap's elements
 *
 */
typedef struct binary_heap
{
	size_t          max_size;
	size_t          filled_elements;
	bh_value_cmp	value_comparer;
	void*           buffer[1];
} binary_heap_t;

/*
 * init_binary_heap:  Learn using training set and the model parameters
 *                               
 *
 * Arguments:
 *      max_size        The number of elements in the binary heap
 *
 * Returns:
 *      Return an initialized binary heap.
 *
 */
binary_heap_t* 
init_binary_heap(size_t max_size, bh_value_cmp value_comparer);

/*
 * free_binary_heap:  Remove the entire binary heap
 *                               
 *
 * Arguments:
 *		bheap		 An initialized binary heap
 *
 * Time Complexity:
 *		O(1)
 *
 *
 */
void
free_binary_heap(binary_heap_t* bheap);

/*
 * insert_binary_heap:  Insert an element to the heap
 *                               
 *
 * Arguments:
 *      value        The value to insert
 *		bheap		 An initialized binary heap
 *
 * Time Complexity:
 *		O(log(n))
 *
 *
 */
void
insert_binary_heap(void* value, binary_heap_t* bheap);

/*
 * remove_binary_heap:  Remove the max element from the binary heap
 *                               
 *
 * Arguments:
 *		bheap		 An initialized binary heap
 *
 * Time Complexity:
 *		O(log(n))
 *
 *
 */
void*
pop_binary_heap(binary_heap_t* bheap);

#endif
