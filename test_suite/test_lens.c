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
//#include "ratings.h"
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
//float RMSE(learned_factors_t* _learned,learning_model_t _model,ratings_t* expected_values){
//	int p;
//	double sum=0;
//	for(p=0;p<20;p++){
//		int user=expected_values[p].user;
//		int item=expected_values[p].item;
//		sum+=pow(expected_values[p].rating-estimate_rating_from_factors(user, item, _learned, _model),2);
//	}
//	return pow(sum/20,0.5);
//}
//
//int main(void) {
//
//	learned_factors_t* learned;
//	training_set_t* tset;
//
//	recommended_items_t* r_items = NULL;
//
//	learning_model_t model = {0};
//
//	unsigned int i, j, l;
//	double k;
//	int s=0;
//	ratings_t* ratings=init_ratings(100*.2);
//	
//
//	//Model configuration
//	//Setup model parameters
//	model_parameters_t params = { 0 };
//
//	FILE* file = NULL;
//
//	clock_t start = clock();
//
//	params->items_number = 65133;
//	params->users_number = 71567;
//	params->training_set_size = (size_t)100*0.8;
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
//	model->learning_algorithm = learn_mf_bias;
//	model->rating_estimator   = estimate_rating_mf_bias;
//	model->parameters		 = params;
//
//	//Learning
//	//Initialize a training set
//	tset = init_training_set(params);
//
//	file = fopen("ratings.dat", "r"); 
//
//	assert(file);
//
//	if (!file)
//		return 1;
//
//	
//
//	while (!feof(file)&&(s++<80)) {
//		if (fscanf(file, "%d::%d::%lf::%d", &i, &j, &k, &l) != 4)
//			break;
//
//		set_known_rating(i - 1, j - 1, (float) k, tset);
//	}
//	while(!feof(file)&&(s++<100)){
//		if (fscanf(file, "%d::%d::%lf::%d", &i, &j, &k, &l) != 4)
//			break;
//
//		set_rating(i-1, j-1,
//	       	(float) k, s-80 ,ratings);
//	}
//
//	fclose(file);
//
//	compile_training_set(tset);
//
//	learned = learn(tset, model);
//	
//
//	printf("users [12] item [203], rating = %f expected 1 \n", estimate_rating_from_factors(0, 615, learned, model));
//// 	params->iteration_number = 20;
//// 
//// 	for (i = 0; i < 10; i++)
//// 		update_learned_factors_mf_bias(learned, tset, params);
//
//	//Calculate RMSE
//// 	params->training_set_size = 20000;
//// 
//// 	tset_test = init_training_set(params);
//// 
//// 	file_test = fopen("C:\\Users\\gha\\Documents\\Visual Studio 2010\\Projects\\cdf\\u1.test", "r");
//// 
//// 	assert(file_test);
//// 
//// 	if (!file_test)
//// 		return 1;
//// 
//// 	while (!feof(file_test)) {
//// 		if (fscanf(file_test, "%d %d %d %d", &i, &j, &k, &l) != 4)
//// 			break;
//// 
//// 		set_known_rating(i - 1, j - 1, k, tset_test);
//// 	}
//// 
//// 	fclose(file_test);
//// 
//// 	compile_training_set(tset_test);
//// 
//// 	for (k = 0; k < tset_test->training_set_size; k++)
//// 	{
//// 		i = tset_test->ratings->entries[k].row_i;
//// 		u = tset_test->ratings->entries[k].column_j;
//// 
//// 		sum += powf(tset_test->ratings->entries[k].value - estimate_rating_from_factors(u, i, learned, model), 2) / 20000.0;
//// 	}
//// 
//// 	//Rating estimation
// //	printf("users [12] item [203], rating = %f expected 1 \n", estimate_rating_from_factors(21, 376, learned, model));
// //	printf("users [190] item [3], rating = %f expected = 4\n", estimate_rating_from_factors(307, 0, learned, model));
//// 
//// 	printf("RMSE = %f \n", sqrtf(sum));
//
//	r_items = recommend_items(100, 100, learned, tset, model);
//
//	//print_recommended_items(r_items->items->head, 0);
//	
//	//printf("RMSE = %f\n",RMSE(learned,model,ratings));
//	printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
//
//	free_recommended_items(r_items);
//	free_learned_factors(learned);
//	free_training_set(tset);
//
////	_CrtDumpMemoryLeaks();
//
//	return 0;
//}