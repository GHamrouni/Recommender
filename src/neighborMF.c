#include "neighborMF.h"
#include "utils.h"
#include "training_set.h"
#include "float_tester.h"
#include "rlog.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include "items_rated_by_user.h"
/************************************************************************/
/*                          Learning algorithms                         */
/************************************************************************/

static void
compute_factors_neighbor (	size_t user_index,
                            size_t item_index,
                            learned_factors_t* lfactors,
                            double predicted_error,
                            const model_parameters_t* params, double* sum, double rating)
{
	size_t i = 0;

	double* item_factors;
	double* user_factors;

	assert (is_valid (predicted_error) );

	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	lfactors->user_bias[user_index] = lfactors->user_bias[user_index] +
	                                  params->step_bias * (predicted_error - params->lambda_bias * lfactors->user_bias[user_index]);

	lfactors->item_bias[item_index] = lfactors->item_bias[item_index] +
	                                  params->step_bias * (predicted_error - params->lambda_bias * lfactors->item_bias[item_index]);

	assert (is_valid (lfactors->user_bias[user_index]) );
	assert (is_valid (lfactors->item_bias[item_index]) );

	for (i = 0; i < params->dimensionality; i++)
	{
		sum[user_index] = sum[user_index] + predicted_error * item_factors[item_index];

		user_factors[i] = user_factors[i] +
		                  params->step * (predicted_error * item_factors[i] - params->lambda * user_factors[i]);

		item_factors[i] = item_factors[i] +
		                  params->step * (predicted_error * user_factors[i] - params->lambda * item_factors[i]);

		lfactors->x[item_index][i] += params->step *
		                              (rating - lfactors->item_bias[item_index] - lfactors->user_bias[user_index] - lfactors->ratings_average) * sum[item_index]
		                              - params->lambda * lfactors->x[item_index][i];

		lfactors->y[item_index][i] += params->step * sum[item_index]
		                              - params->lambda * lfactors->x[item_index][i];

		assert (is_valid (item_factors[i]) );
		assert (is_valid (user_factors[i]) );
	}
}

static void
calculate_average_ratings (struct training_set* tset, learned_factors_t* lfactors, model_parameters_t params)
{
	size_t i;
	float average_rating = (float) tset->ratings_sum / ( (float) tset->training_set_size);
	double* nb_ratings_per_user = NULL;

	if (!tset->ratings_matrix)
	{
		return;
	}

	nb_ratings_per_user = malloc (sizeof (double) * params.users_number);

	if (!nb_ratings_per_user)
	{
		return;
	}

	memset (nb_ratings_per_user, 0, sizeof (double) * params.users_number);

	lfactors->ratings_average = average_rating;

	/*for (i = 0; i < params.items_number; i++)
	{
		lfactors->item_bias[i] = item_ratings_average (i, tset) - average_rating;
	}

	for (i = 0; i < tset->ratings_matrix->nonzero_entries_nb; i++)
	{
		nb_ratings_per_user[tset->ratings_matrix->column_index[i]] += 1;
	}

	for (i = 0; i < tset->ratings_matrix->nonzero_entries_nb; i++)
	{
		lfactors->user_bias[tset->ratings_matrix->column_index[i]] +=
		    tset->ratings_matrix->values[i];
	}

	for (i = 0; i < tset->users_number; i++)
	{
		if (nb_ratings_per_user[i] > 0)
			lfactors->user_bias[i] =
			    lfactors->user_bias[i] / (nb_ratings_per_user[i]) - average_rating;
	}*/

	free (nb_ratings_per_user);
}



double
estimate_error_mf_neighbor (double r_iu, size_t user_index, size_t item_index, learned_factors_t* lfactors,training_set_t * tset)
{
	double estim = 0.0;
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
	estim=calculate_rating(user_index,item_index,lfactors,tset);

	/*for (i = 0; i < lfactors->dimensionality; i++)
	{
		sum += user_factors[i] * item_factors[i];
	}

	assert (is_valid (sum) );*/

	return r_iu - ( estim);
}

/*
 * estimate_rating_:  Return the approximates user's rating of an item based on
 *                                some learned factors.
 */
double
estimate_rating_ (size_t user_index, size_t item_index, learned_factors_t* lfactors)
{
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
		sum += user_factors[i] * lfactors->x[item_index][i] + user_factors[i] * lfactors->y[item_index][i];
	}
	return  bias;
}

