//==============================================================

#include <stdio.h>

unsigned char get_single_bit (unsigned char sym, int bit);

void Two_char (	unsigned int twobit,
		unsigned char *bitHigh, unsigned char *bitLow );

void Four_char (unsigned long fourbit,
		unsigned char *bitHH, unsigned char *bitHL,
		unsigned char *bitLH, unsigned char *bitLL );

void print_in_bits (unsigned long sym);

//==============================================================
//==============================================================
//==============================================================
//==============================================================
