#include "../recommender.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <stddef.h>

#include "../binary_heap.h"

#include "test_bheap_1.h"

int i_compare (const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}

int test_bheap_1(void)
{
	binary_heap_t* bheap_max = NULL;
	binary_heap_t* bheap_min = NULL;

	size_t i = 0;
	size_t v = 0;
	size_t binary_heap_size = 10;
	size_t N = 1000;

	int* sorted_data_array = malloc(sizeof(int) * N);

	int* heapsorted_data_array_max = malloc(sizeof(int) * binary_heap_size);
	int* heapsorted_data_array_min = malloc(sizeof(int) * binary_heap_size);

	printf("Test binary heap ... \n");

	bheap_max = init_binary_heap(binary_heap_size, MAX_HEAP);
	bheap_min = init_binary_heap(binary_heap_size, MIN_HEAP);
	
	assert(bheap_max);
	assert(bheap_min);

	if (!bheap_max)
		return 1;

	if (!bheap_min)
		return 1;

	srand ( 1 );

	for (i = 0; i < N; i++)
	{
		v = (rand() % 1000);

		sorted_data_array[i] = v;
		insert_binary_heap(v, bheap_max);
		insert_binary_heap(v, bheap_min);
	}

	qsort (sorted_data_array, N, sizeof(int), i_compare);

	i = 0;
	while (bheap_max->filled_elements)
	{
		heapsorted_data_array_max[i] = bheap_max->buffer[0];
		heapsorted_data_array_min[i] = bheap_min->buffer[0];

		pop_binary_heap(bheap_max);
		pop_binary_heap(bheap_min);
		i++;
	}

	for (i = 0; i < binary_heap_size; i++)
	{
		assert(heapsorted_data_array_min[i] == sorted_data_array[binary_heap_size - i - 1]);
		assert(heapsorted_data_array_max[i] == sorted_data_array[N - (binary_heap_size - i)]);
	}

	free_binary_heap(bheap_max);
	free(heapsorted_data_array_max);
	free(heapsorted_data_array_min);
	free(sorted_data_array);
	
	printf("Test binary heap [OK] \n");

	return 0;
}