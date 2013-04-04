#include "projection_family.h"
#include <stdlib.h>

projection_family_t*
init_random_projections(unsigned int dim, unsigned int seed, 
                        unsigned int bin_width, size_t projNb)
{
	projection_family_t* pfamily = malloc(sizeof(projection_family_t));
	normal_generator_t gen = init_normal_distribution(seed);
	size_t i;

	if (!pfamily)
		return NULL;

	pfamily->projections = malloc(projNb * sizeof(projection_t*));
	pfamily->projection_nb = projNb;
	
	if (!pfamily->projections)
		return pfamily;

	for (i = 0; i < projNb; i++)
		pfamily->projections[i] = 
                     init_random_projection_rng(dim, seed, bin_width, &gen);

	return pfamily;
}

void
free_projection_family(projection_family_t* pfamily)
{
	size_t i;
	for (i = 0; i < pfamily->projection_nb; i++)
	{
		free(pfamily->projections[i]->vector);
		free(pfamily->projections[i]);
	}
	free(pfamily);
}

int 
lsh_data(projection_family_t* pfamily, double* data)
{
 	size_t i;
	int hash = 1;

	for (i = 0; i < pfamily->projection_nb; i++)
		hash = 33 * hash + project_data(pfamily->projections[i], data);

	return hash;
}

