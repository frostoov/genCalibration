//====================================================================
#include "getsumpls.c"
#include <vector>
//====================================================================

int OpenNewFile(unsigned long nnn, int por, int flag );
//void SaveData ( byte Claster, byte Bank, int event, int add_time, int por );
void SaveData ( byte Claster, byte Bank, int event, int por );
int ReadAmplAnalizator ( byte Claster );
void SaveTime ( byte Claster, int event );
//====================================================================
int OpenNewFile(unsigned long nnn, int por, int flag )
{
FILE *fsub;
int Claster=0;
char fname[80],hname[80];
char st[10];
unsigned long mmm=0L;
int ii;
int key=0;
char *path;



//if ( (flag&0x3)==0x1 ) path="/home/baikal/t133/vme-inet/DATA/";  //  measurements
//if ( (flag&0x3)==0x1 ) path="/home/DATA/T133/";  //  measurements

//if ( (flag&0x3)==0x3 ) path="/home/baikal/t133/vme-inet/DATA/leds/";  // leds
//if ( (flag&0x3)==0x3 ) path="/home/DATA/T133/leds/";  // leds



//path="./data/";  //  measurements



/*

if ( (flag&0x3)==0x2 ) {   // oscillograph

    strcpy(fname,path);
    strcpy(hname,path);

    strcat(fname,"aaa.dat");
    strcat(hname,"hist.dat");

   for ( ii=0; ii<NCLASTREADY; ii++ ) {

	Claster = ClasterReady[ii];

	strcpy(filename[Claster],fname);
	strcpy(histname[Claster],hname);

	fsub = fopen ( filename[Claster],"w");
	fclose(fsub);
	fsub = fopen ( histname[Claster],"w");
	fclose(fsub);
    }
    return 0;

//#ifndef TEST_MODE
//#define TEST_MODE
//#endif

}

*/


   for ( ii=0; ii<NCLASTREADY; ii++ ) {

	Claster = ClasterReady[ii];

	do {

	    mmm = nnn*1000L;
	    mmm += (unsigned long)por;
//	    printf("nnn=%ld   mmm=%ld   por=%i    path=%s\n",nnn,mmm,por,pathfile);

	    strcpy(filename[Claster],pathfile);

	    strcpy(txtname[Claster],pathfile);
	    strcat(txtname[Claster],"txt//");

	    strcpy(histname[Claster],pathfile);
	    strcat(histname[Claster],"hist//");


		if ( mmm<=9999999L ) sprintf(st,"0%7ld",mmm);
		else          sprintf(st,"%8ld",mmm);
		strcat(filename[Claster],st);
		strcat(txtname[Claster],st);
		strcat(histname[Claster],st);

		strcat(filename[Claster],".por");
		sprintf(st,".C%02d",Claster+1);
		strcat(txtname[Claster],st);
		sprintf(st,".H%02d",Claster+1);
		strcat(histname[Claster],st);

//		printf("filename = %s    por=%i\n",filename[Claster],por);
//		printf("filename = %s    por=%i\n",txtname[Claster],por);
//		printf("filename = %s    por=%i\n",histname[Claster],por);


//#ifdef TEST_MODE
//	fsub = fopen ( fname,"w");
//	fclose(fsub);
//	key=1;

//#else

	if ( ( fsub = fopen ( filename[Claster],"r") ) != NULL  ) {
          printf("filename = %s    por=%i  This file is already \n",filename[Claster],por);
		fclose(fsub);
		por++;
		key=0;
	}

	else {
	    key=1;
	}

//#endif

//		} while ( ( fsub = fopen ( fname,"r") ) != NULL );
//		} while ( fsub != NULL );
	} while ( !key );

    }  // Claster


/*
   for ( ii=0; ii<NCLASTREADY; ii++ ) {

	Claster = ClasterReady[ii];

	mmm = nnn*1000L;
	mmm += (unsigned long)por;
//   printf("nnn=%ld   mmm=%ld   por=%i\n",nnn,mmm,por);

		strcpy(fname,path);
		strcpy(hname,path);
//		strcat(hname,"hist\\");

		if ( mmm<=9999999L ) sprintf(st,"0%7ld",mmm);
		else          sprintf(st,"%8ld",mmm);

//		if ( mmm<=9999999L ) sprintf(st,"%03d",por);
//		else          sprintf(st,"%03d",por);
//		strcat(fname,st);
//		if ( Claster<9 ) sprintf(st,"_C0%1i",Claster+1);
//		else              sprintf(st,"_C%2i",Claster+1);


		strcat(fname,st);
		strcat(hname,st);


		if ( Claster<9 ) sprintf(st,".C0%1i",Claster+1);
		else              sprintf(st,".C%2i",Claster+1);
		strcat(fname,st);



//		strcat(fname,".dat");

		if ( Claster<9 ) sprintf(st,".H0%1i",Claster+1);
		else              sprintf(st,".H%2i",Claster+1);
		strcat(hname,st);

		strcpy(filename[Claster],fname);
		strcpy(histname[Claster],hname);

//printf("filename = %s    por=%i\n",fname,por);
//printf("filename = %s    por=%i\n",hname,por);


//	printf("Open NEW file = %s\n",filename[Claster]);
//	fsub = fopen ( filename[Claster],"w");
//	fclose(fsub);

	}

//   printf("nnn=%ld   mmm=%ld   por=%i\n",nnn,mmm,por);




	strcpy(filetime,path);
	if ( nnn<=9999L ) sprintf(st,"0%4ld",nnn);
	else          sprintf(st,"%5ld",nnn);
	strcat(filetime,st);
	strcat(filetime,".time");
*/

return por;
}
//====================================================================
//void SaveData ( byte Claster, byte Bank, unsigned long event, int add_time, int por )
void SaveData ( byte Claster, byte Bank, unsigned long event, int por )
{
FILE *fsub;
int PlataNum,j;

struct data_struct {
	byte hour,min,sec;
	word mlsec;
	unsigned long Nown;
	byte Bank;
	word h,m,s,mls,mks,dns;
	short int pls [NPLAT] [Aperture*NPIN];
} DATA;

float SumPulse[NPLAT*NPIN];


//	printf("size of DATA=%i\n",sizeof(DATA));

	ftime(&Now);
	t = *localtime(&Now.time);

	DATA.hour = t.tm_hour;
	DATA.min = t.tm_min;
	DATA.sec = t.tm_sec;
	DATA.mlsec = 0;			// for DOS
	DATA.mlsec = Now.millitm;	// for LINUX

	DATA.Nown = (unsigned long)event;
//	DATA.Nnet = (unsigned long)add_time;

	GetTimeEvent (Claster,Bank);  // master.c
	
	DATA.Bank = Bank;

//	printf("Cluster=%02d    Bank=%i   LocalEvent=%5ld    %02d:%02d:%02d.%03d.%03d.%03d\n", 
//				Claster+1,Bank+1,DATA.Nown,h,m,s,mls,mks,dns);


	dns = dns*10;
//	printf("Cl#%i add=%i  ",Claster+1, add_time);
//	printf("Bank=%i     ",Bank);
//	printf ("Before add:   h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", h,m,s,mls,mks,dns);
	while (dns>=1000) {
//	printf ("Before add:   h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", h,m,s,mls,mks,dns);
		mks += 1;
		dns -= 1000;
	};
	while (mks>=1000) {
//	printf ("Before add:   h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", h,m,s,mls,mks,dns);
		mls += 1;
		mks -= 1000;
	};
	while (mls>=1000) {
//	printf ("Before add:   h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", h,m,s,mls,mks,dns);
		s += 1;
		mls -= 1000;
	};
	if ( s >= 60 ) {
		m += 1;
		s -= 60;
	}
	if ( m >= 60 ) {
		h += 1;
		m -= 60;
	}
	if ( h >= 24 ) {
		h -= 24;
	}



	DATA.h = h;
	DATA.m = m;
	DATA.s = s;
	DATA.mls = mls;
	DATA.mks = mks;
	DATA.dns = dns;



	fftime=fopen(timefile,"at");
	fprintf(fftime,"Cluster=%02d     LocalEvent=%5ld    Bank=%i   %02d:%02d:%02d.%03d.%03d.%03d\n", 
		Claster+1,DATA.Nown,DATA.Bank+1,h,m,s,mls,mks,dns);
//	printf("Cluster=%02d     LocalEvent=%5ld    %02d:%02d:%02d.%03d.%03d.%03d\n", 
//		Claster+1,DATA.Nown,h,m,s,mls,mks,dns);
	fclose(fftime);

// GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGg
//
	printf("Cluster=%02d    Bank=%i   LocalEvent=%5ld    %02d:%02d:%02d.%03d.%03d.%03d\n", 
		Claster+1,Bank+1,DATA.Nown,h,m,s,mls,mks,dns);
//
// GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGg


//	printf ("%02d:%02d:%02d.%03d.%03d.%03d\n", h,m,s,mls,mks,dns);
//	printf ("h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", h,m,s,mls,mks,dns);
//	printf ("h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", 
//		DATA.h,DATA.m,DATA.s,DATA.mls,DATA.mks,DATA.dns);

//	ftime(&Now);
//	 printf("Start GetPulseForm:   ");printf(ctime(&Now.time));

	for ( PlataNum=0; PlataNum<NPLAT; PlataNum++ ) {
//		GetPulseForm ( Claster, PlataNum, Bank, DATA.pls[PlataNum] );
	    if ( !DEFECT[Claster][PlataNum][0] )
		GetPulseFormDim ( Claster, PlataNum, Bank, DATA.pls[PlataNum] );  //flash_adc.c

	    if ( DEFECT[Claster][PlataNum][0] ) {
		    for (j=0; j<Aperture*4; j++) {
			DATA.pls[PlataNum][j]=0;
		    }
	    }
	}


//-------------------  SAVE BINARY FILE
	fsub = fopen (filename[Claster],"ab");
	fwrite(&DATA, sizeof(DATA), 1, fsub);
	fclose(fsub);
//-------------------  SAVE BINARY FILE


//-------------------  SAVE TEXT FILE
//    if ( por==1 || !(por%10) ) {

	fsub = fopen (txtname[Claster],"at");

	fprintf ( fsub, "%02d:%02d:%02d.%03d",  // save time of event
			 DATA.hour, DATA.min, DATA.sec, DATA.mlsec  );
	fprintf ( fsub, "%9ld%9ld",  // save numbers of event
//			 DATA.Nown, DATA.Nnet );
			 DATA.Nown, (longw)Bank+1 );
//			 DATA.Nown, (longw)add_time );
	fprintf ( fsub, "   %02d:%02d:%02d.%03d.%03d.%03d",  // save self time of event
			 DATA.h, DATA.m, DATA.s, DATA.mls, DATA.mks, DATA.dns );
	fprintf ( fsub, "\n" );

		for (j=0; j<Aperture; j++)	{
			fprintf(fsub, "%4i    ",j);
			for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
				fprintf (fsub, "%6i%6i%6i%6i   ",
				DATA.pls[PlataNum][j], DATA.pls[PlataNum][j+1024],
				DATA.pls[PlataNum][j+2048], DATA.pls[PlataNum][j+3072]);
			}
			fprintf(fsub,"\n");
		}


	/*
	for (PlataNum=0; PlataNum<NPLAT; PlataNum++) {
		for ( ipin=0; ipin<4; ipin++) {
		 for ( j=Aperture*ipin; j<Aperture*(ipin+1); j++) {
			 fprintf ( fsub, "%5i", DATA.pls [PlataNum] [j] );
		 }
		 fprintf ( fsub, "\n" );
		}
	}
	fprintf ( fsub, "\n" );
*/
	fclose(fsub);
