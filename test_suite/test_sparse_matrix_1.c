#include "../recommender.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <stddef.h>

#include "../matrix_factorization.h"
#include "../matrix_factorization_bias.h"
#include "../sparse_matrix.h"

#include "test_sparse_matrix_1.h"

#define ROW_MATRIX_TEST_SIZE		10000

#define COLUMN_MATRIX_TEST_SIZE		10000

#define COO_MATRIX_TEST_SIZE		1000000

int test_sparse_matrix_1(void)
{
	size_t i, u, v;
	float val;
	unsigned char* used_;

	coo_matrix_t* c_matrix = init_coo_matrix(COO_MATRIX_TEST_SIZE);
	sparse_matrix_t* s_matrix;

	printf("Test sparse matrix ... \n");

	used_ =(unsigned char*) malloc(ROW_MATRIX_TEST_SIZE * COLUMN_MATRIX_TEST_SIZE * sizeof(unsigned char));

	if (!used_)
		return 1;

	for (i = 0; i < ROW_MATRIX_TEST_SIZE * COLUMN_MATRIX_TEST_SIZE; i++)
		used_[i] = 0;

	assert(c_matrix);

	if (!c_matrix)
		return 1;

	srand ( 47 );

	for (i = 0; i < COO_MATRIX_TEST_SIZE; i++)
	{
		u = rand() % ROW_MATRIX_TEST_SIZE;
		v = rand() % COLUMN_MATRIX_TEST_SIZE;

		if (!used_[u * COLUMN_MATRIX_TEST_SIZE + v])
		{
			used_[u * COLUMN_MATRIX_TEST_SIZE + v] = 1;
			insert_coo_matrix((float) rand(), u, v, c_matrix);
		} else i--;
	}

	assert(c_matrix->current_size == COO_MATRIX_TEST_SIZE);

	sort_coo_matrix(c_matrix);

	s_matrix = 
		init_sparse_matrix(c_matrix, 
		ROW_MATRIX_TEST_SIZE, 
		COLUMN_MATRIX_TEST_SIZE);

	for (i = 0; i < COO_MATRIX_TEST_SIZE; i++)
	{
		u = c_matrix->entries[i].row_i;
		v = c_matrix->entries[i].column_j;

		assert(element_exists(u, v, s_matrix));

		val = get_element(u, v, s_matrix);

		assert(val == c_matrix->entries[i].value);
	}

	free_coo_matrix(c_matrix);
	free_sparse_matrix(s_matrix);

	printf("Test sparse matrix [OK] \n");

	return 0;
}