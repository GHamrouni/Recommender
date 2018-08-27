#ifndef DATA_SET_H
#define DATA_SET_H

#include "k_fold_parameters.h"
#include "training_set.h"

/*
* read_file : read the file and outputs 2 training_sets
* _tset : the traing set that's going to be read
* _validation_set : the validation set 
* _model : the learning model 
* index : the index of the validation part
* Returns
*		0 if success
*		-1 else
*/
int extract_data(struct k_fold_parameters * _k_fold_params, training_set_t** _t_set,training_set_t ** _validation_set,
	size_t index);

int extract_data_2_tset (struct k_fold_parameters * _k_fold_params, training_set_t** _t_set, 
	training_set_t ** _validation_set, training_set_t ** new_tset, int index);


sparse_matrix_t* extract_social_realtions (char* file_path, size_t users_number,size_t relations_number);

#endif
