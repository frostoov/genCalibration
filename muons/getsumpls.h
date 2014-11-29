#ifndef GETSUMPLS_H
#define GETSUMPLS_H

#include <stdio.h>
#include <sys/timeb.h>
#include "defines.h"

int GetSummaPulse(int clast, short int (*DATA)[1024*4], float *PLS );

#endif // GETSUMPLS_H
