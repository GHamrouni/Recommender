#include "Recommender.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

int main(void) {

	learned_factors_t* learned;
	training_set_t* tset;

	//Model configuration
	//Setup model parameters
	model_parameters_t params;
	
	params.dimensionality = 80;
	params.iteration_number = 15;
	params.items_number = 2;
	params.users_number = 2;
	params.lambda = 0.002;
	params.training_set_size = 3;
	params.step = 0.005;
	
	//Learning
	//Initialize a training set
	tset = init_training_set(params);

	set_known_rating(0, 0, 100, tset);
	set_known_rating(0, 1, 69, tset);
	set_known_rating(1, 1, 30, tset);

	learned = learn(tset, params);

	//Rating estimation
	printf("users [0] item [0], estimated rating = %f \n", estimate_rating_from_factors(0, 0, learned));
	printf("users [0] item [1], estimated rating = %f \n", estimate_rating_from_factors(0, 1, learned));
	printf("users [1] item [1], estimated rating = %f \n", estimate_rating_from_factors(1, 1, learned));
	printf("users [1] item [0], estimated rating = %f \n", estimate_rating_from_factors(1, 0, learned));

	free_learned_factors(learned);
	free_training_set(tset);

	system("pause");

	return 0;
}