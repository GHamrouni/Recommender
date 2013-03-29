#ifndef ITEMS_RATED_BY_USER
#define ITEMS_RATED_BY_USER
#include "training_set.h"


struct items_rated_by_user
{
	size_t * ratings_order;
	size_t items_number;
};
typedef struct items_rated_by_user items_rated_by_user_t ;
items_rated_by_user_t *
	init_items_rated_by_user(training_set_t* tset);

//items_rated_by_user_t *
//	get_nearest_neighbors( training_set_t* tset, items_rated_by_user_t ratings_by_user, size_t item_index);
items_rated_by_user_t *
	get_nearest_neighbors( training_set_t* tset);

#endif