#include "items_rated_by_user.h"
#include "sparse_matrix_hash.h"
#include <stdlib.h>
#include "rlog.h"
#define ABS(a) ((a)<0 ? -(a) : (a))

items_rated_by_user_t *
init_items_rated_by_user (training_set_t* tset)
{
	int i, n;
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
get_nearest_neighbors (training_set_t* tset)
{
	int i, n, j, oldj;
	items_rated_by_user_t* R = malloc (sizeof (items_rated_by_user_t) * tset->items_number);
	/*Projection*/
	int dimension = tset->users_number;
	int seed = 4578;
	int bin_width = 35;
	int proj_family_size = 3;
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
			
			for (j = 0; j < tset->ratings->size; j++)
			{
				if (abs (n - lsh_row (pfamily, tset->ratings_matrix, tset->ratings->entries[j].row_i) ) == 0)
				{
					R[i].ratings_order[R[i].items_number++] = tset->ratings->entries[j].row_i;
				}
			}
		}
	}
	return R;

}


//items_rated_by_user_t *
//	get_nearest_neighbors ( training_set_t* tset, items_rated_by_user_t ratings_by_user, size_t item_index)
//{
//	items_rated_by_user_t* R_u_i= malloc(sizeof(items_rated_by_user_t));
//	int i,n;
//	/*Projection*/
//	int dimension = tset->users_number;
//	int seed = 4578;
//	int bin_width = 35;
//	int proj_family_size = 3;
//	projection_family_t* pfamily = NULL;
//	/* end projection*/
//	/* Initialize a family (size = proj_family_size) */
//	pfamily =
//	    init_random_projections (dimension, seed, bin_width, proj_family_size);
//	R_u_i->ratings_order=malloc(ratings_by_user.items_number*sizeof(size_t));
//	R_u_i->items_number=0;
//	n=lsh_row (pfamily, tset->ratings_matrix, item_index);
//	for(i=0;i<ratings_by_user.items_number;i++)
//	{
//		if(ABS(n-lsh_row (pfamily, tset->ratings_matrix, tset->ratings->entries[ratings_by_user.ratings_order[i]].row_i))<10)
//		{
//			R_u_i->ratings_order[R_u_i->items_number++] = ratings_by_user.ratings_order[i];
//			if(tset->ratings->entries[ratings_by_user.ratings_order[i]].row_i != item_index)
//			{
//				RLog("");
//			}
//		}
//	}
//	free_projection_family(pfamily);
//	return R_u_i;
//}