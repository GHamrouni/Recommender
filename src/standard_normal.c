/* Generate normal random samples with the Ziggurat method */

#include "standard_normal.h"

#include <math.h>
#include <limits.h>

#include <stdlib.h>

static double kn[128];
static double wn[128];
static double fn[128];

static int 
SHR3 (normal_generator_t* gen) 
{
	int jz  = gen->seed;
	int jzr = gen->seed;

	jzr^=(jzr<<13);
	jzr^=(jzr>>17);
	jzr^=(jzr<<5);
	gen->seed = jzr;

	return jz + jzr;
}


static double 
UNI(normal_generator_t* gen) 
{
	return 0.5 * (1 + (double)SHR3(gen)/((double) INT_MIN));
}

static double 
nfix(int hz, int iz, normal_generator_t* gen)
{
	double r = 3.5283501;
	double r1 = 1/r;
	double x, y;

	for(;;)
	{  
		x = hz*wn[iz];

		if(iz == 0)
		{ 
			do { 
				x = (-log(UNI(gen))*r1); 
				y = -log(UNI(gen));
			} while(y+y<x*x);

	        return (hz>0)? r+x : -r-x;
	    }

	    if(fn[iz]+UNI(gen)*(fn[iz-1]-fn[iz]) < exp(-.5*x*x)) 
	        return x;

	    hz = SHR3(gen);
	    iz = hz&127;
	 
	    if(abs(hz)<kn[iz]) 
		    return (hz*wn[iz]);
	}
}

static double 
RNOR(normal_generator_t* gen)
{
	int hz, iz;

	hz = SHR3(gen); 
	iz = hz & 127;
	
	return (abs(hz) < kn[iz]) ? hz*wn[iz] : nfix(hz, iz, gen);
}

/* 
 * Initialize a new RNG with the specified initial seed                
 */
normal_generator_t 
init_normal_distribution(unsigned long seed)
{  
	const double m1 = 2147483648.0;

	double dn = 3.5283501;
	double tn = dn;
	double vn = 9.91256303526217e-3;
	double q;

	
	normal_generator_t gen = { 123456789 };

	int i;

	gen.seed^=seed;

	q = vn/exp(-.5*dn*dn);
	kn[0] = (dn/q)*m1;
	kn[1] = 0;

	wn[0]   = q/m1;
	wn[127] = dn/m1;

	fn[0]   = 1.0;
	fn[127] = exp(-0.5*dn*dn);

	for(i = 126;i >= 1; i--)
	{
		dn = sqrt(-2.0*log(vn/dn+exp(-0.5*dn*dn)));
		kn[i+1] = (dn/tn)*m1;
		tn = dn;
		fn[i] = exp(-0.5*dn*dn);
		wn[i] = dn / m1;
	}

	return gen;
}


/* 
 * next_gaussian: Get a new random value from the   
 * RNG for a normal distribution                    
 */
double 
next_gaussian(normal_generator_t* gen) 
{
	return RNOR(gen);
}


