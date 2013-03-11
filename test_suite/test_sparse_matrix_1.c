#include "../src/recommender.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <stddef.h>

#include "../src/matrix_factorization.h"
#include "../src/matrix_factorization_bias.h"
#include "../src/sparse_matrix.h"

#include "test_sparse_matrix_1.h"

#define ROW_MATRIX_TEST_SIZE 4

#define COLUMN_MATRIX_TEST_SIZE 4

#define COO_MATRIX_TEST_SIZE 12

int test_sparse_matrix_1(void)
{
	size_t i, u, v;
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

	//add_rows(s_matrix,2);
	add_row(s_matrix);
	/*add_row(s_matrix);
	add_row(s_matrix);
	add_row(s_matrix);
	add_row(s_matrix);
	add_row(s_matrix);

	add_row(s_matrix);*/
	add_column(s_matrix);

	insert_value(s_matrix,4,4,1);
/*
	s_matrix=insert(s_matrix,4,4,2);
	s_matrix=insert(s_matrix,3,0,3);
	s_matrix=insert(s_matrix,4,0,3);
	s_matrix=insert(s_matrix,4,1,3);
	s_matrix=insert(s_matrix,4,3,3);
	s_matrix=insert(s_matrix,4,2,3);
	s_matrix=insert(s_matrix,5,0,3);
	s_matrix=insert(s_matrix,5,2,3);
	s_matrix=insert(s_matrix,5,1,3);
	s_matrix=insert(s_matrix,5,3,3);
	s_matrix=insert(s_matrix,5,4,3);
	s_matrix=insert(s_matrix,6,0,3);
	s_matrix=insert(s_matrix,6,1,3);
	s_matrix=insert(s_matrix,6,2,3);
	s_matrix=insert(s_matrix,6,3,3);
	s_matrix=insert(s_matrix,6,4,3);*/

	/*c_matrix = init_coo_matrix(COO_MATRIX_TEST_SIZE+1);
	insert_coo_matrix(2, 4, 4, c_matrix);
	insert_coo_matrix(3, 0, 3, c_matrix);
	insert_coo_matrix(3, 4, 0, c_matrix);
	insert_coo_matrix(3, 3, 0, c_matrix);
	
	insert_coo_matrix(3, 4, 1, c_matrix);
	insert_coo_matrix(3, 4, 3, c_matrix);
	insert_coo_matrix(3, 4, 2, c_matrix);
	insert_coo_matrix(3, 2, 0, c_matrix);

	insert_coo_matrix(3, 5, 0, c_matrix);
	insert_coo_matrix(3, 5, 1, c_matrix);
	insert_coo_matrix(3, 5, 2, c_matrix);
	insert_coo_matrix(3, 5, 3, c_matrix);

	insert_coo_matrix(3, 6, 0, c_matrix);*/
	//insert_coo_matrix(3, 6, 1, c_matrix);
	//insert_coo_matrix(3, 6, 2, c_matrix);
	//insert_coo_matrix(3, 6, 3, c_matrix);
	insert_coo(s_matrix,c_matrix);
	/*for (i = 0; i < COO_MATRIX_TEST_SIZE; i++)
	{
		u = c_matrix->entries[i].row_i;
		v = c_matrix->entries[i].column_j;

		assert(element_exists(u, v, s_matrix));

		val = get_element(u, v, s_matrix);

		assert(val == c_matrix->entries[i].value);
	}*/

//	val = get_element(4, 4, s_matrix);
// 	for(v=0;v < s_matrix->column_nb; v++)
// 	{
// 		for(u=0 ;u < s_matrix->row_nb;u++)
// 		{
// 			val = get_element(u, v, s_matrix);
// 			printf("%f ",val);
// 		}
// 			printf("\n");
// 	}


	free_coo_matrix(c_matrix);
	free_sparse_matrix(s_matrix);

	printf("Test sparse matrix [OK] \n");

	return 0;
}