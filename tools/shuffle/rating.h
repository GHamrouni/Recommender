#include<stdlib.h>
#ifndef RATING_H
#define RATING_H

struct rating{
	unsigned int user;
	unsigned int item;
	float rating;
	unsigned int time_stamp;

};

typedef struct rating rating_t;

rating_t* init_ratings(size_t size);

void set_rating(unsigned int user_index, unsigned int item_index,
	       	float value,unsigned int time_stamp,unsigned int index, rating_t* ratings_set);

rating_t get_rating(unsigned int index,rating_t* ratings_set);



#endif 