#include "rating.h"
#include<stdlib.h>

rating_t* init_rating(size_t size){
	rating_t * rating_set=(rating_t*) malloc(sizeof(rating_t)*size);
	return rating_set;
}

void set_rating(unsigned int user_index, unsigned int item_index,
	       	float value,unsigned int time_stamp, size_t index ,rating_t* ratings_set){
	
				ratings_set[index].user=user_index;
				ratings_set[index].item=item_index;
				ratings_set[index].rating=value;
				ratings_set[index].time_stamp=time_stamp;
}

rating_t get_rating(unsigned int index,
	       	 rating_t* ratings_set){

				 return ratings_set[index];
}



