#include "sparse_matrix_hash.h"

#include <math.h>
#include <assert.h>

int
lsh_row(projection_family_t* pfamily, sparse_matrix_t* matrix, size_t row_i)
{
	ptrdiff_t i = 0;
	size_t k = 0;
	ptrdiff_t r1, r2; /* Range */
	unsigned int bwidth, bias;

	int proj_value;
	int hash = 0;

	if (!matrix->row_index[row_i]) return 0;

	r1 = matrix->row_index[row_i] - 1;
	r2 = matrix->row_index[row_i + 1] - 1;

	for (k = 0; k < pfamily->projection_nb; k++)
	{
		double dot_product_r = 0;

		if (r1 >= 0)
			for (i = r1; i < r2; i++)
			{
				int cindex = matrix->column_index[i];
				dot_product_r += pfamily->projections[k]->vector[cindex] * matrix->values[i];
			}

		bwidth = pfamily->projections[k]->bin_width;
		bias = pfamily->projections[k]->bias;

		proj_value = (int) floor((dot_product_r + bias) / (double) bwidth);
		hash = 33 * hash + proj_value;
	}		

	return hash;
}


