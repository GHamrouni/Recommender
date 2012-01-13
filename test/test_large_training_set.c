#include "../Recommender.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include "../matrix_factorization.h"
#include "../matrix_factorization_bias.h"
#include "../sparse_matrix.h"

int main(void) {

	learned_factors_t* learned;
	training_set_t* tset;

	learning_model_t model = {0};

	unsigned int i, j;

	//Model configuration
	//Setup model parameters
	model_parameters_t params;

	params.dimensionality = 1500;
	params.iteration_number = 35;
	params.items_number = 1600;
	params.users_number = 800;
	params.lambda = 0.005;
	params.training_set_size = 600 * 100;
	params.step = 0.005;

	//Use the basic matrix factorization model
	model.learning_algorithm = learn_mf_bias;
	model.rating_estimator   = estimate_rating_mf_bias;

	//Learning
	//Initialize a training set
	tset = init_training_set(params);

	for (i = 0; i < 600; i++)
	{
		for (j = 0; j < 100; j++)
			set_known_rating(i, j, 10, tset);
	}

	compile_training_set(tset);

	learned = learn(tset, params, model);

	//Rating estimation
	printf("users [0] item [0], rating = %f \n", estimate_rating_from_factors(0, 0, learned, model));
	printf("users [300] item [1], rating = %f \n", estimate_rating_from_factors(300, 0, learned, model));
	printf("users [1] item [1], rating = %f \n", estimate_rating_from_factors(1, 1, learned, model));
	printf("users [1] item [0], rating = %f \n", estimate_rating_from_factors(1, 0, learned, model));

	free_learned_factors(learned);
	free_training_set(tset);

	return 0;
}