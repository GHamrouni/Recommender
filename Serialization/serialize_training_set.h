#ifndef SERIALIZE_TRAINING_SET_H
#define SERIALIZE_TRAINING_SET_H
#include "../training_set.h"

int save_training_set(training_set_t* tset);
training_set_t* load_training_set();

#endif