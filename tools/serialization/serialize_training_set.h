#ifndef SERIALIZE_TRAINING_SET_H
#define SERIALIZE_TRAINING_SET_H
#include "../../src/training_set.h"
#include "redis_parameters.h"

/* 
 * Save the learned factors to redis
 */
int save_training_set(training_set_t* tset,redis_parameters_t redis_parameters);
/* 
 * Load the learned factors from redis
 */
training_set_t* load_training_set(redis_parameters_t redis_parameters);

#endif