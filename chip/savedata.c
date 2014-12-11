#include "getsumpls.c"
int OpenNewFile(unsigned long nnn, int por);
void SaveData ( byte Claster, byte Bank, int event, int por );
int ReadAmplAnalizator ( byte Claster );
void SaveTime ( byte Claster, int event );
//====================================================================
int OpenNewFile(unsigned long nnn, int por )
{
	FILE *fsub;
	int Claster=0;
	char st[10];
	unsigned long mmm=0L;
	int ii;
	int key=0;
	for ( ii=0; ii<NCLASTREADY; ii++ ) {

		Claster = ClasterReady[ii];

		do {

			mmm = nnn*1000L;
			mmm += (unsigned long)por;
			strcpy(filename[Claster],pathfile);
			strcpy(txtname[Claster],pathfile);
			strcat(txtname[Claster],"txt//");
			strcpy(histname[Claster],pathfile);
			strcat(histname[Claster],"hist//");
			if ( mmm<=9999999L )
				sprintf(st,"0%7ld",mmm);
			else
				sprintf(st,"%8ld",mmm);
			strcat(filename[Claster],st);
			strcat(txtname[Claster],st);
			strcat(histname[Claster],st);
			strcat(filename[Claster],".por");
			sprintf(st,".C%02d",Claster+1);
			strcat(txtname[Claster],st);
			sprintf(st,".H%02d",Claster+1);
			strcat(histname[Claster],st);
			if ( ( fsub = fopen ( filename[Claster],"r") ) != NULL  )
			{
				printf("filename = %s    por=%i  This file is already \n",filename[Claster],por);
				fclose(fsub);
				por++;
				key=0;
			}
			else {
				key=1;
			}
		} while ( !key );

	}  // Claster
	return por;
}
//====================================================================
void SaveData ( byte Claster, byte Bank, unsigned long event )
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

	ftime(&Now);
	t = *localtime(&Now.time);

	DATA.hour = t.tm_hour;
	DATA.min = t.tm_min;
	DATA.sec = t.tm_sec;
	DATA.mlsec = 0;			// for DOS
	DATA.mlsec = Now.millitm;	// for LINUX

	DATA.Nown = (unsigned long)event;
	GetTimeEvent (Claster,Bank);  // master.c
	DATA.Bank = Bank;
	dns = dns*10;
	while (dns>=1000) {
		mks += 1;
		dns -= 1000;
	};
	while (mks>=1000) {
		mls += 1;
		mks -= 1000;
	};
	while (mls>=1000) {
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
	fclose(fftime);

	printf("Cluster=%02d    Bank=%i   LocalEvent=%5ld    %02d:%02d:%02d.%03d.%03d.%03d\n",
		   Claster+1,Bank+1,DATA.Nown,h,m,s,mls,mks,dns);
	for ( PlataNum=0; PlataNum<NPLAT; PlataNum++ ) {
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
	fsub = fopen (txtname[Claster],"at");

	fprintf ( fsub, "%02d:%02d:%02d.%03d",  // save time of event
			  DATA.hour, DATA.min, DATA.sec, DATA.mlsec  );
	fprintf ( fsub, "%9ld%9ld",  // save numbers of event
			  DATA.Nown, (longw)Bank+1 );
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
	fclose(fsub);
	//-------------------  SAVE TEXT FILE

	//--------------------  SAVE SUMMA PULSE
	GetSummaPulse(DATA.pls, SumPulse);  // getsumpls.c
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


	printf("\n!!!!!!  Time for histogramm - ");
	printf("%s", ctime(&Now.time));

	for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
		if ( DEFECT[Claster][PlataNum][0] == 0 ) {
			GetHistogramDim ( Claster, PlataNum, HIST.pls[PlataNum] );  //flash_adc.c
		}

		if ( DEFECT[Claster][PlataNum][0] ==1) {
			for (j=0; j<Aperture*2; j++) {
				HIST.pls[PlataNum][j]=0;
			}
		}
	}

	printf("Histogramm filename = %s\n",histname[Claster]);
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
void SaveTime ( byte Claster, int event )
{
	FILE *fsub;

	int h1,m1,s1,mls1;
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
	tcomp1 = (long)t.tm_hour*3600L*1000L +
			(long)t.tm_min*60L*1000L +
			(long)t.tm_sec*1000L +
			(long)Now.millitm;
	tcomp1 -= tcomp0;
	printf ("dT comp=%ld         ", tcomp1);
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
	fsub = fopen ("dtime.dat","at");
	fprintf (fsub,"%4i", event);
	fprintf (fsub,"%13ld", tcomp1);
	fprintf (fsub,"%13ld\n", tclast1);
	fclose(fsub);

}

void SaveData(unsigned char Claster, unsigned char Bank, int event, int por)
{
	SaveData((byte) Claster,(byte)Bank, event, por);
}

//====================================================================
//====================================================================
