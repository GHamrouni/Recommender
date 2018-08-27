#include "../src/recommender.h"
#include "../src/training_set.h"
#include "../src/model_parameters.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include <stddef.h>

#include "test_training_set_1.h"
#include "../src/nearest_neighbors.h"
#define USER_NUMBER_TEST_SIZE		4

#define ITEM_NUMBER_TEST_SIZE		4

#define TRAINING_SET_TEST_SIZE		(4 * 4)

int test_training_set_1(void)
{
	training_set_t* tset;
	size_t u,  i, k;
	double * a;
	/* Model configuration */
	/* Setup model parameters */
	model_parameters_t params = { 0 };

	printf("Test training set ... \n");

	params.items_number = ITEM_NUMBER_TEST_SIZE;
	params.users_number = USER_NUMBER_TEST_SIZE;
	params.training_set_size = TRAINING_SET_TEST_SIZE;

	/* Initialize a training set */
	tset = init_training_set(&params);

	srand ( 47 );

	for (u = 0; u < tset->users_number; u++)
	{
		for (i = 0; i < tset->items_number; i++)
		{
			int x=(rand() % 10);
			set_known_rating(u, i, (float) x, tset);
			printf("%d ",x);
		}
		printf("\n");
	}
	

	compile_training_set(tset);
	//a = get_row(1, tset->ratings_matrix);
	get_number_in_row(0, tset->ratings_matrix);
	get_number_in_column(0, tset->ratings_matrix);
	for (i = 0; i < TRAINING_SET_TEST_SIZE; i++)
	{
		float val;

		u = tset->ratings->entries[i].row_i;
		k = tset->ratings->entries[i].column_j;

		assert(element_exists(u, k, tset->ratings_matrix));

		val = get_element(u, k, tset->ratings_matrix);

		assert(val == tset->ratings->entries[i].value);
	}

	free_training_set(tset);
	
	printf("Test training set [OK] \n");

	return 0;
}