#include "../src/recommender.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>

#include "../src/matrix_factorization.h"
#include "../src/matrix_factorization_bias.h"
#include "../src/sparse_matrix.h"
#include "../src/red_black_tree.h"

#include "test_red_black_tree_1.h"

struct rb_node_value
{
	int value;
};

typedef struct rb_node_value rb_node_value_t;

/*
 * Define a weak strict order between node-values.
 */
int 
cmp_rb_node_value(const void* a, const void* b)
{
	rb_node_value_t* item_a = (rb_node_value_t*)a;
	rb_node_value_t* item_b = (rb_node_value_t*)b;

	if (item_a->value < item_b->value)
		return -1;

	if (item_a->value > item_b->value)
		return 1;

	return 0;
}

/*
 * Recommended item constructor
 */
rb_node_value_t* 
new_rb_node_value(int _value)
{
	rb_node_value_t* r_item = 
		(rb_node_value_t*) malloc(sizeof(rb_node_value_t));

	if (!r_item)
		return NULL;

	r_item->value = _value;

	return r_item;
}

/*
 * Recommended item destructor
 */
void 
destruct_rb_node_value(void* v)
{
	rb_node_value_t* _v =(rb_node_value_t*) v;
	free(_v);
}

/*
 * Define a copy operator dest = src.
 */
void
copy_rb_node_value(const void* src, void* dest)
{
	rb_node_value_t* item_src  = (rb_node_value_t*) src;
	rb_node_value_t* item_dest = (rb_node_value_t*) dest;

	item_dest->value = item_src->value;
}

int
get_rb_node_value_from_node(const void* src)
{
	return ((rb_node_value_t*) src)->value;
}

int 
cmp_integer(const void* a, const void* b)
{
	int* item_a = (int*)a;
	int* item_b = (int*)b;

	if (*item_a < *item_b)
		return -1;

	if (*item_a > *item_b)
		return 1;

	return 0;
}

#define MAX_TREE_VALUES		100000
#define SEED_VALUE			13

int test_red_black_tree_1(void)
{
	red_black_tree_t* tree_1 = init_red_black_tree(cmp_rb_node_value, 
								destruct_rb_node_value, 
								copy_rb_node_value);


	red_black_tree_t* tree_2 = init_red_black_tree(cmp_rb_node_value, 
								destruct_rb_node_value, 
								copy_rb_node_value);

	int i;
	int n;

	void* node = NULL;

	int* sorted_values = malloc(MAX_TREE_VALUES * sizeof(int));
	int* values = malloc(MAX_TREE_VALUES * sizeof(int));

	printf("Test red black tree ... \n");

	assert(tree_1);
	assert(tree_2);

	if (!tree_1 || !tree_2)
		return 1;

	if (!values)
		return 1;

	if (!sorted_values)
		return 1;

	node = rb_max_value(tree_1);

	assert(node == NULL);

	node = rb_min_value(tree_1);

	assert(node == NULL);

	srand ( SEED_VALUE );
	
	for (i = 0; i < MAX_TREE_VALUES; i++)
	{
		n = rand();
		values[i] = n;
		sorted_values[i] = n;

		rb_insert_value(tree_1, new_rb_node_value(n));
		rb_insert_value(tree_2, new_rb_node_value(n));
	}

	qsort(sorted_values, MAX_TREE_VALUES, sizeof(int), cmp_integer);

	n = get_rb_node_value_from_node(rb_max_value(tree_1));

	assert(n == sorted_values[MAX_TREE_VALUES - 1]);

	n = get_rb_node_value_from_node(rb_min_value(tree_1));

	assert(n == sorted_values[0]);

	for (i = 0; i < MAX_TREE_VALUES - 1; i++)
	{
		rb_delete_min_element(tree_1);

		n = get_rb_node_value_from_node(rb_min_value(tree_1));
		assert(sorted_values[i + 1] == n);

		n = get_rb_node_value_from_node(rb_max_value(tree_1));

		assert(n == sorted_values[MAX_TREE_VALUES - 1]);
	}

	for (i = 0; i < MAX_TREE_VALUES - 1; i++)
	{
		rb_delete_max_element(tree_2);
		n = get_rb_node_value_from_node(rb_max_value(tree_2));
		assert(sorted_values[MAX_TREE_VALUES - i - 2] == n);

		n = get_rb_node_value_from_node(rb_min_value(tree_2));

		assert(n == sorted_values[0]);
	}

	rb_delete_tree(tree_1);
	rb_delete_tree(tree_2);

	free(values);
	free(sorted_values);

	printf("Test red black tree [OK] \n");

	return 0;
}