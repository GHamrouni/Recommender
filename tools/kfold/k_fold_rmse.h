#ifndef K_FOLD_RSME_H
#define K_FOLD_RMSE_H

#include "k_fold_parameters.h"
#include "training_set.h"



/*
* RMSE : Calculate the RMSE between a validation set and 
*
* Arguments :  
*		learned : the learned factors
*		model : the learing model
*		_validation_set : the exepected ratings values
*		_k_fold_params : the fold parameters
* 
*	Returns :
*		return the RMSE
**/
double RMSE (learned_factors_t* learned, training_set_t * _validation_set,
             training_set_t * tset) ;

/*
* RMSE : Calculate the RMSE
*
*Arguments : 
*		 _k_fold_params : the fold parameters
*Returns :
*		return the RMSE
**/
double RMSE_mean(k_fold_parameters_t* k_fold_params);

#endif
