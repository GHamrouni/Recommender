#include "Recommender.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include "matrix_factorization.h"
#include "matrix_factorization_bias.h"

int main(void) {

	learned_factors_t* learned;
	training_set_t* tset;

	learning_model_t model;

	//Model configuration
	//Setup model parameters
	model_parameters_t params;
	
	params.dimensionality = 35;
	params.iteration_number = 1250;
	params.items_number = 2;
	params.users_number = 2;
	params.lambda = 0.005;
	params.training_set_size = 4;
	params.step = 0.005;
	
	//Use the basic matrix factorization model
	model.learning_algorithm = learn_mf_bias;
	model.rating_estimator   = estimate_rating_mf_bias;

// 	model.learning_algorithm = learn_basic_mf;
// 	model.rating_estimator   = estimate_rating_basic_mf;

	//Learning
	//Initialize a training set
	tset = init_training_set(params);

	set_known_rating(0, 0, 1, tset);
	set_known_rating(0, 1, 5, tset);
	set_known_rating(1, 1, 50, tset);
    set_known_rating(1, 0, 2, tset);

	learned = learn(tset, params, model);

	//Rating estimation
	printf("users [0] item [0], rating = %f \n", estimate_rating_from_factors(0, 0, learned, model));
	printf("users [0] item [1], rating = %f \n", estimate_rating_from_factors(0, 1, learned, model));
	printf("users [1] item [1], rating = %f \n", estimate_rating_from_factors(1, 1, learned, model));
	printf("users [1] item [0], rating = %f \n", estimate_rating_from_factors(1, 0, learned, model));

	free_learned_factors(learned);
	free_training_set(tset);

	system("pause");

	return 0;
}