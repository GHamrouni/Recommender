/*-
* Copyright (c) 2012 Hamrouni Ghassen.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

/*
 *
 * Recommender system using matrix factorization (MF)
 * Computing the product recommendation using latent factor models
 *
 */

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

/*
 * Calculate the dot product
 */
double
dot_product(double* vect1, double* vect2, unsigned int dim)
{
    double sum = 0;
    unsigned int i;

    for (i = 0; i < dim; i++)
        sum += vect1[i] * vect2[i];

    return sum;
}

/*
 * Get the sum of the elements of a vector
 */
double
sum(double* vector, unsigned int size)
{
	double sum = 0;
	unsigned int i;

	for (i = 0; i < size; i++)
		sum += vector[i];

	return sum;
}

/*
* length2:  Return the squared length of a vector
*
* Arguments:
*      vector            A vector (array) of double
*      size              The size of the vector
*
* Returns:
*      Return the squared length of a vector.
*
*/
double
length2(double* vector, unsigned int size)
{
    double sum = 0;
    unsigned int i;

    for (i = 0; i < size; i++)
        sum += vector[i] * vector[i];

    return sum;
}

//Calculate Pearson correlation coefficient
double pearson_correlation_coefficient(double* vectX, double* vectY, unsigned int dim)
{
	double sumXX = length2(vectX, dim);
	double sumYY = length2(vectY, dim);

	double sumX = sum(vectX, dim);
	double sumY = sum(vectY, dim);

	double N = dim;

	double d = (dot_product(vectX, vectY, dim) - (sumX * sumY) / N);
	double f = sqrtf((sumXX - sumX * sumX / N) * (sumYY - sumY * sumY / N));

	return d / f;
}