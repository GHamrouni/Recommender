#include "float_tester.h"
#include <float.h>

int
is_valid(double number)
{
	return  (number == number) && 
		(number <= DBL_MAX && 
		 number >= -DBL_MAX);
}
