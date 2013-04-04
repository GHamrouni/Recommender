#include "items_rated_by_user.h"
#include "sparse_matrix_hash.h"
#include <stdlib.h>
#include "rlog.h"
#define ABS(a) ((a)<0 ? -(a) : (a))

items_rated_by_user_t *
init_items_rated_by_user (training_set_t* tset)
{
	int  n;
	size_t i;
	items_rated_by_user_t* R = malloc (sizeof (items_rated_by_user_t) * tset->users_number);
	for (i = 0; i < tset->users_number; i++)
	{
		R[i].items_number = 0;
		R[i].ratings_order = malloc (sizeof (size_t) * tset->items_number);
	}
	for (i = 0; i < tset->ratings->size; i++)
	{
		n = R[tset->ratings->entries[i].column_j].items_number++;
		R[tset->ratings->entries[i].column_j].ratings_order[n] = i;
	}
	return R;
}

items_rated_by_user_t *
get_nearest_neighbors (training_set_t* tset,int bin_width,int proj_family_size,int seed)
{
	int n;
	size_t i, j, oldj;
	items_rated_by_user_t* R = malloc (sizeof (items_rated_by_user_t) * tset->items_number);
	/*Projection*/
	int dimension = tset->users_number;
	projection_family_t* pfamily = NULL;
	/* end projection*/
	int add = 0;
	/* Initialize a family (size = proj_family_size) */
	pfamily =
	    init_random_projections (dimension, seed, bin_width, proj_family_size);
	for (i = 0; i < tset->items_number; i++)
	{
		R[i].items_number = 0;
		R[i].ratings_order = malloc (sizeof (size_t) * tset->items_number);
	}
	for (i = 0; i < tset->items_number; i++)
	{
		n = lsh_row (pfamily, tset->ratings_matrix, i);
		if (n != 0)
		{
			oldj=i;

			for (j = 0; j < tset->ratings->size; j++)
			{
				if(tset->ratings->entries[j].row_i != oldj)
				{
				if ((tset->ratings->entries[j].row_i != i)&&(abs (n - lsh_row (pfamily, tset->ratings_matrix, tset->ratings->entries[j].row_i) ) == 0))
				{
					R[i].items_number++;
					add=1;
				}else add=0;
				oldj=tset->ratings->entries[j].row_i;
				}else if(add)
				{
					R[i].items_number++;
				}
			}
		}
	}
	for (i = 0; i < tset->items_number; i++)
	{
		R[i].ratings_order = malloc (sizeof (size_t) * R[i].items_number);
		R[i].items_number = 0;
	}
	for (i = 0; i < tset->items_number; i++)
	{
		n = lsh_row (pfamily, tset->ratings_matrix, i);
		if (n != 0)
		{
			oldj=i;

			for (j = 0; j < tset->ratings->size; j++)
			{
				if(tset->ratings->entries[j].row_i != oldj)
				{
				if ((tset->ratings->entries[j].row_i != i) && abs(n - lsh_row (pfamily, tset->ratings_matrix, tset->ratings->entries[j].row_i) ) == 0)
				{
					R[i].ratings_order[R[i].items_number++] = j;
					add=1;
				}else add=0;
				oldj=tset->ratings->entries[j].row_i;
				}else if(add)
				{
					R[i].ratings_order[R[i].items_number++] = j;
				}
			}
		}
	}
	return R;

}
