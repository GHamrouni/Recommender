#ifndef REDIS_PARAMETERS
#define REDIS_PARAMETERS
struct redis_parameters
{
	char * ip_adr;
	int port;
};
typedef struct redis_parameters redis_parameters_t;
#endif
