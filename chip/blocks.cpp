#include "blocks.h"

void endMainBlock()
{
	//	  =1 -  external trigger
	//	  =2 -  programm's trigger
	//	  =3 -  self trigger

	int flag_STOP=0;

	//	 ----------------  END  MAIN CYCLE  -----------
	ftime(&Now);
	printf("Stop at - ");
	printf("%s", ctime(&Now.time));
	printf("flag stop = %i\n", flag_STOP);

	ffstat = fopen(statfile,"a");
	fprintf(ffstat,"\n\n  Stop Data Taking ---> ");
	fprintf(ffstat, "%s", ctime(&Now.time));
	fclose(ffstat);

	fferr = fopen(errfile,"a");
	fprintf(fferr,"\n\n  Stop Data Taking ---> ");
	fprintf(fferr, "%s", ctime(&Now.time));
	fclose(fferr);
}

std::vector<int> SaveDataResult(byte Claster, byte Bank, int event)
{
	int PlataNum,j;

	struct data_struct {
		byte hour,min,sec;
		word mlsec;
		unsigned long Nown;
		byte Bank;
		word h,m,s,mls,mks,dns;
		short int pls [NPLAT] [Aperture*NPIN];
	} DATA;

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

	for ( PlataNum=0; PlataNum<NPLAT; PlataNum++ ) {
		if ( !DEFECT[Claster][PlataNum][0] )
			GetPulseFormDim ( Claster, PlataNum, Bank, DATA.pls[PlataNum] );  //flash_adc.c

		if ( DEFECT[Claster][PlataNum][0] ) {
			for (j=0; j<Aperture*4; j++) {
				DATA.pls[PlataNum][j]=0;
			}
		}
	}
	std::vector<int> max(4);

	for (j=0; j<Aperture; j++)
	{
		//					fprintf(fsub, "%4i    ",j);
		//					for(PlataNum=0; PlataNum<NPLAT; PlataNum++)
		//					{
		//						fprintf (fsub, "%6i%6i%6i%6i   ",
		//						DATA.pls[PlataNum][j], DATA.pls[PlataNum][j+1024],
		//						DATA.pls[PlataNum][j+2048], DATA.pls[PlataNum][j+3072]);
		//					}
		if (max[0] < DATA.pls[0][j])
			max[0] = DATA.pls[0][j];
		if (max[1] < DATA.pls[0][j + 1024])
			max[1] = DATA.pls[0][j + 1024];
		if (max[2] < DATA.pls[0][j + 2048])
			max[2] = DATA.pls[0][j + 2048];
		if (max[3] < DATA.pls[0][j + 3072])
			max[3] = DATA.pls[0][j + 3072];
	}

	return max;
	//--------------------  SAVE SUMMA PULSE
}

