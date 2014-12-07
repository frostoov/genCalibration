#include "blocks.h"
#include <thread>

exchangeBlock startMainBlock (unsigned long nnn, int *Threshold, int flag )
{
	int TRIGGER=1;
	//	  =1 -  external trigger
	//	  =2 -  programm's trigger
	//	  =3 -  self trigger

	int key=0;
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

	unsigned long NEVENTS=0L;


//	printf("Press <s> for start measurements\n");
//	do {
//		key = getchar();
//	} while ( key != 115 && key != 27 );
//	if ( key==27 ) return -27;


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

	// -----------------  Threshold  -------------------

	for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];
		printf("Claster=%i\n",Claster+1);
		SetThresholdWithBaseLine ( Claster, Threshold, 0 );  // thresholds.c
	}
	printf("\n");
	// -----------------  Threshold  -------------------

	/*
// -----------------  Block Request  -------------------
// -----------------  Block Request  -------------------
*/

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

	exchangeBlock result;
	result.Claster	= Claster;
	result.coin		= coin;
	result.flag_STOP= flag_STOP;
	result.ii		= ii;
	result.PlataNum	= PlataNum;
	result.TRIGGER	= TRIGGER;
	result.flag		= flag;
	result.nnn		= nnn;
	result.this_moment = this_moment;
	result.last_moment = last_moment;
	for (int i = 0; i < NCLAST; i++)
		result.Nev[i] = Nev[i];

	return result;
}

void endMainBlock()
{

	int TRIGGER=1;
	//	  =1 -  external trigger
	//	  =2 -  programm's trigger
	//	  =3 -  self trigger

	int key=0;
	unsigned long event=0,ev=0;
	int por=1;
	int ii=0,jj=0;
	byte Claster=0;
	byte PlataNum=0;

	int coin=0;
	int Data=0;
	unsigned long Nev[NCLAST];

	int cl=0;
	int slu=0;
	int StopRun=0;
	int flag_STOP=0;

	unsigned long NEVENTS=0L;

//	 ----------------  END  MAIN CYCLE  -----------
	ftime(&Now);   // Get Time
	printf("Stop at - ");
	printf(ctime(&Now.time));
	printf("flag stop = %i\n", flag_STOP);


	ffstat = fopen(statfile,"a");
	fprintf(ffstat,"\n\n  Stop Data Taking ---> ");
	fprintf(ffstat,ctime(&Now.time));
	fclose(ffstat);

	fferr = fopen(errfile,"a");
	fprintf(fferr,"\n\n  Stop Data Taking ---> ");
	fprintf(fferr,ctime(&Now.time));
	fclose(fferr);
}

