#include"data_set.h"
#include <stdio.h>
#include <assert.h>

#if WIN32
#pragma warning(disable:4996)
#endif

int extract_data (struct k_fold_parameters _k_fold_params, training_set_t** _t_set, training_set_t ** _validation_set,
                  int index)
{
	FILE* file;
	unsigned int i, j, l, ss;
	double m;

	*_t_set = init_training_set (_k_fold_params.params);
	_k_fold_params.params.training_set_size = (size_t) (_k_fold_params.ratings_number / _k_fold_params.K);
	*_validation_set = init_training_set (_k_fold_params.params);
	file = fopen (_k_fold_params.file_path, "r");

	assert (file);

	if (!file)
	{
		return -1;
	}

	ss = 0;

	while (!feof (file))
	{
		if (fscanf (file, "%u %u %lf %u", &i, &j, &m, &l) != 4)
		{
			break;
		}
		++ss;
		if ( (ss <= index * _k_fold_params.ratings_number / _k_fold_params.K) || (ss > (index + 1) *_k_fold_params.ratings_number / _k_fold_params.K) )
		{
			set_known_rating (i - 1, j - 1, (float) m, *_t_set);
		}
		else
		{
			set_known_rating (i - 1, j - 1, (float) m, *_validation_set);
		}
	}

	fclose (file);
	return 0;
}


int extract_data_2_tset (struct k_fold_parameters _k_fold_params, training_set_t** _t_set, 
	training_set_t ** _validation_set, training_set_t ** new_tset, int index)
{
	FILE* file;
	unsigned int i, j, l, ss;
	double m;
	_k_fold_params.params.training_set_size = (size_t) (0.5 * (_k_fold_params.K-1)*(_k_fold_params.ratings_number / _k_fold_params.K));
	*_t_set = init_training_set (_k_fold_params.params);
	*new_tset = init_training_set (_k_fold_params.params);
	_k_fold_params.params.training_set_size = (size_t) (2 * _k_fold_params.params.training_set_size / (_k_fold_params.K-1));
	*_validation_set = init_training_set (_k_fold_params.params);

	
	
	file = fopen (_k_fold_params.file_path, "r");

	assert (file);

	if (!file)
	{
		return -1;
	}

	ss = 0;

	while (!feof (file))
	{
		if (fscanf (file, "%u %u %lf %u", &i, &j, &m, &l) != 4)
		{
			break;
		}
		++ss;
		if ( (ss <= index * _k_fold_params.ratings_number / _k_fold_params.K) || (ss > (index + 1) *_k_fold_params.ratings_number / _k_fold_params.K) )
		{
			if(ss % 2==0)
			set_known_rating (i - 1, j - 1, (float) m, *_t_set);
			else
			set_known_rating (i - 1, j - 1, (float) m, *new_tset);
		}
		else
		{
			set_known_rating (i - 1, j - 1, (float) m, *_validation_set);
		}
	}

	fclose (file);
	return 0;
}