#include "rlog.h"
#include <stdio.h>

void RLog(const char *format, ...)
{
va_list arglist;

va_start( arglist, format );
#if DEBUG
vprintf( format, arglist );
#endif
va_end( arglist );
}