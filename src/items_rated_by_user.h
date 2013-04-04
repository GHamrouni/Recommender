#ifndef ITEMS_RATED_BY_USER
#define ITEMS_RATED_BY_USER
#include "training_set.h"


struct items_rated_by_user
{
	size_t * ratings_order;
	size_t items_number;
};
typedef struct items_rated_by_user items_rated_by_user_t ;
/*
 * Create an array of the ratings'order in the training set of the items rated by each user
 * tset						training set of the known ratings
 */
items_rated_by_user_t *
	init_items_rated_by_user(training_set_t* tset);


/*
 * Create an array of the ratings'order in the training set of the near items rated of each item
 * tset						training set of the known ratings
 * bin_width				bin's width of the LSH
 * proj_family_size			the projection's family size used by LSH
 * seed						random seed used by LSH 
 */
items_rated_by_user_t *
	get_nearest_neighbors (training_set_t* tset,int bin_width,int proj_family_size,int seed);

#endif