//struct learned_factors*
//learn_mf_neighbor (struct training_set* tset, struct model_parameters params)
//{
//	int iter, u, j, dim, sum, i;
//	double estim_rating, estim_error;
//	struct learned_factors* lfactors = init_learned_factors (params);
//	if (!lfactors)
//	{
//		return NULL;
//	}
//
//	lfactors->dimensionality = params.dimensionality;
//	lfactors->items_number = params.items_number;
//	lfactors->users_number = params.users_number;
//	calculate_average_ratings (tset, lfactors, params);
//	//for (dim = 0; dim < params.dimensionality; dim++)
//	//{
//	dim=0;
//		for (iter = 0; iter < params.iteration_number; iter++)
//		{
//			for (u = 0; u < params.users_number; u++)
//			{
//				lfactors->user_factor_vectors[u][dim] = 0;
//				for (j = 0; j < tset->training_set_size; j++)
//				{
//					if (tset->ratings->entries[j].column_j == u)
//					{
//						lfactors->user_factor_vectors[u][dim] += tset->ratings->entries[j].value -
//						        lfactors->user_bias[u] - lfactors->item_bias[tset->ratings->entries[j].row_i] -
//						        lfactors->ratings_average + lfactors->y[tset->ratings->entries[j].row_i][dim];
//					}
//				}
//				if (get_number_in_column (u, tset->ratings_matrix) )
//				{
//					lfactors->user_factor_vectors[u][dim] /= sqrtf (get_number_in_column (u, tset->ratings_matrix) );
//				}
//				sum = 0;
//				for (i = 0; i < tset->training_set_size; i++)
//				{
//					if (tset->ratings->entries[i].column_j == u)
//					{
//						estim_rating = lfactors->ratings_average + lfactors->user_bias[u] +
//						               lfactors->item_bias[tset->ratings->entries[i].row_i] +
//						               dot_product (lfactors->item_factor_vectors[tset->ratings->entries[i].row_i],
//						                            lfactors->user_factor_vectors[u], lfactors->dimensionality);
//						estim_error = tset->ratings->entries[i].value - estim_rating;
//						sum += estim_error * lfactors->item_factor_vectors[tset->ratings->entries[i].row_i][dim];
//						lfactors->item_factor_vectors[tset->ratings->entries[i].row_i][dim] += params.step *
//						        (estim_error * lfactors->user_factor_vectors[u][dim] - params.lambda * lfactors->item_factor_vectors[tset->ratings->entries[i].row_i][dim]);
//						lfactors->user_bias[u] += params.step * (estim_error - params.lambda * lfactors->user_bias[u]);
//						
//						lfactors->user_bias[tset->ratings->entries[i].row_i] += params.step * (estim_error - params.lambda * lfactors->user_bias[tset->ratings->entries[i].row_i]);
//						
//					}
//				}
//				for (i = 0; i < tset->training_set_size; i++)
//				{
//					if (tset->ratings->entries[i].column_j == u)
//					{
//						lfactors->x[tset->ratings->entries[i].row_i][dim] += params.step * ( (1 / sqrtf (get_number_in_column (u, tset->ratings_matrix) ) )
//						        * (tset->ratings->entries[i].value - lfactors->ratings_average - lfactors->user_bias[u] -
//						           lfactors->item_bias[tset->ratings->entries[i].row_i]) * sum - params.lambda * lfactors->x[tset->ratings->entries[i].row_i][dim]);
//
//						lfactors->y[tset->ratings->entries[i].row_i][dim] += params.step * ( (1 / sqrtf (get_number_in_column (u, tset->ratings_matrix) ) )
//						        * sum - params.lambda * lfactors->y[tset->ratings->entries[i].row_i][dim]);
//					}
//				}
//			}
//		//}
//	}
//		return lfactors;
//}



