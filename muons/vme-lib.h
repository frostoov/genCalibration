#ifndef VMELIB_H
#define VMELIB_H

#include <unistd.h>
#include "vme.hpp"

typedef unsigned long ULONG;
typedef unsigned int UWORD;

extern VME cl;
extern VME* clA;

void UnConnect( void);
void Connect(char* host,int size);
int ReadWordExt12(ULONG adr,UWORD AModifier);     /* get word from arbitrary address */
int ReadWordExt(ULONG adr,UWORD AModifier);     /* get word from arbitrary address */
int ReadDimExt12(ULONG adr, word nwords, short int *Data);     /* get word from arbitrary address */
int ReadDimExt(ULONG adr, word nwords, int *Data);     /* get word from arbitrary address */
void WriteWordExt(ULONG adr, int Data,UWORD AModifier);


#endif // VMELIB_H
