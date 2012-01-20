#include "sparse_matrix.h"

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

coo_matrix_t*
init_coo_matrix(unsigned int max_size)
{
	coo_matrix_t* matrix = malloc(sizeof(coo_matrix_t));

	if (!matrix)
		return NULL;

	matrix->current_size = 0;
	matrix->size = max_size;

	matrix->entries = malloc(sizeof(coo_entry_t) * max_size);

	return matrix;
}

void
free_coo_matrix(coo_matrix_t* matrix)
{
	free(matrix->entries);
	free(matrix);
}

void
insert_coo_matrix(double val, unsigned int row_i, unsigned int column_j, coo_matrix_t* matrix)
{
	assert(matrix->current_size < matrix->size);

	matrix->entries[matrix->current_size].row_i = row_i;
	matrix->entries[matrix->current_size].column_j = column_j; 
	matrix->entries[matrix->current_size].value = val;

	matrix->current_size++;
}

int entry_cmp(const void *e1, const void *e2)
{
	int diff = ((coo_entry_t*) e1)->row_i - ((coo_entry_t*) e2)->row_i;

	if (diff != 0)
		return diff;

	return ((coo_entry_t*) e1)->column_j - ((coo_entry_t*) e2)->column_j;
}

void
sort_coo_matrix(coo_matrix_t* matrix)
{
	qsort(matrix->entries, matrix->size, sizeof(coo_entry_t), entry_cmp);
}

sparse_matrix_t* 
init_sparse_matrix(coo_matrix_t* c_matrix, unsigned int row_nb, unsigned int column_nb)
{
	unsigned int i = 0;
	int current_row = -1;

	sparse_matrix_t* matrix = malloc(sizeof(sparse_matrix_t));

	if (!matrix)
		return NULL;

	matrix->column_nb = column_nb;
	matrix->row_nb = row_nb;
	matrix->nonzero_entries_nb = c_matrix->size;

	matrix->values = malloc(sizeof(double) * c_matrix->size);
	matrix->row_index = malloc(sizeof(unsigned int) * (row_nb + 1));
	matrix->column_index = malloc(sizeof(int) * c_matrix->size);

	for (i = 0; i < row_nb + 1; i++)
		matrix->row_index[i] = 0;

	for (i = 0; i < c_matrix->size; i++)
	{
		if (current_row != c_matrix->entries[i].row_i)
		{
			if (c_matrix->entries[i].row_i < row_nb + 1)
			{
				matrix->row_index[c_matrix->entries[i].row_i] = i + 1;
				current_row = c_matrix->entries[i].row_i;
			}
		} else if (i == (c_matrix->size - 1))
		{
			if (c_matrix->entries[i].row_i == row_nb - 1)
				matrix->row_index[row_nb] = i + 2;
		}

		matrix->values[i] = c_matrix->entries[i].value;
		matrix->column_index[i] = c_matrix->entries[i].column_j;
	}

	return matrix;
}

void
free_sparse_matrix(sparse_matrix_t* matrix)
{
	free(matrix->column_index);
	free(matrix->row_index);
	free(matrix->values);
	free(matrix);
}

int
element_exists(unsigned int row_i, unsigned int column_j, sparse_matrix_t* matrix)
{
	int i = 0;
	int r1, r2;
	r1 = r2 = 0; //Range

	assert(row_i < matrix->row_nb);
	assert(column_j < matrix->column_nb);

	if (!matrix->row_index[row_i]) return 0;

	r1 = matrix->row_index[row_i] - 1;
	r2 = matrix->row_index[row_i + 1] - 1;

	for (i = r1; i < r2; i++)
		if (matrix->column_index[i] == column_j)
			return 1;

	return 0;
}

double 
get_element(unsigned int row_i, unsigned int column_j, sparse_matrix_t* matrix)
{
	int i = 0;
	int r1, r2;
	r1 = r2 = 0; //Range

	assert(row_i < matrix->row_nb);
	assert(column_j < matrix->column_nb);

	if (!matrix->row_index[row_i]) return 0;

	r1 = matrix->row_index[row_i] - 1;
	r2 = matrix->row_index[row_i + 1] - 1;

	for (i = r1; i < r2; i++)
		if (matrix->column_index[i] == column_j)
			return matrix->values[i];

	return 0;
}

double
row_values_average(unsigned int row_i, sparse_matrix_t* matrix)
{
	int i = 0;
	int r1, r2;
	double sum = 0;
	double N = 0;

	r1 = r2 = 0; //Range

	if (!matrix->row_index[row_i]) return 0;

	r1 = matrix->row_index[row_i] - 1;
	r2 = matrix->row_index[row_i + 1] - 1;

	for (i = r1; i < r2; i++)
	{
		N++;
		sum += matrix->values[i];
	}

	if (N == 0)
		return 0;

	return sum / N;
}

double
column_values_average(unsigned int column_j, sparse_matrix_t* matrix)
{
	unsigned int i = 0;

	double sum = 0;
	double N = 0;

	for (i = 0; i < matrix->nonzero_entries_nb; i++)
	{
		if ( matrix->column_index[i] == column_j)
		{
			N++;
			sum += matrix->values[i];
		}
	}

	if (N == 0)
		return 0;

	return sum / N;
}
