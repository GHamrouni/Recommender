#include "neighborMF.h"
#include "utils.h"
#include "training_set.h"
#include "float_tester.h"
#include "rlog.h"
#include <stdio.h>
#include <stdlib.h>
#include "learning_algorithm.h"
#include <memory.h>
#include <math.h>
#include <assert.h>
#include "items_rated_by_user.h"
#include "rating_estimator.h"
/************************************************************************/
/*                          Learning algorithms                         */
/************************************************************************/


static void
calculate_average_ratings (struct training_set* tset, learned_factors_t* lfactors)
{
	lfactors->ratings_average = (float) tset->ratings_sum / ( (float) tset->training_set_size);
}



double
estimate_error_mf_neighbor (double r_iu, rating_estimator_parameters_t* estim_param)
{
	learned_factors_t* lfactors = estim_param->lfactors;
	size_t item_index = estim_param->item_index;
	size_t user_index = estim_param->user_index;
	double estim = 0.0;


	assert (item_index < lfactors->items_number);
	assert (user_index < lfactors->users_number);


	estim = estimate_rating_mf_neighbor (estim_param);

	return r_iu - ( estim);
}

/*
 * estimate_rating_:  Return the approximates user's rating of an item based on
 *                                some learned factors.
 */
double
estimate_rating_mf_neighbor (rating_estimator_parameters_t* estim_param)
{
	learned_factors_t* lfactors = estim_param->lfactors;
	training_set_t * tset = estim_param->tset;
	size_t item_index = estim_param->item_index;
	size_t user_index = estim_param->user_index;
	double sum = 0;
	size_t  ii, ri;

	double item_bias;
	double user_bias;

	double bias;
	double * sumy;
	double term2;


	sumy = malloc (sizeof (double) * lfactors->dimensionality);
	memset (sumy, 0, sizeof (double) *lfactors->dimensionality);

	assert (item_index < lfactors->items_number);
	assert (user_index < lfactors->users_number);

	item_bias = lfactors->item_bias[item_index];
	user_bias = lfactors->user_bias[user_index];

	bias = lfactors->ratings_average + item_bias + user_bias;



	for (ri = 0 ; ri < lfactors->R[user_index].items_number; ri++)
	{
		ii = tset->ratings->entries[ri].row_i;
		add_vector (sumy, lfactors->y[ii], lfactors->dimensionality);
	}
	if (lfactors->R[user_index].items_number != 0)
	{
		scalar_product (sumy, (1 / (double) lfactors->R[user_index].items_number), lfactors->dimensionality);	
	}
	
	
	for (ri = 0 ; ri < lfactors->R_K[item_index].items_number; ri++)
	{
		if (tset->ratings->entries[lfactors->R_K[item_index].ratings_order[ri]].column_j == user_index)
		{
			ii = tset->ratings->entries[ri].row_i;
			sum += (tset->ratings->entries[lfactors->R_K[item_index].ratings_order[ri]].value - (lfactors->item_bias[ii] + lfactors->user_bias[user_index]
			        + lfactors->ratings_average) ) * lfactors->x[item_index][ii];
		}
	}
	if (lfactors->R[user_index].items_number != 0)
	{
		sum /= sqrtf (lfactors->R[user_index].items_number);
	}
	add_vector (sumy, lfactors->user_factor_vectors[user_index], lfactors->dimensionality);
	term2 = dot_product (lfactors->item_factor_vectors[item_index], lfactors->user_factor_vectors[user_index], lfactors->dimensionality);
	free (sumy);
	if (term2 + bias + sum < 1)
	{
		return 1;
	}
	if (term2 + bias + sum > 5)
	{
		return 5;
	}
	assert (is_valid (term2 + bias + sum) );
	return term2 + bias + sum;
}

