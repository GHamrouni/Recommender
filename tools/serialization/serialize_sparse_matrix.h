#ifndef SERIALIZE_SPARSE_MATRIX_H
#define SERIALIZE_SPARSE_MATRIX_H
#include "../../src/sparse_matrix.h"
#include "hiredis-win32/hiredis.h"
int save_sparse_matrix (sparse_matrix_t* matrix, redisContext *c);
sparse_matrix_t* load_sparse_matrix (redisContext *c);

int save_coo_matrix (coo_matrix_t* coo, redisContext *c);
coo_matrix_t* load_coo_matrix (redisContext *c);

#endif