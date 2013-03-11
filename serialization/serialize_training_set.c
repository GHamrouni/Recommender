#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include "../hiredis-win32/hiredis.h"
#else
#include "../hiredis/hiredis.h"
#endif
#include "serialize_training_set.h"
#include "serialize_sparse_matrix.h"



int save_training_set (training_set_t* tset, redis_parameters_t redis_parameters)
{
	redisContext *c;
	redisReply *reply;

	c = redisConnect ( redis_parameters.ip_adr, redis_parameters.port);
	if (c->err)
	{
		printf ("Connection error: %s\n", c->errstr);
		return (-1);
	}
	reply = redisCommand (c, "SET %s %d", "dimensionality", tset->dimensionality);
	freeReplyObject (reply);
	reply = redisCommand (c, "SET %s %d", "items_number", tset->items_number);
	freeReplyObject (reply);
	reply = redisCommand (c, "SET %s %d", "users_number", tset->users_number);
	freeReplyObject (reply);
	reply = redisCommand (c, "SET %s %d", "training_set_size", tset->training_set_size);
	freeReplyObject (reply);
	reply = redisCommand (c, "SET %s %f", "rating_sum", tset->ratings_sum);
	freeReplyObject (reply);
	if (save_sparse_matrix (tset->ratings_matrix, c) )
	{
		return -1;
	}
	if (save_coo_matrix (tset->ratings, c) )
	{
		return -1;
	}
	redisFree (c);
	return 0;
}

training_set_t* load_training_set (redis_parameters_t redis_parameters)
{
	redisContext *c;
	redisReply *reply;
	training_set_t* tset;
	c = redisConnect ( redis_parameters.ip_adr, redis_parameters.port);
	if (c->err)
	{
		printf ("Connection error: %s\n", c->errstr);
		return NULL;
	}
	tset = malloc (sizeof (training_set_t) );
	reply = redisCommand (c, "GET dimensionality");
	tset->dimensionality = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET items_number");
	tset->items_number = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET users_number");
	tset->users_number = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET training_set_size");
	tset->training_set_size = atoi (reply->str);
	freeReplyObject (reply);
	reply = redisCommand (c, "GET rating_sum");
	tset->ratings_sum = atof (reply->str);
	freeReplyObject (reply);
	tset->ratings = load_coo_matrix (c);
	tset->ratings_matrix = load_sparse_matrix (c);
	redisFree (c);
	return tset;
}
