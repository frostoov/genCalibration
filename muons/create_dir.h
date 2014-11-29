#ifndef CREATE_DIR_H
#define CREATE_DIR_H

#include <stdio.h>
#include "defines.h"
#include <sys/stat.h>

extern struct timeb Now,Stop;
extern struct tm t;
extern FILE *ffstat, *fftime, *fferr, *ffdat;
extern char pathfile[120];
extern char statfile[120];
extern char timefile[120];
extern char summfile[120];
extern char errfile[120];
extern char datafile[120];

unsigned long Create_Dir_Files(void);

#endif // CREATE_DIR_H