void update_factors_mf_neighbor (size_t u, size_t i, double e_iu,learned_factors_t* lfactors,training_set_t * tset, struct model_parameters * params)
{
	size_t dim, ri, ii;
	double sumy;
	
	lfactors->user_bias[u] += params->step_bias * (e_iu - params->lambda_bias * lfactors->user_bias[u]);
	lfactors->item_bias[i] += params->step_bias * (e_iu - params->lambda_bias * lfactors->item_bias[i]);
	for (dim = 0; dim < lfactors->dimensionality; dim++)
	{
		sumy = 0;
		for (ri = 0 ; ri < lfactors->R[u].items_number; ri++)
		{
			ii = tset->ratings->entries[lfactors->R[u].ratings_order[ri]].row_i;
			sumy += lfactors->y[ii][dim];
		}

		lfactors->item_factor_vectors[i][dim] += params->step * (e_iu * lfactors->user_factor_vectors[u][dim]  - params->lambda * lfactors->item_factor_vectors[i][dim]);
		lfactors->user_factor_vectors[u][dim] += params->step * (e_iu * lfactors->item_factor_vectors[i][dim] - params->lambda * lfactors->user_factor_vectors[u][dim]);
		for (ri = 0 ; ri < lfactors->R[u].items_number; ri++)
		{
			ii = tset->ratings->entries[lfactors->R[u].ratings_order[ri]].row_i;
			lfactors->y[ii][dim] += params->step * (e_iu * lfactors->item_factor_vectors[i][dim] / sqrtf (ri+ lfactors->R[u].items_number) - params->lambda * lfactors->y[ii][dim]);
		}
		
		for (ri = 0 ; ri < lfactors->R_K[i].items_number; ri++)
		{
			if (tset->ratings->entries[lfactors->R_K[i].ratings_order[ri]].column_j == u)
			{
				ii = tset->ratings->entries[lfactors->R_K[i].ratings_order[ri]].row_i;
				lfactors->x[i][ii] += params->step * (e_iu * (tset->ratings->entries[lfactors->R_K[i].ratings_order[ri]].value - lfactors->user_bias[u] - lfactors->item_bias[ii] - lfactors->ratings_average) / sqrtf (lfactors->R[u].items_number) - params->lambda * lfactors->x[i][ii]);
			}
		}
	}

}


struct learned_factors*
learn_mf_neighbor (learning_algorithm_params_t* learning_param)
{
	struct training_set* tset = learning_param->tset;
	struct model_parameters params = learning_param->params;
	size_t k,r, u, i;
	double r_iu, e_iu;
	rating_estimator_parameters_t * rating_estim = malloc (sizeof (rating_estimator_parameters_t) );
	struct learned_factors* lfactors = init_learned_factors (&params);
	if (!lfactors)
	{
		return NULL;
	}
	rating_estim->tset = tset;
	rating_estim->lfactors = lfactors;
	lfactors->dimensionality = params.dimensionality;
	lfactors->items_number = params.items_number;
	lfactors->users_number = params.users_number;
	calculate_average_ratings (tset, lfactors);
	lfactors->R = init_items_rated_by_user (tset);
	lfactors->R_K = get_nearest_neighbors (tset, params.bin_width, params.proj_family_size, params.seed);
	for (k = 0; k < params.iteration_number; k++)
	{

		for (r = 0; r < tset->training_set_size; r++)
		{
			r_iu = tset->ratings->entries[r].value;

			i = tset->ratings->entries[r].row_i;
			u = tset->ratings->entries[r].column_j;
			rating_estim->user_index = u;
			rating_estim->item_index = i;
			e_iu = estimate_error_mf_neighbor (r_iu, rating_estim);
			update_factors_mf_neighbor(u,i,e_iu,lfactors,tset, &params);
		}
	}
	free (rating_estim);
	return lfactors;
}


void update_learning_with_training_set_neighborMF(training_set_t * old_tset,training_set_t* new_tset,learned_factors_t* lfactors,
		model_parameters_t * params)
{
	size_t r, k, i, u;

	double r_iu = 0;
	double e_iu = 0;
	rating_estimator_parameters_t * rating_estim = malloc (sizeof (rating_estimator_parameters_t) );
	rating_estim->tset = old_tset;
	rating_estim->lfactors = lfactors;
	add_training_set(old_tset,new_tset);
	lfactors->R = init_items_rated_by_user (old_tset);
	lfactors->R_K = get_nearest_neighbors (old_tset, params->bin_width, params->proj_family_size, params->seed);
	for (k = 0; k < params->iteration_number; k++)
	{

		for (r = 0; r < params->training_set_size; r++)
		{
			r_iu = old_tset->ratings->entries[r].value;
			i = old_tset->ratings->entries[r].row_i;
			u = old_tset->ratings->entries[r].column_j;
			rating_estim->user_index = u;
			rating_estim->item_index = i;
			e_iu = estimate_error_mf_neighbor (r_iu, rating_estim);
			update_factors_mf_neighbor(u,i,e_iu,lfactors,old_tset,params);
		}
	}
}
