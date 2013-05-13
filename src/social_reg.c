#include "matrix_factorization_bias.h"
#include "utils.h"
#include "training_set.h"
#include "float_tester.h"
#include "rlog.h"
#include "learning_algorithm.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
/************************************************************************/
/*                          Learning algorithms                         */
/************************************************************************/
#define ABS(a) ((a)<0 ? -(a) : (a))

static void
compute_factors_social (	size_t user_index,
                            size_t item_index,
                            learned_factors_t* lfactors,
                            double predicted_error,
                            const model_parameters_t* params)
{
	size_t i = 0;

	double* item_factors;
	double* user_factors;

	assert (is_valid (predicted_error) );

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	lfactors->user_bias[user_index] = lfactors->user_bias[user_index] +
	                                  params->step_bias * (predicted_error );

	lfactors->item_bias[item_index] = lfactors->item_bias[item_index] +
	                                  params->step_bias * (predicted_error);

	assert (is_valid (lfactors->user_bias[user_index]) );
	assert (is_valid (lfactors->item_bias[item_index]) );

	for (i = 0; i < params->dimensionality; i++)
	{
		double delta_i =  predicted_error * user_factors[i] ;
		double delta_u =  predicted_error * item_factors[i] ;
		item_factors[i] = item_factors[i] +
		                  params->step * delta_i;

		user_factors[i] = user_factors[i] +
		                  params->step * delta_u;

		assert (is_valid (item_factors[i]) );
		assert (is_valid (user_factors[i]) );
	}
}

static void
calculate_average_ratings (struct training_set* tset, learned_factors_t* lfactors)
{
	double average_rating = (double) tset->ratings_sum / ( (double) tset->training_set_size);
	average_rating = (average_rating - 1) / 4.0;
	lfactors->ratings_average = log ( (double) average_rating / (1 - average_rating) );

}

/*
 * Update the learned factors
 */
