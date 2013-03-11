#ifndef SERIALIZE_FACTORS_H
#define SERIALIZE_FACTORS_H
#include "../../src/learned_factors.h"
#include "redis_parameters.h"

int test_redis (void);

int save_learned_factors (learned_factors_t *learned,redis_parameters_t redis_parameters);
learned_factors_t* load_learned_factors(redis_parameters_t redis_parameters);


#endif