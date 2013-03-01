#ifndef SERIALIZE_FACTORS_H
#define SERIALIZE_FACTORS_H
#include "../learned_factors.h"	


int test_redis(void);

int save_learned_factors(learned_factors_t *learned);
learned_factors_t* load_learned_factors();


#endif