std::vector<int> sniffMainBlock(const exchangeBlock& inputVars)
{
	std::vector<int> max;
	int TRIGGER				= inputVars.TRIGGER;
	//	  =1 -  external trigger
	//	  =2 -  programm's trigger
	//	  =3 -  self trigger

	unsigned long event		= 0;
	int por					= 1;
	int ii					= inputVars.ii;
	int jj					= 0;
	byte Claster			= inputVars.Claster;
	byte PlataNum			= inputVars.PlataNum;

	int coin				= inputVars.coin;
	int Data				= 0;
	unsigned long Nev[NCLAST];

	time_t this_moment = inputVars.this_moment;
	time_t last_moment = inputVars.last_moment;

	int cl					= 0;
	int slu					= 0;
	int StopRun				= 0;
	int flag_STOP			= inputVars.flag_STOP;

	int flag				= inputVars.flag;
	int nnn					= inputVars.nnn;

	unsigned long NEVENTS=0L;

	for (int i = 0; i < NCLAST; i++)
		Nev[i] = inputVars.Nev[i];


	do {
		//------------ Connect ERROR  --------------

		if ( clA->GetSync() ) {   //  vme.h

			clA->ClearSync();   //  vme.h

			fferr = fopen(errfile,"a");
			fprintf(fferr," ERROR Connect <---  ");
			fprintf(fferr,ctime(&Now.time));
			fclose(fferr);
			printf("ERROR Connect      ");
			printf(ctime(&Now.time));
		}
		//------------ Connect ERROR  --------------



		ii = 0;
		Claster = ClasterReady[ii];
		Data = 0;

		Data = GetStatusEvent ( Claster );  // master.c


		//  ------------   SAVE DATA and RESET ----------

		if ( ( ( Data & 1 ) == 1 ) || ( ( Data & 2 ) == 2 ) ) {

			slu = 1;   // !!!   Event Ready   !!!

			if ( ( Data & 1 ) == 1 ) {  //   Bank 1
				Nev[Claster] ++;
//				SaveData ( Claster, 0, Nev[Claster], por );  //savedata.c
				max = SaveDataResult(Claster, 0, Nev[Claster], por);
				ResetEvent (Claster, 0 );  // master.c
				NEVENTS++;
				StopRun = 1;
			}

			if ( ( Data & 2 ) == 2 ) {   //  Bank 2
				Nev[Claster] ++;
//				SaveData ( Claster, 1, Nev[Claster], por );  //savedata.c
				max = SaveDataResult(Claster, 1, Nev[Claster], por);
				ResetEvent (Claster, 1 );  // master.c
				NEVENTS++;
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

		}  // if ( ( ( Data & 1 ) == 1 ) || ( ( Data & 2 ) == 2 ) )

		//--------------  END   SAVE DATA and RESET -------------


		//--------------   SAVE HIST and RESET  ------------------------

		ftime(&Now);   // Get Time and Date ( DOS and LINUX )

//		if (this_moment>last_moment+TimePerHistogramm ) {

//			printf("Histogramm Ready  -  ");  printf(ctime(&Now.time));

//			for(jj=0; jj<NCLASTREADY;jj++) {
//				ReadAmplAnalizator( ClasterReady[jj]);  //savedata.c
//				for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
//					if ( DEFECT[ClasterReady[jj]][PlataNum][0] ) continue;
//					ResetHIST ( ClasterReady[jj], PlataNum, 0 );  // flash_adc.c
//					ResetHIST ( ClasterReady[jj], PlataNum, 1 );
//				}
//			}

//			last_moment = this_moment;
//		}

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
				por = OpenNewFile(nnn,por,flag);
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
	return max;
}

std::vector<int> SaveDataResult(byte Claster, byte Bank, int event, int por)
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



//		fftime=fopen(timefile,"at");
//		fprintf(fftime,"Cluster=%02d     LocalEvent=%5ld    Bank=%i   %02d:%02d:%02d.%03d.%03d.%03d\n",
//			Claster+1,DATA.Nown,DATA.Bank+1,h,m,s,mls,mks,dns);
//		fclose(fftime);

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
//		fsub = fopen (filename[Claster],"ab");
//		fwrite(&DATA, sizeof(DATA), 1, fsub);
//		fclose(fsub);
	//-------------------  SAVE BINARY FILE


	//-------------------  SAVE TEXT FILE
	//    if ( por==1 || !(por%10) ) {

//		fsub = fopen (txtname[Claster],"at");

//		fprintf ( fsub, "%02d:%02d:%02d.%03d",  // save time of event
//				 DATA.hour, DATA.min, DATA.sec, DATA.mlsec  );
//		fprintf ( fsub, "%9ld%9ld",  // save numbers of event
//				 DATA.Nown, (longw)Bank+1 );
//		fprintf ( fsub, "   %02d:%02d:%02d.%03d.%03d.%03d",  // save self time of event
//				 DATA.h, DATA.m, DATA.s, DATA.mls, DATA.mks, DATA.dns );
//		fprintf ( fsub, "\n" );

		//BEGIN
//			int max1 = 0, max2 = 0, max3 = 0, max4 = 0;
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
//					fprintf(fsub,"\n");
				}
	//			printf("%d   %d    %d    %d\n", max1, max2, max3, max4);
//			FILE* mainFile;
//			if (Bank == 0)
//				mainFile = fopen("/home/main/projects/output0", "w+");
//			else
//				mainFile = fopen("/home/main/projects/output1", "w+");
//			fprintf(mainFile, "%d       %d       %d       %d\n", max1, max2, max3, max4);
//			fclose(mainFile);
		//END

		//		for (j=0; j<Aperture; j++)	{
		//			fprintf(fsub, "%4i    ",j);
		//			for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
		//				fprintf (fsub, "%6i%6i%6i%6i   ",
		//				DATA.pls[PlataNum][j], DATA.pls[PlataNum][j+1024],
		//				DATA.pls[PlataNum][j+2048], DATA.pls[PlataNum][j+3072]);
		//			}
		//			fprintf(fsub,"\n");
		//		}
//		fclose(fsub);
	//-------------------  SAVE TEXT FILE

	//--------------------  SAVE SUMMA PULSE
//			GetSummaPulse(Claster,DATA.pls, SumPulse);  // getsumpls.c
//		fsub = fopen(summfile,"a");
//		fprintf ( fsub, "%9ld", DATA.Nown );
//			for(j=0;j<NPIN;j++)
//			fprintf ( fsub, "%11.1f", SumPulse[j] );
//		fprintf ( fsub, "\n" );
//		fclose(fsub);

	return max;
	//--------------------  SAVE SUMMA PULSE
}

