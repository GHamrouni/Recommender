#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include "hiredis-win32\hiredis.h"
#else
#include "hiredis/hiredis.h"
#endif
#include "serialize_factors.h"
#include "redis_parameters.h"



int save_learned_factors (learned_factors_t * factors, redis_parameters_t redis_parameters)
{
	redisContext *c;
	redisReply *reply;
	size_t i, j;
	c = redisConnect ( redis_parameters.ip_adr, redis_parameters.port);
	if (c->err)
	{
		printf ("Connection error: %s\n", c->errstr);
		return -1;
	}
	reply = redisCommand (c, "SET %s %d", "dimensionality", factors->dimensionality);
	printf ("SET: %s\n", reply->str);
	freeReplyObject (reply);

	reply = redisCommand (c, "SET %s %d", "users_number", factors->users_number);
	printf ("SET: %s\n", reply->str);
	freeReplyObject (reply);

	reply = redisCommand (c, "SET %s %d", "items_number", factors->items_number);
	printf ("SET: %s\n", reply->str);
	freeReplyObject (reply);

	reply = redisCommand (c, "SET %s %1.29f", "rating_average", factors->ratings_average);
	printf ("SET: %s\n", reply->str);
	freeReplyObject (reply);

	reply = redisCommand (c, "DEL user_bias");
	freeReplyObject (reply);
	for (i = 0; i < factors->users_number; i++)
	{
		reply = redisCommand (c, "RPUSH user_bias %f", factors->user_bias[i]);
		freeReplyObject (reply);
		reply = redisCommand (c, "DEL user_factors_%d", i);
		freeReplyObject (reply);
		for (j = 0; j < factors->dimensionality; j++)
		{
			reply = redisCommand (c, "RPUSH user_factors_%d %f", i, factors->user_factor_vectors[i][j]);
			freeReplyObject (reply);
		}
	}

	reply = redisCommand (c, "DEL item_bias");
	freeReplyObject (reply);
	for (i = 0; i < factors->items_number; i++)
	{
		reply = redisCommand (c, "RPUSH item_bias %f", factors->item_bias[i]);
		freeReplyObject (reply);
		reply = redisCommand (c, "DEL item_factors_%d", i);
		freeReplyObject (reply);
		for (j = 0; j < factors->dimensionality; j++)
		{
			reply = redisCommand (c, "RPUSH item_factors_%d %f", i, factors->item_factor_vectors[i][j]);
			freeReplyObject (reply);
		}
	}
	return 0;
}


learned_factors_t * load_learned_factors (redis_parameters_t redis_parameters)
{
	learned_factors_t* factors;
	redisContext *c;
	redisReply *reply;
	size_t i, j;
	c = redisConnect ( redis_parameters.ip_adr, redis_parameters.port);
	if (c->err)
	{
		printf ("Connection error: %s\n", c->errstr);
		return NULL;
	}
	factors = malloc (sizeof (learned_factors_t) );
	reply = redisCommand (c, "GET dimensionality");
	factors->dimensionality = atoi (reply->str);
	freeReplyObject (reply);

	reply = redisCommand (c, "GET users_number");
	factors->users_number = atoi (reply->str);
	freeReplyObject (reply);

	reply = redisCommand (c, "GET items_number");
	factors->items_number = atoi (reply->str);
	freeReplyObject (reply);

	reply = redisCommand (c, "GET rating_average");
	factors->ratings_average = atof (reply->str);
	freeReplyObject (reply);

	factors->user_bias = malloc (sizeof (double) * factors->users_number);
	reply = redisCommand (c, "LRANGE user_bias 0 -1");
	if (reply->type == REDIS_REPLY_ARRAY && reply->elements == factors->users_number)
	{
		for (i = 0; i < reply->elements; i++)
		{
			factors->user_bias[i] = atof (reply->element[i]->str);
		}
	}

	factors->item_bias = malloc (sizeof (double) * factors->items_number);
	reply = redisCommand (c, "LRANGE item_bias 0 -1");
	if (reply->type == REDIS_REPLY_ARRAY)
	{
		for (i = 0; i < reply->elements; i++)
		{
			factors->item_bias[i] = atof (reply->element[i]->str);
		}
	}
	factors->user_factor_vectors =
	    malloc (sizeof (double*) * factors->users_number);
	for (i = 0; i < factors->users_number; i++)
	{
		factors->user_factor_vectors[i] =
		    malloc (sizeof (double*) * factors->users_number);
		reply = redisCommand (c, "LRANGE user_factors_%d 0 -1", i);
		if (reply->type == REDIS_REPLY_ARRAY)
		{
			factors->user_factor_vectors[i] = malloc (sizeof (double) * factors->dimensionality);
			for (j = 0; j < reply->elements; j++)
			{
				factors->user_factor_vectors[i][j] = atof (reply->element[j]->str);
			}
		}
	}
	factors->item_factor_vectors =
	    malloc (sizeof (double*) * factors->items_number);
	for (i = 0; i < factors->items_number; i++)
	{
		reply = redisCommand (c, "LRANGE item_factors_%d 0 -1", i);
		if (reply->type == REDIS_REPLY_ARRAY)
		{
			factors->item_factor_vectors[i] = malloc (sizeof (double) * factors->dimensionality);
			for (j = 0; j < reply->elements; j++)
			{
				factors->item_factor_vectors[i][j] = atof (reply->element[j]->str);
			}
		}
	}
	return factors;
}
