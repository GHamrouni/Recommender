#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../hiredis-win32/hiredis.h"
#include "serialize_training_set.h"
#include "Serialize_sparse_matrix.h"


int save_training_set (training_set_t* tset)
{
	redisContext *c;
	redisReply *reply;

	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	c = redisConnect ( (char*) "127.0.0.1", 6379);
	if (c->err)
	{
		printf ("Connection error: %s\n", c->errstr);
		return (1);
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

training_set_t* load_training_set()
{
	redisContext *c;
	redisReply *reply;
	training_set_t* tset;
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	c = redisConnect ( (char*) "127.0.0.1", 6379);
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
	return tset;
}