#include "test_red_black_tree_1.h"
#include "test_sparse_matrix_1.h"
#include "test_training_set_1.h"
#include "test_bheap_1.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
	int return_value;
	clock_t start_c, end_c;

	start_c = clock();

	return_value = test_red_black_tree_1();

	assert(!return_value);

	return_value = test_sparse_matrix_1();

	assert(!return_value);
	
	return_value = test_training_set_1();

	assert(!return_value);

	return_value = test_bheap_1();

	assert(!return_value);

	end_c = clock();
	printf("Time : %f s \n", (double)(end_c - start_c)/ CLOCKS_PER_SEC);

	return 0;
}