//}
//-------------------  SAVE TEXT FILE

//--------------------  SAVE SUMMA PULSE
	    GetSummaPulse(Claster,DATA.pls, SumPulse);  // getsumpls.c
    fsub = fopen(summfile,"a");
    fprintf ( fsub, "%9ld", DATA.Nown );
	    for(j=0;j<NPIN;j++)
		fprintf ( fsub, "%11.1f", SumPulse[j] );
    fprintf ( fsub, "\n" );
    fclose(fsub);
//--------------------  SAVE SUMMA PULSE

}
//====================================================================
//============================================================
int ReadAmplAnalizator ( byte Claster )
{
byte PlataNum=0;
int j;
FILE *fsub;

struct data_struct {
    int pls [4] [Aperture*2];
} HIST;


    printf("\n!!!!!!  Time for histogramm - ");  printf(ctime(&Now.time));

    for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
//	GetHistogram ( Claster, PlataNum, HIST.pls[PlataNum] );
	    if ( DEFECT[Claster][PlataNum][0] == 0 ) {
//	printf("Plata=%i   Defect=%i\n",PlataNum+1,DEFECT[Claster][PlataNum][0]);

	    GetHistogramDim ( Claster, PlataNum, HIST.pls[PlataNum] );  //flash_adc.c
	}

	    if ( DEFECT[Claster][PlataNum][0] ==1) {
//	printf("!!!!!!!!! Plata=%i   Defect=%i\n",PlataNum+1,DEFECT[Claster][PlataNum][0]);
		    for (j=0; j<Aperture*2; j++) {
			HIST.pls[PlataNum][j]=0;
		    }
	    }
    }

    printf("Histogramm filename = %s\n",histname[Claster]);