void
update_learned_factors_social (struct learned_factors* lfactors, struct training_set* tset, sparse_matrix_t* social_matrix, struct model_parameters params)
{
	size_t r, k, i, u, n, vv;

	double r_iu = 0;

	double e_iu = 0;
	double prediction;
	double sig_score;
	double score;
	double *sum = malloc (sizeof (double) * params.dimensionality);
	double *friend_sum = malloc (sizeof (double) * params.dimensionality);
	double* user_bias_copy;
	double** user_factors_copy;
	lfactors->dimensionality = params.dimensionality;
	lfactors->items_number = params.items_number;
	lfactors->users_number = params.users_number;

	r = k = u = i = 0;

	for (k = 0; k < params.iteration_number; k++)
	{
		user_bias_copy = malloc (sizeof (double) * params.users_number);
		user_factors_copy = malloc (sizeof (double*) *params.users_number);
		memcpy (user_bias_copy, lfactors->user_bias, sizeof (double) *params.users_number);
		for (r = 0; r < params.users_number; r++)
		{
			user_factors_copy[r] = malloc (sizeof (double) * params.dimensionality);
			memcpy (user_factors_copy[r], lfactors->user_factor_vectors[r], params.dimensionality * sizeof (double) );
		}

		for (r = 0; r < tset->training_set_size; r++)
		{
			r_iu = tset->ratings->entries[r].value;

			i = tset->ratings->entries[r].row_i;
			u = tset->ratings->entries[r].column_j;

			score = lfactors->ratings_average + lfactors->user_bias[u] + lfactors->item_bias[i] +
			        dot_product (lfactors->user_factor_vectors[u], lfactors->item_factor_vectors[i], lfactors->dimensionality);
			sig_score = 1 / (1 + exp (-score) );
			prediction = 1 + sig_score * 4;

			e_iu = r_iu - prediction;
			if (e_iu)
			{
				compute_factors_social (u, i, lfactors, e_iu , &params);
			}

		}
		for (u = 0; u < params.users_number; u++)
			lfactors->user_bias[u] += params.step_bias * user_bias_copy[u] * params.lambda_bias;
		for (i = 0; i < params.items_number; i++)
			lfactors->item_bias[i] += params.step_bias * lfactors->item_bias[i] * params.lambda_bias;
			
			for (u = 0; u < params.users_number; u++)
				for (r = 0; r < params.dimensionality; r++)
					lfactors->user_factor_vectors[u][r] +=params.step * user_factors_copy[u][r] * params.lambda;

			for (i = 0; i < params.items_number; i++)
				for (r = 0; r < params.dimensionality; r++)
					lfactors->item_factor_vectors[i][r] +=params.step * lfactors->item_factor_vectors[i][r] * params.lambda;
		for (u = 0; u < params.users_number; u++)
		{
			coo_matrix_t* user_relations = get_row_in_coo (social_matrix, u);

			size_t v;
			double bias_diff = 0;
			memset (sum, 0, params.dimensionality * sizeof (double) );

			for (v = 0 ; v < user_relations->current_size; v++)
			{
				for (r = 0; r < params.dimensionality; r++)
				{
					sum[r] += user_factors_copy[user_relations->entries[v].column_j][r] / user_relations->current_size;
				}
				bias_diff += user_bias_copy[user_relations->entries[v].column_j] / user_relations->current_size;
			}



			for (r = 0; r < params.dimensionality; r++)
			{
				lfactors->user_factor_vectors[u][r] += params.betha * params.step* (user_factors_copy[u][r] - sum[r]);
			}
			lfactors->user_bias[u] += params.betha  * params.step_bias * (user_bias_copy[u] - bias_diff);


			bias_diff=0;
			memset(sum,0,params.dimensionality * sizeof (double));
			for (v = 0 ; v < user_relations->current_size; v++)
			{
				size_t friend_id = user_relations->entries[v].column_j;
				coo_matrix_t* friend_relations = get_row_in_coo (social_matrix, friend_id);
				double friend_bias_diff = 0;
				memset (friend_sum, 0, params.dimensionality * sizeof (double) );

				for (vv = 0 ; vv < friend_relations->current_size; vv++)
				{
					for (r = 0; r < params.dimensionality; r++)
					{
						friend_sum[r] += user_factors_copy[friend_relations->entries[vv].column_j][r] / friend_relations->current_size;
					}
					friend_bias_diff += user_bias_copy[friend_relations->entries[vv].column_j] / friend_relations->current_size;
				}
				for (r = 0; r < params.dimensionality; r++)
				{
					sum[r] += (user_factors_copy[friend_id][r] - friend_sum[r]) / user_relations->current_size;
				}
				bias_diff += (user_bias_copy[friend_id] - friend_bias_diff) / user_relations->current_size;

				free_coo_matrix (friend_relations);
			}
			for (r = 0; r < params.dimensionality; r++)
			{
				lfactors->user_factor_vectors[u][r] -= params.betha * params.step * ( sum[r]);
			}
			lfactors->user_bias[u] -=  params.betha  * params.step_bias * (bias_diff);

			
			free_coo_matrix (user_relations);
		}


		free (user_bias_copy);
		for (n = 0; n < params.users_number; n++)
		{
			free (user_factors_copy[n]);
		}
		free (user_factors_copy);
	}




	free (sum);
}

/*
 * Stochastic gradient descent
 */
struct learned_factors*
learn_social (learning_algorithm_params_t learning_param)
{
	struct training_set* tset = learning_param.tset;
	struct model_parameters params = learning_param.params;
	struct learned_factors* lfactors = init_learned_factors (params);
	sparse_matrix_t * social_matrix = learning_param.social_matrix;

	if (!lfactors)
	{
		return NULL;
	}

	lfactors->dimensionality = params.dimensionality;
	lfactors->items_number = params.items_number;
	lfactors->users_number = params.users_number;

	if (tset->ratings_matrix)
	{
		calculate_average_ratings (tset, lfactors);

		free_sparse_matrix (tset->ratings_matrix);
		tset->ratings_matrix = NULL;
	}

	update_learned_factors_social (lfactors, tset, social_matrix, params);

	return lfactors;
}


/*
 * estimate_rating_from_factors:  Return the approximates user's rating of an item based on
 *                                some learned factors.
 */
double
estimate_rating_social (rating_estimator_parameters_t* estim_param)
{
	learned_factors_t* lfactors = estim_param->lfactors;
	size_t item_index = estim_param->item_index;
	size_t user_index = estim_param->user_index;

	double sum = 0;
	size_t i;

	double* item_factors;
	double* user_factors;

	double item_bias;
	double user_bias;

	double bias;

	assert (item_index < lfactors->items_number);
	assert (user_index < lfactors->users_number);

	item_bias = lfactors->item_bias[item_index];
	user_bias = lfactors->user_bias[user_index];

	bias = lfactors->ratings_average + item_bias + user_bias;

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	for (i = 0; i < lfactors->dimensionality; i++)
	{
		sum += user_factors[i] * item_factors[i] ;
	}
	return 1 + (1 / (1 + exp (-sum - bias) ) ) * 4;
}
