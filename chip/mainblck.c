//====================================================================

int MainBlock (unsigned long nnn, int *Threshold )
{
	int TRIGGER=1;
	//	  =1 -  external trigger
	//	  =2 -  programm's trigger
	//	  =3 -  self trigger

	unsigned long event=0;
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
		SetThresholdWithBaseLine ( Claster, Threshold);  // thresholds.c
	}
	printf("\n");
	// -----------------  Threshold  -------------------
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
			fprintf(fferr, "%s", ctime(&Now.time));
			fclose(fferr);
			printf("ERROR Connect      ");
			printf("%s", ctime(&Now.time));
		}
		//------------ Connect ERROR  --------------



		Claster = ClasterReady[ii];

		Data = 0;

		Data = GetStatusEvent ( Claster );  // master.c
		//  ------------   SAVE DATA and RESET ----------

		if ( ( ( Data & 1 ) == 1 ) || ( ( Data & 2 ) == 2 ) ) {
			slu = 1;   // !!!   Event Ready   !!!

			if ( ( Data & 1 ) == 1 ) {  //   Bank 1
				Nev[Claster] ++;
				//			printf ( "=====  Claster=%02i   Local Event = %5ld\n",
				//				Claster+1, Nev[Claster] );
				SaveData ( Claster, 0, Nev[Claster], por );  //savedata.c
				ResetEvent (Claster, 0 );  // master.c
				NEVENTS++;
			}

			if ( ( Data & 2 ) == 2 ) {   //  Bank 2
				Nev[Claster] ++;
				//			printf ( "=====  Claster=%02i   Local Event = %5ld\n",
				//				Claster+1, Nev[Claster] );
				SaveData ( Claster, 1, Nev[Claster], por );  //savedata.c
				ResetEvent (Claster, 1 );  // master.c
				NEVENTS++;
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

			printf("Histogramm Ready  -  ");  \
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

	ftime(&Now);   // Get Time
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
	return 1;
}
//====================================================================
//====================================================================
//====================================================================
//====================================================================
