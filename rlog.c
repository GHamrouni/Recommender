#include "rlog.h"
#include <stdio.h>
void RLog(const char *format, ...)
{
va_list arglist;

va_start( arglist, format );
vprintf( format, arglist );
va_end( arglist );
}