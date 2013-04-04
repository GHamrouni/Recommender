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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>

/* MSVC doesn't define fmax/fmin in math.h */
#if defined(_WIN32) || defined(_WIN64)
#define fmax max
#define fmin min
#endif

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
length2(double* vector, size_t size);

/*
 * Get the sum of the elements of a vector
 */
double
sum(double* vector, size_t size);

/*
 * Calculate the dot product between the two vectors.
 */
double
dot_product(double* vect1, double* vect2, size_t dim);

/*
 * Calculate Pearson correlation coefficient
 */
double
pearson_correlation_coefficient(double* vectX, double* vectY, size_t dim);

/*
 * Add two vectors together
 *    The result is in vect1
 */
void
add_vector(double* vect1,double * vect2, size_t dim);

/*
 * Multiply a vector by a scalar value
 *
 */
void
scalar_product(double * vector,double scalar,size_t dim);

/*
 * Generate a random vector
 */
double * generate_random_vector(int n,int seed);

/*
 * Generate a random matrix
 */
double** generate_random_matrix(int nrow,int ncol,int seed);

#endif

