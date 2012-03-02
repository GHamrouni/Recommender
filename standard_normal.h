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

/************************************************************************/
/* Generate normal random samples with the Ziggurat method              */
/* The method is described in Marsaglia and W. W. Tsang in the SIAM     */
/* Journal of Scientific and Statistical Programming, volume 5, 1984    */
/************************************************************************/

#ifndef NORMAL_DISTRIBUTION_H
#define NORMAL_DISTRIBUTION_H


/* Represents an RNG for a normal distribution */
typedef struct normal_generator
{
	unsigned long seed;
} normal_generator_t;


/*
 * learn: Initialize a new RNG with the specified initial seed
 *                               
 *
 * Arguments:
 *      initial_seed The initial value of the seed
 *
 * Returns:
 *      Return an initialized RNG.
 *
 */
normal_generator_t 
init_normal_distribution(unsigned long initial_seed);


/*
 * next_gaussian: Get a new random value from the RNG for a normal 
 * distribution
 *                               
 *
 * Arguments:
 *      gen  The random number generator
 *
 * Returns:
 *      A random sampled value.
 * 
 */
double 
next_gaussian(normal_generator_t* gen);

#endif

