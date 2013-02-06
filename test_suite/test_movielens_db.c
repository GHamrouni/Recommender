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
//
//model_parameters_t params = { 0 };
//int RMSE(learned_factors_t* learned,learning_model_t model){
//	unsigned int i,j,l;
//	double k;
//	float sum=0;
//	int s;
// 	FILE* file_test;
//	size_t u;
//
//	training_set_t * tset_test;
//	//Calculate RMSE
// 	params.training_set_size = 20000;
// 
// 	tset_test = init_training_set(params);
// 
// 	file_test = fopen("u1.test", "r");
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
// 		set_known_rating(i - 1, j - 1, l, tset_test);
// 	}
// 
// 	fclose(file_test);
// 
// 	compile_training_set(tset_test);
//	printf("users [12] item [203], rating = %f expected 1 \n", estimate_rating_from_factors(21, 376, learned, model));
// 	for (s = 0; s < tset_test->training_set_size; s++)
// 	{
// 		i = tset_test->ratings->entries[s].row_i;
// 		u = tset_test->ratings->entries[s].column_j;
// 
// 		sum += powf(tset_test->ratings->entries[s].value - estimate_rating_from_factors(u, i, learned, model), 2) / 20000.0;
// 	}
// 
// 	//Rating estimation
// 	//printf("users [12] item [203], rating = %f expected 1 \n", estimate_rating_from_factors(21, 376, learned, model));
// 	//printf("users [190] item [3], rating = %f expected = 4\n", estimate_rating_from_factors(307, 0, learned, model));
// 
// 	printf("RMSE = %f \n", sqrtf(sum));
//
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
//
//	//Model configuration
//	//Setup model parameters
//	
//
//	FILE* file = NULL;
//
//	clock_t start = clock();
//
//	params.items_number = 1682;
//	params.users_number = 943;
//	params.training_set_size = 100000;
//
//	params.dimensionality = 30;
//	params.iteration_number = 30;
//
//	params.lambda = 0.055f;
//	params.step = 0.0095f;
//
//	params.lambda_bias = 0.02f;
//	params.step_bias = 0.001f;
//
//	//Use the basic matrix factorization model
//	model.learning_algorithm = learn_mf_bias;
//	model.rating_estimator   = estimate_rating_mf_bias;
//	model.parameters		 = params;
//
//	//Learning
//	//Initialize a training set
//	tset = init_training_set(params);
//
//	file = fopen("u.data", "r"); 
//
//	assert(file);
//
//	if (!file)
//		return 1;
//
//	
//
//	while (!feof(file)) {
//		if (fscanf(file, "%d %d %lf %d", &i, &j, &k, &l) != 4)
//			break;
//
//		set_known_rating(i - 1, j - 1, (float) k, tset);
//	}
//
//	fclose(file);
//
//	compile_training_set(tset);
//
//	learned = learn(tset, model);
//	
//
//	printf("users [12] item [203], rating = %f expected 1 \n", estimate_rating_from_factors(21, 376, learned, model));
//// 	params.iteration_number = 20;
//// 
//// 	for (i = 0; i < 10; i++)
//// 		update_learned_factors_mf_bias(learned, tset, params);
//
//	
//	RMSE(learned,model);
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