//    fsub = fopen (histname[Claster],"ab");
//    fwrite(&HIST, sizeof(HIST), 1, fsub);

    fsub = fopen (histname[Claster],"at");
    for ( j=512; j<Aperture; j++) {
	
	fprintf( fsub,"%4i", j-512);
	for (PlataNum=0; PlataNum<NPLAT; PlataNum++) {
	    fprintf ( fsub, " %6i%6i", HIST.pls[PlataNum][j], HIST.pls[PlataNum][j+Aperture] );
	}
	fprintf ( fsub, "\n" );
    }

    fclose(fsub);

return 1;
}
//============================================================
//============================================================
//void SaveTime ( byte Claster, byte Bank, int event, int add_time )
void SaveTime ( byte Claster, int event )
{
FILE *fsub;
//char *fname;
//char fname[80];
byte PlataNum,ipin;
int j;
int ii;
int jj;
//int portion=0;


int h1,m1,s1,mls1;

//int delay;

/*
	gettime(&t);
   DATA.hour = t.ti_hour;
   DATA.min = t.ti_min;
   DATA.sec = t.ti_sec;
	DATA.mlsec = t.ti_hund;
*/

	printf ("Event=%i         ", event);

	ftime(&Now);
	t = *localtime(&Now.time);


	if ( event==1 ) {
	h0 = t.tm_hour;
	m0 = t.tm_min;
	s0 = t.tm_sec;
	mls0 = Now.millitm;

	tcomp0 = (long)t.tm_hour*3600L*1000L +
				(long)t.tm_min*60L*1000L +
				(long)t.tm_sec*1000L +
				(long)Now.millitm;

	}
//	printf ("h=%i  m=%i  s=%i  mls=%i\n", h0,m0,s0,mls0);

	tcomp1 = (long)t.tm_hour*3600L*1000L +
				(long)t.tm_min*60L*1000L +
				(long)t.tm_sec*1000L +
				(long)Now.millitm;
	tcomp1 -= tcomp0;
	printf ("dT comp=%ld         ", tcomp1);

//	printf ("h=%i  m=%i  s=%i  mls=%i\n", DATA.hour,DATA.min,DATA.sec,DATA.mlsec);

	h1 = t.tm_hour-h0;
	m1 = t.tm_min-m0;
	s1 = t.tm_sec-s0;
	mls1 = Now.millitm-mls0;   // for LINUX
	if ( mls1<0 ) {
		s1 -= 1;
		mls1 += 1000;
	}
	if ( s1<0 ) {
		m1 -= 1;
		s1 += 60;
	}
	if ( m1<0 ) {
	  h1 -= 1;
	  m1 += 60;
	}

//	DATA.mlsec = 0;             // for DOS
//	printf ("h=%i  m=%i  s=%i  mls=%i\n", DATA.hour,DATA.min,DATA.sec,DATA.mlsec);

	GetTimer (Claster);

	if (event==1 ) {
	tclast0 = (long)h*3600L*1000L +
				(long)m*60L*1000L +
				(long)s*1000L +
				(long)mls;
	}
	tclast1 = (long)h*3600L*1000L +
				(long)m*60L*1000L +
				(long)s*1000L +
				(long)mls;
	tclast1 -= tclast0;
	printf ("dT clast=%ld\n", tclast1);



	dns = dns*10;

//	printf("Bank=%i     ",Bank);
//	printf ("h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", h,m,s,mls,mks,dns);

/*
	dns += add_time;

	if (dns > 1000 ) {
		mks += 1;
		dns -= 1000;
	}

	if (Claster==0 ) dat1 = (long)mls*1000000L+(long)mks*1000L+(long)dns;
	if (Claster==1 ) dat2 = (long)mls*1000000L+(long)mks*1000L+(long)dns;
*/


//	printf("Bank=%i     ",Bank);
//	printf ("h=%i  m=%i  s=%i  mls=%i  mks=%i  dns=%i\n", h,m,s,mls,mks,dns);

	fsub = fopen ("dtime.dat","at");
//	fsub = fopen (filename[Claster],"wt");

	fprintf (fsub,"%4i", event);
	fprintf (fsub,"%13ld", tcomp1);
	fprintf (fsub,"%13ld\n", tclast1);


/*
	fprintf ( fsub, "%02d:%02d:%02d.%03d",  // save time of event
			 DATA.hour, DATA.min, DATA.sec, DATA.mlsec  );
	fprintf ( fsub, "%9ld%9ld",  // save numbers of event
			 DATA.Nown, DATA.Nnet );
	fprintf ( fsub, "   %02d:%02d:%02d.%03d",  // save self time of event
			 DATA.h, DATA.m, DATA.s, DATA.mls );
	fprintf ( fsub, "\n" );
*/

	fclose(fsub);

}

void SaveData(unsigned char Claster, unsigned char Bank, int event, int por)
{
	SaveData((byte) Claster,(byte)Bank, event, por);
}

//====================================================================
//====================================================================
