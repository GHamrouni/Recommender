#include "ratings.h"
#include<stdlib.h>

ratings_t* init_ratings(size_t size){
	ratings_t * rating_set=(ratings_t*) malloc(sizeof(ratings_t)*size);
	return rating_set;
}

void set_rating(size_t user_index, size_t item_index,
	       	float value, size_t index ,ratings_t* ratings_set){
	
				ratings_set[index].user=user_index;
				ratings_set[index].item=item_index;
				ratings_set[index].rating=value;
}

float get_rating(size_t user_index, size_t item_index,
	       	 int index, ratings_t* ratings_set){

				 return ratings_set[index].rating;
}



