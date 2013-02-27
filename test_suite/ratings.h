#include<stdlib.h>
#ifndef RATINGS_H
#define RATINGS_H

struct ratings{
	size_t user;
	size_t item;
	float rating;
};

typedef struct ratings ratings_t;

ratings_t* init_ratings(size_t size);

void set_rating(size_t user_index, size_t item_index,
	       	float value,int index, ratings_t* ratings_set);

float get_rating(size_t user_index, size_t item_index,
	       	 ratings_t* ratings_set);


#endif 