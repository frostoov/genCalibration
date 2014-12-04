//====================================================================
//#include "array.h"
//#include "status.h"
//====================================================================

int MainBlock (unsigned long nnn, int *Threshold, int flag )
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



		Claster = ClasterReady[ii];

		Data = 0;

		Data = GetStatusEvent ( Claster );  // master.c
		//		FILE *mainFile = fopen("/home/main/lognew", "a+");
		//		fprintf(mainFile, "%d\n", Data);
		//		fclose(mainFile);

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

			//event = Nev[3];
			//printf( "=====  Common Event = %4ld\n", event );


			/*
ftime(&Now);   // Get Time
ftm = fopen(filetime,"at");
//fprintf(ftm, "=====  Common Event = %4ld\n", event );
  fprintf(ftm, "---> CommonEvent= %5ld", event );
  fprintf(ferr,"  %02d:%02d:%02d.%03d\n", t.tm_hour, t.tm_min, t.tm_sec, Now.millitm);
fclose(ftm);
*/

			//			printf (" -*> CommonEvent= %5ld", event);
			//                        printf("   %02d:%02d:%02d.%03d\n", t.tm_hour, t.tm_min, t.tm_sec, Now.millitm);
			if ( event >= EventsPerEndMeasure )	{
				StopRun=1;
			}
			if ( StopRun ) continue;
			if ( event && !(event % EventsPerFile) ) {  // open new file
				//				printf ( "por = %i   =====  Common Event = %4ld\n", por,event );
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
			//		else flag_STOP=0;
		}

		//	} while ( (event <EventsPerEndMeasure) && !StopRun && (kkkk!=27)   );
	} while ( (NEVENTS <EventsPerEndMeasure) && !StopRun );

	// ----------------  END  MAIN CYCLE  -----------


	//	printf("Event=%l\n",Nev[ClasterReady[0]]);

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




	/*
//	ferr = fopen("//home//DATA//T133//errors.txt","a");
	ferr = fopen("errors.txt","a");
	fprintf(ferr,"Flag stop  of measurements= %i\n", flag_STOP);
		fprintf(ferr,"\n \n  Stop Data Taking ---> ");
		fprintf(ferr,"  %02d-%02d-%02d %02d:%02d:%02d.%03dLT\n",
				  t.tm_mday, t.tm_mon+1, t.tm_year-100,
		  t.tm_hour, t.tm_min, t.tm_sec, Now.millitm);
	fclose(ferr);
	
//  ferr = fopen("//home//DATA//T133//t133_status.txt","at");
  ferr = fopen("t133_status.txt","at");
  fprintf(ferr,"\n \n  Stop Data Taking ---> ");
  fprintf(ferr,"  %02d-%02d-%02d %02d:%02d:%02d.%03dLT\n",
				  t.tm_mday, t.tm_mon+1, t.tm_year-100,
		  t.tm_hour, t.tm_min, t.tm_sec, Now.millitm);
  fprintf(ferr,"\n");
  fclose(ferr);
*/

	return 1;


	/*
	if ( flag_STOP ) {

		for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];
	if ( ((flag&24)==24) && (Claster==((flag>>6)-1)) ) continue;  //  WATER TANK
		All_HV_OFF(Claster, Print);

//	ferr = fopen("//home//DATA//T133//t133_status.txt","a");
	ferr = fopen("t133_status.txt","a");
	fprintf(ferr,"Claster #%02d   Procedures HV OFF have been done \n", Claster+1,flag_STOP);
	fclose(ferr);
		}
*/
	/*
		for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];
	if ( ((flag&24)==24) && (Claster==((flag>>6)-1)) ) continue;  //  WATER TANK
		All_Heat_OFF(Claster, Print);

	ferr = fopen("//DATA//T133//t133_status.txt","a");
	fprintf(ferr,"Claster #%02d   Procedures Heat OFF have been done \n", Claster+1,flag_STOP);
	fclose(ferr);
		}


		for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];
	if ( ((flag&24)==24) && (Claster==((flag>>6)-1)) ) continue;  //  WATER TANK
		All_Covers_Close(Claster, Print);

	ferr = fopen("//DATA//T133//t133_status.txt","a");
	fprintf(ferr,"Claster #%02d   Procedures COVERs CLOSE  have been done \n", Claster+1,flag_STOP);
	fclose(ferr);
		}


		for ( ii=0; ii<NCLASTREADY; ii++) {
		Claster = ClasterReady[ii];
	if ( ((flag&24)==24) && (Claster==((flag>>6)-1)) ) continue;  //  WATER TANK
		All_Heat_ON(Claster, Print);

	ferr = fopen("//DATA//T133//t133_status.txt","a");
	fprintf(ferr,"Claster #%02d   Procedures Heat ON have been done \n", Claster+1,flag_STOP);
	fclose(ferr);
		}
*/
	/*
	ftime(&Now);   // Get Time
//        ferr = fopen("//home//DATA//T133//t133_status.txt","at");
		ferr = fopen("t133_status.txt","at");
		fprintf(ferr,"\n\n Program Stop : ");
		fprintf(ferr,"  %02d-%02d-%02d %02d:%02d:%02d.%03dLT\n",
					   t.tm_mday, t.tm_mon+1, t.tm_year-100,
				   t.tm_hour, t.tm_min, t.tm_sec, Now.millitm);
		fprintf(ferr,"\n");
		fclose(ferr);

		UnConnect();

		exit(1);
*/

	//	} // flag STOP

	return 1;
}
//====================================================================
//====================================================================
//====================================================================
//====================================================================
