#include "nearest_neighbors.h"
#include <stdlib.h>
#include "utils.h"
#include "rlog.h"
double ** 
	calculate_simularities_matrix (training_set_t* tset)
{
	size_t i, j, k;
	double ** simularities_matrix = (double**) malloc (tset->items_number * sizeof (double*) );
	double *item_i_ratings, *item_j_ratings;
	item_i_ratings = malloc (tset->users_number * sizeof (double) );
	item_j_ratings = malloc (tset->users_number * sizeof (double) );
	for (i = 0; i < tset->items_number; i++)
	{
		simularities_matrix[i] = malloc (i * sizeof (double) );
		
			item_i_ratings = get_row(i,tset->ratings_matrix);
		
		for (j = 0; j < i; j++)
		{
			item_j_ratings = get_row(j,tset->ratings_matrix);
			for (k = 0; k < tset->users_number; k++)
			{
				if (item_j_ratings[k] == 0)
				{
					item_i_ratings[k] = 0;
				}
				else if (item_i_ratings[k] == 0)
				{
					item_j_ratings[k] = 0;
				}
			}
			 simularities_matrix[i][j] = pearson_correlation_coefficient (item_i_ratings, item_j_ratings, tset->users_number);
			 free(item_j_ratings);
		}
	}

	return simularities_matrix;
}

double 
	estimate_rating(double** simularities_matrix,training_set_t* tset,size_t user,size_t item)
{	
	size_t  i;
	float rating;
	double sum_numerator=0;
	double sum_denum=0;
	for(i=0;i<tset->items_number;i++)
	{
		rating= get_element(i,user,tset->ratings_matrix);
		if(rating!=0)
		{
			sum_numerator+=simularities_matrix[i][item]*rating;
			sum_denum+=simularities_matrix[i][item];
		}
	}
	return (sum_numerator/sum_denum);
}