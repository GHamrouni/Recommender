#include "projection.h"
#include "standard_normal.h"

#include <math.h>
#include <stdlib.h>

static int initialized = 0;

projection_t*
init_random_projection(size_t dim, unsigned int seed, unsigned int bin_width)
{
	normal_generator_t gen = init_normal_distribution(seed);
	size_t i;
	projection_t* proj = malloc(sizeof(projection_t));

	if (!proj)
		return NULL;

	if (!initialized)
	{
		srand(seed);
		initialized = 1;
	}

	proj->dimension = dim;
	proj->vector = malloc(sizeof(double) * dim);
	proj->bin_width = bin_width;
	proj->bias = rand() % (bin_width + 1);

	if (!proj->vector)
		return proj;

	for (i = 0; i < dim; i++)
		proj->vector[i] = next_gaussian(&gen);

	return proj;
}

projection_t*
init_random_projection_rng(size_t dim, unsigned int seed, 
                           unsigned int bin_width, normal_generator_t* gen)
{
	size_t i;
	projection_t* proj = malloc(sizeof(projection_t));

	if (!proj)
		return NULL;

	if (!initialized)
	{
		srand(seed);
		initialized = 1;
	}

	proj->dimension = dim;
	proj->vector = malloc(sizeof(double) * dim);
	proj->bin_width = bin_width;
	proj->bias = rand() % (bin_width + 1);

	if (!proj->vector)
		return proj;

	for (i = 0; i < dim; i++)
		proj->vector[i] = next_gaussian(gen);

	return proj;	
}

int 
project_data(projection_t* proj, double* data)
{
	size_t i;
	double dot_product_r = 0;

	for (i = 0; i < proj->dimension; i++)
		dot_product_r += proj->vector[i] * data[i];

	return (int) floor((dot_product_r + proj->bias) / (double) proj->bin_width);
}

void
free_projection(projection_t* proj)
{
	free(proj->vector);
	free(proj);
}