double
calculate_rating (size_t user_index, size_t item_index, learned_factors_t* lfactors,training_set_t * tset)
{
	double sum = 0;
	size_t j,ii,ri;

	double* item_factors;
	double* user_factors;

	double item_bias;
	double user_bias;

	double bias;
	double * sumy;
	double term2;
	int i;
	size_t number=0;
	sumy=malloc(sizeof(double)*lfactors->dimensionality);
	memset(sumy,0,sizeof(double)*lfactors->dimensionality);
	
	assert (item_index < lfactors->items_number);
	assert (user_index < lfactors->users_number);

	item_bias = lfactors->item_bias[item_index];
	user_bias = lfactors->user_bias[user_index];

	bias = lfactors->ratings_average + item_bias + user_bias;

	
	item_factors = lfactors->item_factor_vectors[item_index];
	user_factors = lfactors->user_factor_vectors[user_index];

	for(ri=0 ;ri<lfactors->R[user_index].items_number;ri++)
	{
			ii=tset->ratings->entries[ri].row_i;
			add_vector(sumy,lfactors->y[ii],lfactors->dimensionality);
			
			number++;
		
	}
	scalar_product(sumy,(double)1/number,lfactors->dimensionality);
	number=0;
	for(ri=0 ;ri<lfactors->R_K[item_index].items_number;ri++)
			{
				if(tset->ratings->entries[lfactors->R_K[item_index].ratings_order[ri]].column_j==user_index)
				{
				ii=tset->ratings->entries[ri].row_i;
				sum += (tset->ratings->entries[lfactors->R_K[item_index].ratings_order[ri]].value - (lfactors->item_bias[ii]+lfactors->user_bias[user_index]
				+lfactors->ratings_average))*lfactors->x[item_index][ii];
				number++;
				}
			}
	if(sum!=0)
	{
	sum/=sqrtf(number);
	}
	add_vector(sumy,lfactors->user_factor_vectors[user_index], lfactors->dimensionality);
	term2 = dot_product(lfactors->item_factor_vectors[item_index],sumy,lfactors->dimensionality);
	free(sumy);
	if(term2 + bias + sum<1)
	{
		return 1;
	}
	if(term2 + bias + sum>5)
	{
		return 5;
	}
	assert(is_valid(term2+bias+sum));
	return term2+bias+sum;
}




struct learned_factors*
learn_mf_neighbor (struct training_set* tset, struct model_parameters params)
{
	int iter, u, j, dim, sum, i, k, r,ri,ii,number;
	double estim_rating, estim_error,r_iu,e_iu,sumy;
	struct learned_factors* lfactors = init_learned_factors (params);
	if (!lfactors)
	{
		return NULL;
	}

	lfactors->dimensionality = params.dimensionality;
	lfactors->items_number = params.items_number;
	lfactors->users_number = params.users_number;
	calculate_average_ratings (tset, lfactors, params);
	lfactors->R=init_items_rated_by_user(tset);
	lfactors->R_K=get_nearest_neighbors(tset);
	for (k = 0; k < params.iteration_number; k++)
	{
		double max_error = 0;

		for (r = 0; r < params.training_set_size; r++)
		{
			r_iu = tset->ratings->entries[r].value;

			i = tset->ratings->entries[r].row_i;
			u = tset->ratings->entries[r].column_j;
			e_iu = estimate_error_mf_neighbor(r_iu, u, i, lfactors,tset);
			//assert (is_valid(e_iu));

			lfactors->user_bias[u]+=params.step*(e_iu-params.lambda*lfactors->user_bias[u]);
			lfactors->item_bias[i]+=params.step*(e_iu-params.lambda*lfactors->item_bias[i]);
			for(dim=0;dim<lfactors->dimensionality;dim++)
			{
			sumy=0;
			number=0;
			for(ri=0 ;ri<lfactors->R[u].items_number;ri++)
			{
				
				ii=tset->ratings->entries[lfactors->R[u].ratings_order[ri]].row_i;
					sumy += lfactors->y[ii][dim];
					number++;
				
			}
			
			lfactors->item_factor_vectors[i][dim] += params.step*(e_iu *(lfactors->user_factor_vectors[u][dim]+sumy/sqrtf(number)) -params.lambda*lfactors->item_factor_vectors[i][dim]);
			lfactors->user_factor_vectors[u][dim] += params.step*(e_iu *lfactors->item_factor_vectors[i][dim] -params.lambda*lfactors->user_factor_vectors[u][dim]);
			for(ri=0 ;ri<lfactors->R[u].items_number;ri++)
			{
				
				ii=tset->ratings->entries[lfactors->R[u].ratings_order[ri]].row_i;
					lfactors->y[ii][dim] += params.step *(e_iu*lfactors->item_factor_vectors[i][dim]/sqrtf(number) - params.lambda * lfactors->y[ii][dim]);
					number++;
				
			}
			for(ri=0 ;ri<lfactors->R_K[i].items_number;ri++)
			{
				if(tset->ratings->entries[lfactors->R_K[i].ratings_order[ri]].column_j==u)
				{
					ii=tset->ratings->entries[lfactors->R_K[i].ratings_order[ri]].row_i;
					lfactors->x[i][ii] += params.step *(e_iu*(tset->ratings->entries[lfactors->R_K[i].ratings_order[ri]].value-lfactors->user_bias[u]-lfactors->item_bias[ii]-lfactors->ratings_average)/sqrtf(number) - params.lambda * lfactors->x[i][ii]);
				}
			}
			}

		}

		/*if (max_error < step)
			break;*/
	}
		return lfactors;
}