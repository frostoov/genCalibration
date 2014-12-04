//==============================================================
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
unsigned char get_single_bit (unsigned char sym, int bit)
{
	sym=sym<<(7-bit);
	sym=sym>>7;
	return sym;
}
//==============================================================
void Two_char (	unsigned int twobit,
		unsigned char *bitHigh, unsigned char *bitLow )
{
	unsigned int word;

	*bitLow = (unsigned char) twobit;
	word = twobit >> 8;
	*bitHigh = (unsigned char) word;
}
//==============================================================
void Four_char (unsigned long fourbit,
		unsigned char *bitHH, unsigned char *bitHL,
		unsigned char *bitLH, unsigned char *bitLL )
{
	unsigned long wordlong;
	unsigned int twobitL,twobitH, word;

	twobitL = (unsigned int) fourbit;
	wordlong = fourbit >> 16;
	twobitH = (unsigned int) wordlong;

	*bitHL = (unsigned char) twobitH;
	word = twobitH >> 8;
	*bitHH = (unsigned char) word;

	*bitLL = (unsigned char) twobitL;
	word = twobitL >> 8;
	*bitLH = (unsigned char) word;
}
//==============================================================
/*
void print_in_bits (unsigned char sym)
{
unsigned char b0,b1,b2,b3,b4,b5,b6,b7;

	b0=get_single_bit(sym,0);
	b1=get_single_bit(sym,1);
	b2=get_single_bit(sym,2);
	b3=get_single_bit(sym,3);
	b4=get_single_bit(sym,4);
	b5=get_single_bit(sym,5);
	b6=get_single_bit(sym,6);
	b7=get_single_bit(sym,7);
	printf("             8  7  6  5  4  3  2  1\n");
	printf("%10i = %i  %i  %i  %i  %i  %i  %i  %i\n",
			sym,b7,b6,b5,b4,b3,b2,b1,b0);
}
*/
//==============================================================
void print_in_bits (unsigned long sym)
{
unsigned char b0,b1,b2,b3,b4,b5,b6,b7;
unsigned char b8,b9,b10,b11,b12,b13,b14,b15;
unsigned char b16,b17,b18,b19,b20,b21,b22,b23;
unsigned char b24,b25,b26,b27,b28,b29,b30,b31;
unsigned char HH,HL;
unsigned char LH,LL;

Four_char ( sym, &HH, &HL, &LH, &LL );

	b0=get_single_bit(LL,0);
	b1=get_single_bit(LL,1);
	b2=get_single_bit(LL,2);
	b3=get_single_bit(LL,3);
	b4=get_single_bit(LL,4);
	b5=get_single_bit(LL,5);
	b6=get_single_bit(LL,6);
	b7=get_single_bit(LL,7);

	b8 =get_single_bit(LH,0);
	b9 =get_single_bit(LH,1);
	b10=get_single_bit(LH,2);
	b11=get_single_bit(LH,3);
	b12=get_single_bit(LH,4);
	b13=get_single_bit(LH,5);
	b14=get_single_bit(LH,6);
	b15=get_single_bit(LH,7);

	b16=get_single_bit(HL,0);
	b17=get_single_bit(HL,1);
	b18=get_single_bit(HL,2);
	b19=get_single_bit(HL,3);
	b20=get_single_bit(HL,4);
	b21=get_single_bit(HL,5);
	b22=get_single_bit(HL,6);
	b23=get_single_bit(HL,7);

	b24=get_single_bit(HH,0);
	b25=get_single_bit(HH,1);
	b26=get_single_bit(HH,2);
	b27=get_single_bit(HH,3);
	b28=get_single_bit(HH,4);
	b29=get_single_bit(HH,5);
	b30=get_single_bit(HH,6);
	b31=get_single_bit(HH,7);

/*
	printf("            32 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9   8  7  6  5  4  3  2  1\n");
	printf("%10ld = %i  %i  %i  %i  %i  %i  %i  %i",sym,b31,b30,b29,b28,b27,b26,b25,b24);
	printf("  %i  %i  %i  %i  %i  %i  %i  %i",b23,b22,b21,b20,b19,b18,b17,b16);
	printf("  %i  %i  %i  %i  %i  %i  %i  %i",b15,b14,b13,b12,b11,b10,b9,b8);
	printf("  %i  %i  %i  %i  %i  %i  %i  %i\n",b7,b6,b5,b4,b3,b2,b1,b0);
*/
//        printf("\n");
//        printf("                HH        HL        LH        LL\n");
        printf("               32-24      23-16      15-8        7-0\n");
	printf("%10lX = %i%i%i%i %i%i%i%i",sym,b31,b30,b29,b28,b27,b26,b25,b24);
	printf("  %i%i%i%i %i%i%i%i",b23,b22,b21,b20,b19,b18,b17,b16);
	printf("  %i%i%i%i %i%i%i%i",b15,b14,b13,b12,b11,b10,b9,b8);
	printf("  %i%i%i%i %i%i%i%i",b7,b6,b5,b4,b3,b2,b1,b0);
        printf("\n");
//	printf("\n\n");

}
//==============================================================