std::vector<int> sniffMainBlockThree(unsigned long nnn, int countSignals)
{
	std::vector<int> max(4);
	int TRIGGER=1;
	//	  =1 -  external trigger
	//	  =2 -  programm's trigger
	//	  =3 -  self trigger

	unsigned long event=0,ev=0;
	int por=1;
	int ii=0,jj=0;
	byte Claster=0;
	byte PlataNum=0;

	int coin=0;
	int Data=0;
	unsigned long Nev[NCLAST];

	time_t this_moment, last_moment;

	int cl=0;
	int slu=0;
	int StopRun=0;
	int flag_STOP=0;
	int goSignals = 0;

	unsigned long NEVENTS=0L;

	// ------------------   TRIGGER   --------------------

	for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];

		for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {

			if ( DEFECT[Claster][PlataNum][0] ) continue;

			if ( TRIGGER == 1 )  //  master.c
				SetTriggerExternal ( Claster, PlataNum );	// extra trigger
			if ( TRIGGER == 2 )
				SetTriggerProgramm ( Claster, PlataNum );	// programm's trigger
			if ( TRIGGER == 3 )
				SetTriggerSelf ( Claster, PlataNum );	// self trigger
		}
	}
	// ------------------   TRIGGER   --------------------

	// -----------------  Coinsidence  -------------------

	for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];
		coin = COINS[Claster];
		SetCoinsidence ( Claster, coin);
		coin = 0;
		coin =	GetCoinsidence ( Claster );  // master.c
		printf("Claster %i  ++++++++++++   Coinsidence = %x   +++++++++++\n",
			   Claster+1,coin);
	}
	// -----------------  Coinsidence  -------------------

	// ----------------   RESET   -------------------

	for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];

		ResetEvent ( Claster, 0 );   //  master.c
		ResetEvent ( Claster, 1 );

		for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {

			if ( !DEFECT[Claster][PlataNum][0] )
				ResetHIST ( Claster, PlataNum, 0 );  //flash_adc.c
			if ( !DEFECT[Claster][PlataNum][1] )
				ResetHIST ( Claster, PlataNum, 1 );
		}
	}
	// ----------------   RESET   -------------------

	// ---------------   Program TRIGGER   --------------------

	if ( TRIGGER == 2 ) {
		for ( ii=0; ii<NCLASTREADY; ii++) {
			Claster = ClasterReady[ii];
			for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
				if ( DEFECT[Claster][PlataNum][0] ) continue;
				SetProgramTrigger (Claster, PlataNum);  // master.c
			}
		}
	}
	// ---------------   Program TRIGGER   --------------------
	// ----------------   ZERO DIMENSION   -------------------

	for(ii=0; ii<NCLAST; ii++) {
		Nev[ii] = 0;
	}

	por = OpenNewFile(nnn,por);  // savedata.c

	this_moment = time(NULL);
	last_moment = time(NULL);

	event = 0;
	ev = 0;
	ii = 0;
	slu=0;
	cl=0;
	StopRun=0;
	// ----------------   ZERO DIMENSION   -------------------


	// ----------------   MAIN CYCLE  -----------

	clock_t	time = clock();

	do {
		ii = 0;
		Claster = ClasterReady[ii];

		Data = 0;

		Data = GetStatusEvent ( Claster );

		if (clock() - time > 10000)
			return max;
		//  ------------   SAVE DATA and RESET ----------

		if ( ( ( Data & 1 ) == 1 ) || ( ( Data & 2 ) == 2 ) ) {
			slu = 1;   // !!!   Event Ready   !!!

			if ( ( Data & 2 ) == 2 ) {   //  Bank 2
				Nev[Claster] ++;
				max = SaveDataResult(Claster, 1, Nev[Claster]);
				ResetEvent (Claster, 1 );  // master.c
				NEVENTS++;
				goSignals++;
				if (countSignals == goSignals)
					StopRun = 1;
			}

			// -------------   Programm trigger   ----------------------
			if ( TRIGGER == 2 ) {
				for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
					if ( DEFECT[Claster][PlataNum][0] ) continue;
					SetProgramTrigger ( Claster, PlataNum );  // master.c
				}
			}
			// -------------   Programm trigger   ----------------------

		}

		//--------------  END   SAVE DATA and RESET -------------


		//--------------   SAVE HIST and RESET  ------------------------

		ftime(&Now);   // Get Time and Date ( DOS and LINUX )

		if (this_moment>last_moment+TimePerHistogramm ) {

			printf("Histogramm Ready  -  ");
			printf("%s", ctime(&Now.time));

			for(jj=0; jj<NCLASTREADY;jj++) {
				ReadAmplAnalizator( ClasterReady[jj]);  //savedata.c
				for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
					if ( DEFECT[ClasterReady[jj]][PlataNum][0] ) continue;
					ResetHIST ( ClasterReady[jj], PlataNum, 0 );  // flash_adc.c
					ResetHIST ( ClasterReady[jj], PlataNum, 1 );
				}
			}
			last_moment = this_moment;
		}

		this_moment = Now.time;
		//--------------   END   SAVE HIST and RESET  ------------------------

		if ( slu ) cl++;
		if ( cl == NCLASTREADY ) {
			event++;
			if ( event >= EventsPerEndMeasure )	{
				StopRun=1;
			}
			if ( StopRun ) continue;
			if ( event && !(event % EventsPerFile) ) {  // open new file
				por++;
				por = OpenNewFile(nnn,por);
			}
			slu=0;
			cl=0;
		}


		ii ++;
		if ( ii == NCLASTREADY ) ii = 0;


		if ( !StopRun ) {
			StopRun = GetStopRun();  //  stoprun.c
			if ( StopRun ) flag_STOP = 1;
		}

	} while ( (NEVENTS <EventsPerEndMeasure) && !StopRun );

	// ----------------  END  MAIN CYCLE  -----------

	return max;
}

//====================================================================
//====================================================================
//====================================================================
//====================================================================

