#! /bin/bash

gcc test_suite/test_all.c test_suite/test_red_black_tree_1.c test_suite/test_bheap_1.c test_suite/test_sparse_matrix_1.c test_suite/test_training_set_1.c src/libRecommender.a -lm -o test_suite/test_all


./test_suite/test_all