std::vector<int> sniffMainBlockSignals(unsigned long nnn, int *Threshold, int flag , int countSignals)
{
	std::vector<int> max(4);
	int TRIGGER=1;
	//	  =1 -  external trigger
	//	  =2 -  programm's trigger
	//	  =3 -  self trigger

	int key=0;
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

	// -----------------  Threshold  -------------------

	for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];
		printf("Claster=%i\n",Claster+1);
		SetThresholdWithBaseLine ( Claster, Threshold, 0 );  // thresholds.c
	}
	printf("\n");
	// -----------------  Threshold  -------------------

	/*
// -----------------  Block Request  -------------------
//	for ( ii=0; ii<NCLASTREADY; ii++) {
//		Claster = ClasterReady[ii];
//		WriteStatusRegister(Claster, 1, 0,0,0, 1,0,1);
//	}
	for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];
		if ( Claster!=((flag>>6)-1) ) continue;
		for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
		if ( DEFECT[Claster][PlataNum][0] ) continue;
		BlockedChannel(Claster, PlataNum, 0);  // flash_adc.c
		BlockedChannel(Claster, PlataNum, 1);
		}
	}
// -----------------  Block Request  -------------------
*/

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

	/*
// ---------------   Delay Request   --------------------
printf("Set Delay Request (in codes)\n");
scanf("%i",&delay);
		for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];
		for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
		if ( DEFECT[Claster][PlataNum][0] ) continue;
		SetDelayRequest(Claster,PlataNum,1,delay);  // flash_adc.c
		}
		}
// ---------------   Delay Request   --------------------
*/


	// ----------------   ZERO DIMENSION   -------------------

	for(ii=0; ii<NCLAST; ii++) {
		Nev[ii] = 0;
	}

	por = OpenNewFile(nnn,por,flag);  // savedata.c

	this_moment = time(NULL);
	last_moment = time(NULL);

	event = 0;
	ev = 0;
	ii = 0;
	slu=0;
	cl=0;
	StopRun=0;

	ftime(&Now);   // Get Time
	ffstat = fopen(statfile,"at");
	fprintf(ffstat,"\n Data Taking Start : ");
	fprintf(ffstat,"   %02d-%02d-%02d %02d:%02d:%02d.%03d LT\n",
			t.tm_mday, t.tm_mon, t.tm_year,
			t.tm_hour, t.tm_min, t.tm_sec, Now.millitm);
	fprintf(ffstat,"\n");
	fclose(ffstat);

	fferr = fopen(errfile,"at");
	fprintf(fferr,"\n Data Taking Start : ");
	fprintf(fferr,"   %02d-%02d-%02d %02d:%02d:%02d.%03d LT\n",
			t.tm_mday, t.tm_mon, t.tm_year,
			t.tm_hour, t.tm_min, t.tm_sec, Now.millitm);
	fprintf(fferr,"\n");
	fclose(fferr);
	// ----------------   ZERO DIMENSION   -------------------


	// ----------------   MAIN CYCLE  -----------

	do {


		//------------ Connect ERROR  --------------

		if ( clA->GetSync() ) {   //  vme.h

			clA->ClearSync();   //  vme.h

			fferr = fopen(errfile,"a");
			fprintf(fferr," ERROR Connect <---  ");
			fprintf(fferr,ctime(&Now.time));
			fclose(fferr);
			printf("ERROR Connect      ");
			printf(ctime(&Now.time));
		}
		//------------ Connect ERROR  --------------


		ii = 0;
		Claster = ClasterReady[ii];

		Data = 0;

		Data = GetStatusEvent ( Claster );  // master.c


		//  ------------   SAVE DATA and RESET ----------

		if ( ( ( Data & 1 ) == 1 ) || ( ( Data & 2 ) == 2 ) ) {
			slu = 1;   // !!!   Event Ready   !!!

			if ( ( Data & 1 ) == 1 ) {  //   Bank 1
				Nev[Claster] ++;
//				SaveData ( Claster, 0, Nev[Claster], por );  //savedata.c
				max = SaveDataResult(Claster, 0, Nev[Claster], por);
				ResetEvent (Claster, 0 );  // master.c
				NEVENTS++;
				goSignals++;
				if (countSignals == goSignals / 2)
					StopRun = 1;
			}

			if ( ( Data & 2 ) == 2 ) {   //  Bank 2
				Nev[Claster] ++;
//				SaveData ( Claster, 1, Nev[Claster], por );  //savedata.c
				max = SaveDataResult(Claster, 1, Nev[Claster], por);
				ResetEvent (Claster, 1 );  // master.c
				NEVENTS++;
				goSignals++;
				if (countSignals == goSignals / 2)
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

		}  // if ( ( ( Data & 1 ) == 1 ) || ( ( Data & 2 ) == 2 ) )

		//--------------  END   SAVE DATA and RESET -------------


		//--------------   SAVE HIST and RESET  ------------------------

		ftime(&Now);   // Get Time and Date ( DOS and LINUX )

		if (this_moment>last_moment+TimePerHistogramm ) {

			printf("Histogramm Ready  -  ");  printf(ctime(&Now.time));

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
				por = OpenNewFile(nnn,por,flag);
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

	ftime(&Now);   // Get Time
	printf("Stop at - ");
	printf(ctime(&Now.time));
	printf("flag stop = %i\n", flag_STOP);


	ffstat = fopen(statfile,"a");
	fprintf(ffstat,"\n\n  Stop Data Taking ---> ");
	fprintf(ffstat,ctime(&Now.time));
	fclose(ffstat);

	fferr = fopen(errfile,"a");
	fprintf(fferr,"\n\n  Stop Data Taking ---> ");
	fprintf(fferr,ctime(&Now.time));
	fclose(fferr);
	return max;
}

std::vector<int> sniffMainBlockThree(unsigned long nnn, int flag, int countSignals)
{
		std::vector<int> max(4);
		int TRIGGER=1;
		//	  =1 -  external trigger
		//	  =2 -  programm's trigger
		//	  =3 -  self trigger

		int key=0;
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

		por = OpenNewFile(nnn,por,flag);  // savedata.c

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

//				if ( ( Data & 1 ) == 1 ) {  //   Bank 1
//					Nev[Claster] ++;
//	//				SaveData ( Claster, 0, Nev[Claster], por );  //savedata.c
////					max = SaveDataResult(Claster, 0, Nev[Claster], por);
//					ResetEvent (Claster, 0 );  // master.c
//					NEVENTS++;
////					goSignals++;
////					if (countSignals == goSignals / 2)
////						StopRun = 1;
//				}

				if ( ( Data & 2 ) == 2 ) {   //  Bank 2
					Nev[Claster] ++;
	//				SaveData ( Claster, 1, Nev[Claster], por );  //savedata.c
//					std::this_thread::sleep_for(std::chrono::milliseconds(99));
					max = SaveDataResult(Claster, 1, Nev[Claster], por);
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

			}  // if ( ( ( Data & 1 ) == 1 ) || ( ( Data & 2 ) == 2 ) )

			//--------------  END   SAVE DATA and RESET -------------


			//--------------   SAVE HIST and RESET  ------------------------

			ftime(&Now);   // Get Time and Date ( DOS and LINUX )

			if (this_moment>last_moment+TimePerHistogramm ) {

				printf("Histogramm Ready  -  ");  printf(ctime(&Now.time));

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
					por = OpenNewFile(nnn,por,flag);
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

