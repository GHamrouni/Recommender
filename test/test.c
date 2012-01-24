#include "../recommender.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include "../matrix_factorization.h"
#include "../matrix_factorization_bias.h"
#include "../sparse_matrix.h"

void print_recommended_items(rb_node_t* node, int depth)
{
	int i = 0;

	if (node == NULL)
		return;

	for (i = 0; i < depth; i++)
		printf("->");

	printf("%f \n", ((recommended_item_t*) node->value)->rating);

	depth++;

	print_recommended_items(node->left_node, depth);
	print_recommended_items(node->right_node, depth);
}

int main(void) {

	learned_factors_t* learned;
	training_set_t* tset;
	recommended_items_t* r_items = NULL;

	learning_model_t model = {0};

	//Model configuration
	//Setup model parameters
	model_parameters_t params = { 0 };
	
	params.items_number = 3;
	params.users_number = 2;
	params.training_set_size = 5;

	params.dimensionality = 40;
	params.iteration_number = 60;

	params.lambda = 0.055;
	params.step = 0.0095;

	params.lambda_bias = 0.02;
	params.step_bias = 0.001;
	
	//Use the basic matrix factorization model
	model.learning_algorithm = learn_mf_bias;
	model.rating_estimator   = estimate_rating_mf_bias;
	model.parameters		 = params;

	//Learning
	//Initialize a training set
	tset = init_training_set(params);

	set_known_rating(0, 0, 1, tset);
	set_known_rating(0, 1, 0, tset);
	set_known_rating(0, 2, 6, tset);

	set_known_rating(1, 1, 0, tset);
	set_known_rating(1, 0, 2, tset);

	compile_training_set(tset);

	learned = learn(tset, model);

	//Rating estimation
	printf("users [0] item [0], rating = %f \n", estimate_rating_from_factors(0, 0, learned, model));
	printf("users [0] item [1], rating = %f \n", estimate_rating_from_factors(0, 1, learned, model));
	printf("users [0] item [1], rating = %f \n", estimate_rating_from_factors(0, 2, learned, model));
	printf("users [1] item [1], rating = %f \n", estimate_rating_from_factors(1, 1, learned, model));
	printf("users [1] item [0], rating = %f \n", estimate_rating_from_factors(1, 0, learned, model));

	r_items = recommend_items(0, 2, learned, tset, model);

	print_recommended_items(r_items->items->head, 0);

	free_recommended_items(r_items);
	free_learned_factors(learned);
	free_training_set(tset);

	return 0;
}

