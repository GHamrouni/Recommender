#include "../recommender.h"

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

	unsigned int i, j, k;

	double sum = 0;

	//Model configuration
	//Setup model parameters
	model_parameters_t params;

	params->dimensionality = 100;
	params->iteration_number = 1;
	params->items_number = 800;
	params->users_number = 800;
	params->lambda = 0.005;
	params->training_set_size = 600 * 100;
	params->step = 0.005;

	//Use the basic matrix factorization model
	model->learning_algorithm = learn_mf_bias;
	model->rating_estimator   = estimate_rating_mf_bias;
	model->parameters		 = params;

	//Learning
	//Initialize a training set
	tset = init_training_set(&params);

	for (i = 0; i < 600; i++)
	{
		for (j = 0; j < 100; j++)
		{
			sum = 0;
			
			//for (k = 0; k < params->dimensionality; k++)
			//	sum += (((i+k/10) % 100) / 250.0) * (((j+k/10) % 100) / 250.0);

			set_known_rating(i, j, i / 100.0, tset);
		}
	}

	compile_training_set(tset);

	learned = learn(tset, model);

	for (k = 0; k < 100; k++)
	{
		update_learned_factors_mf_bias(learned, tset, &params);
	}

	sum = 0;

	for (i = 0; i < 600; i++)
	{
		for (j = 0; j < 100; j++)
		{
			sum += fabs(get_element(i, j, tset->ratings_matrix) -  estimate_rating_from_factors(i, j, learned, &model));
		}
	}

	printf("Total error {%f} \n", sum/60000.0);

	//Rating estimation
	printf("users [32] item [11], rating = %f \n", estimate_rating_from_factors(32, 11, learned, &model));
	printf("users [190] item [3], rating = %f \n", estimate_rating_from_factors(190, 3, learned, &model));
	printf("users [1] item [1], rating = %f \n",  estimate_rating_from_factors(1, 1, learned, &model));
	printf("users [1] item [0], rating = %f \n", estimate_rating_from_factors(1, 0, learned, &model));

	free_learned_factors(learned);
	free_training_set(tset);

	return 0;
}