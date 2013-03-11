#include "serialize_sparse_matrix.h"
#ifdef _WIN32
#include "hiredis-win32/hiredis.h"
#else
#include "hiredis/hiredis.h"
#endif
#include <stdlib.h>

int save_sparse_matrix (sparse_matrix_t* matrix, redisContext *c)
{
	redisReply *reply;
	size_t i;

	reply = redisCommand (c, "SET %s %d", "column_nb", matrix->column_nb);
	freeReplyObject (reply);
	reply = redisCommand (c, "SET %s %d", "row_nb", matrix->row_nb);
	freeReplyObject (reply);
	reply = redisCommand (c, "SET %s %d", "nonzero_entries_nb", matrix->nonzero_entries_nb);
	freeReplyObject (reply);
	reply = redisCommand (c, "SET %s %d", "row_capacity", matrix->row_capacity);
	freeReplyObject (reply);
	reply = redisCommand (c, "SET %s %d", "nonzero_entries_capacity", matrix->nonzero_entries_capacity);
	freeReplyObject (reply);
	reply = redisCommand (c, "DEL values");
	freeReplyObject (reply);
	reply = redisCommand (c, "DEL column_index");
	freeReplyObject (reply);
	for ( i = 0 ; i < matrix->nonzero_entries_nb; i++)
	{
		reply = redisCommand (c, "RPUSH values %f", matrix->values[i]);
		freeReplyObject (reply);
		reply = redisCommand (c, "RPUSH column_index %d", matrix->column_index[i]);
		freeReplyObject (reply);
	}
	reply = redisCommand (c, "DEL row_index");
	freeReplyObject (reply);
	for (i = 0; i < matrix->row_nb + 1 ; i++)
	{
		reply = redisCommand (c, "RPUSH row_index %d", matrix->row_index[i]);
		freeReplyObject (reply);
	}
	return 0;
}


sparse_matrix_t* load_sparse_matrix (redisContext *c)
{
	redisReply *reply;
	size_t i;
	sparse_matrix_t* matrix;

	matrix = malloc (sizeof (sparse_matrix_t) );
	reply = redisCommand (c, "GET column_nb");
	matrix->column_nb = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET row_nb");
	matrix->row_nb = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET nonzero_entries_nb");
	matrix->nonzero_entries_nb = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET row_capacity");
	matrix->row_capacity = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET nonzero_entries_capacity");
	matrix->nonzero_entries_capacity = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET nonzero_entries_capacity");
	matrix->nonzero_entries_capacity = atoi (reply->str);
	freeReplyObject (reply);

	matrix->values = malloc (matrix->nonzero_entries_capacity * sizeof (float) );
	matrix->column_index = malloc (matrix->nonzero_entries_capacity * sizeof (size_t) );

	reply = redisCommand (c, "LRANGE values 0 -1");
	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == matrix->nonzero_entries_nb)
	{
		for (i = 0; i < reply->elements; i++)
		{
			matrix->values[i] = atof (reply->element[i]->str);
		}
	}
	reply = redisCommand (c, "LRANGE column_index 0 -1");
	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == matrix->nonzero_entries_nb)
	{
		for (i = 0; i < reply->elements; i++)
		{
			matrix->column_index[i] = atoi (reply->element[i]->str);
		}
	}
	freeReplyObject (reply);
	matrix->row_index = malloc (matrix->row_capacity * sizeof (size_t) );
	reply = redisCommand (c, "LRANGE row_index 0 -1");
	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == matrix->row_nb + 1)
	{
		for (i = 0; i < matrix->row_nb + 1; i++)
		{
			matrix->row_index[i] = atoi (reply->element[i]->str);
		}
	}
	return matrix;
}


int save_coo_matrix (coo_matrix_t* coo, redisContext *c)
{

	redisReply *reply;
	size_t i;

	reply = redisCommand (c, "SET %s %d", "current_size", coo->current_size);
	freeReplyObject (reply);
	reply = redisCommand (c, "SET %s %d", "size", coo->size);
	freeReplyObject (reply);
	reply = redisCommand (c, "DEL coo_entries_column_j");
	reply = redisCommand (c, "DEL coo_entries_row_i");
	reply = redisCommand (c, "DEL coo_entries_value");
	freeReplyObject (reply);
	for (i = 0; i < coo->size; i++)
	{
		reply = redisCommand (c, "RPUSH coo_entries_column_j %d", coo->entries[i].column_j);
		reply = redisCommand (c, "RPUSH coo_entries_row_i %d", coo->entries[i].row_i);
		reply = redisCommand (c, "RPUSH coo_entries_value %f", coo->entries[i].value);
		freeReplyObject (reply);
	}
	return 0;

}
coo_matrix_t* load_coo_matrix (redisContext *c)
{
	redisReply *reply;
	size_t i;
	coo_matrix_t* coo;
	coo = malloc (sizeof (coo_matrix_t) );
	reply = redisCommand (c, "GET current_size");
	coo->current_size = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET size");
	coo->size = atoi (reply->str);
	coo->entries = malloc (coo->size * sizeof (coo_entry_t) );
	reply = redisCommand (c, "lrange coo_entries_column_j 0 -1");
	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == coo->size)
	{
		for (i = 0; i < reply->elements; i++)
		{
			coo->entries[i].column_j = atoi (reply->element[i]->str);
		}
	}
	reply = redisCommand (c, "lrange coo_entries_row_i 0 -1");
	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == coo->size)
	{
		for (i = 0; i < reply->elements; i++)
		{
			coo->entries[i].row_i = atoi (reply->element[i]->str);
		}
	}
	reply = redisCommand (c, "lrange coo_entries_value 0 -1");
	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == coo->size)
	{
		for (i = 0; i < reply->elements ; i++)
		{
			coo->entries[i].value = atof (reply->element[i]->str);
		}
	}
	return coo;
}
