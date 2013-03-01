#ifndef SERIALIZE_SPARSE_MATRIX_H
#define SERIALIZE_SPARSE_MATRIX_H
#include "../sparse_matrix.h"

int save_sparse_matrix(sparse_matrix_t* matrix);
sparse_matrix_t* load_sparse_matrix();

int save_coo_matrix(coo_matrix_t* coo);
coo_matrix_t* load_coo_matrix();

#endif