#include "test_red_black_tree_1.h"
#include "test_sparse_matrix_1.h"

#include <assert.h>

int main(void)
{
	int return_value;

	return_value = test_red_black_tree_1();

	assert(!return_value);

	return_value = test_sparse_matrix_1();

	assert(!return_value);

	return 0;
}