//#include "../recommender.h"
//
//#include <stdio.h>
//#include <assert.h>
//#include <stdlib.h>
//#include <memory.h>
//#include <math.h>
//#include <time.h>
//
//#include "../matrix_factorization.h"
//#include "../matrix_factorization_bias.h"
//#include "../sparse_matrix.h"
//#include "../red_black_tree.h"
//
//#define K 5.0
//#define sample_size 100000
//model_parameters_t params = { 0 };
//double RMSE(learned_factors_t* learned,learning_model_t model,training_set_t * _tset_test){
//	unsigned int i,j,l;
//	double k;
//	float sum=0;
//	int s;
//	size_t u;
//
//
//	//Calculate RMSE
// 	
// 
// 	
// 
// /*	file_test = fopen("u1.test", "r");
// 
// 	assert(file_test);
// 
// 	if (!file_test)
// 		return 1;
// 
// 	while (!feof(file_test)) {
// 		if (fscanf(file_test, "%d %d %d %d", &i, &j, &k, &l) != 4)
// 			break;
// 
// 		set_known_rating(i - 1, j - 1, l, _tset_test);
// 	}
// 
// 	fclose(file_test);*/
// 
// 	compile_training_set(_tset_test);
//	//printf("users [12] item [203], rating = %f expected 1 \n", estimate_rating_from_factors(21, 376, learned, model));
// 	for (s = 0; s < _tset_test->training_set_size; s++)
// 	{
// 		i = _tset_test->ratings->entries[s].row_i;
// 		u = _tset_test->ratings->entries[s].column_j;
// 
// 		sum += powf(_tset_test->ratings->entries[s].value - estimate_rating_from_factors(u, i, learned, model), 2) / 20000.0;
// 	}
// 
// 	//Rating estimation
// 	//printf("users [12] item [203], rating = %f expected 1 \n", estimate_rating_from_factors(21, 376, learned, model));
// 	//printf("users [190] item [3], rating = %f expected = 4\n", estimate_rating_from_factors(307, 0, learned, model));
// 
// 	printf("RMSE = %f \n", sqrtf(sum));
//	return (sqrtf(sum));
//}
//
//
//void print_recommended_items(rb_node_t* node, int depth)
//{
//	int i = 0;
//
//	if (node == NULL)
//		return;
//
//	for (i = 0; i < depth; i++)
//		printf("->");
//
//	printf("%f \n", ((recommended_item_t*) node->value)->rating);
//
//	depth++;
//
//	print_recommended_items(node->left_node, depth);
//	print_recommended_items(node->right_node, depth);
//}
//
//
//int read_file(training_set_t** _tset,training_set_t ** _tset_test,learning_model_t* _model,int index){
//	FILE* file;
//	unsigned int i, j, l,ss;
//	double m;
//	
//	params->items_number = 1682;
//	params->users_number = 943;
//
//	params->training_set_size = sample_size*((K-1)/K);
//
//	params->dimensionality = 30;
//	params->iteration_number = 30;
//
//	params->lambda = 0.055f;
//	params->step = 0.0095f;
//
//	params->lambda_bias = 0.02f;
//	params->step_bias = 0.001f;
//
//	//Use the basic matrix factorization model
//	_model->learning_algorithm = learn_mf_bias;
//	_model->rating_estimator   = estimate_rating_mf_bias;
//	_model->parameters		 = params;
//
//	//Learning
//	//Initialize a training set
//	*_tset = init_training_set(params);
//	params->training_set_size = sample_size/K;
//	*_tset_test=init_training_set(params);
//	file = fopen("u.data", "r"); 
//
//	assert(file);
//
//	if (!file)
//		return 1;
//
//	
//	ss=0;
//	while (!feof(file)) {
//		if (fscanf(file, "%d %d %lf %d", &i, &j, &m, &l) != 4)
//			break;
//		++ss;
//		if((ss<=index*sample_size/K)||(ss>(index+1)*sample_size/K))
//		set_known_rating(i - 1, j - 1, (float) m, *_tset);
//		else 
//		set_known_rating(i-1,j-1,(float)m,*_tset_test);
//	}
//
//	fclose(file);
//	return 0;
//}
//
//
//
//
//int main(void) {
//
//	learned_factors_t* learned;
//	training_set_t* tset;
//	int index;
//	double RMSE_sum=0;
//	recommended_items_t* r_items = NULL;
//
//	learning_model_t model = {0};
//
//	training_set_t * tset_test;
//
//
//
//	//Model configuration
//	//Setup model parameters
//	
//
//	FILE* file = NULL;
//
//	clock_t start = clock();
//
//		
//	for(index=0;index<5;index++){
//	read_file(&tset,&tset_test,&model,index);
//
//
//	compile_training_set(tset);
//
//	learned = learn(tset, model);
//	RMSE_sum+=RMSE(learned,model,tset_test);
//	}
//
//	printf("mean(RMSE)=%f",RMSE_sum/K);
//
//
////	printf("users [12] item [203], rating = %f expected 1 \n", estimate_rating_from_factors(21, 376, learned, model));
//// 	params->iteration_number = 20;
//// 
//// 	for (i = 0; i < 10; i++)
//// 		update_learned_factors_mf_bias(learned, tset, params);
//
//	
//	
////	r_items = recommend_items(100, 100, learned, tset, model);
//
////	print_recommended_items(r_items->items->head, 0);
//
//	printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
//
//	free_recommended_items(r_items);
//	free_learned_factors(learned);
//	free_training_set(tset);
//	getchar();
////	_CrtDumpMemoryLeaks();
//
//	return 0;
//}