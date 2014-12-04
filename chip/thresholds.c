//===========================================================================
#ifndef __TOOLS_C
#define __TOOLS_C
//===========================================================================
//====================================================================
void GetBaseLine (byte Claster, byte PlataNum, int *BaseLine, int flag ) 
{
float baseline=0;
int ibase;
short int MASS[Aperture*4];
int ich,imas;

    GetPulseFormDim(Claster,PlataNum,0,MASS);




    for (ich=0; ich<4; ich++) {

        ibase=0;
	baseline=0;
        for (imas=0+ich*Aperture;imas<300+ich*Aperture;imas++) {
//	  if( MASS[imas]>-150 && MASS[imas]<150 ) {
	    baseline += (float)MASS[imas];
	    ibase++;
//        }
	}

	if (ibase==0) {
	    printf("Cluster#%02d  Plata%i   ",Claster+1,PlataNum+1);
	    printf("!!!!!!!   GetThreshold:: ERROR - NOT BASE LINE !!!!\n");

/*
ffstat = fopen(statfile,"a");
fprintf(ffstat,"!!!!!!!   GetThreshold:: ERROR - NOT BASE LINE !!!!\n");
fclose(ffstat);
*/
	    abort();
	}

        baseline /= (float)ibase;
//	printf("                 Base Line  Channel %i  =%.1f   bank 1\n",ich+1,baseline);
/*
if ( flag ) {
ffstat = fopen(statfile,"a");
fprintf(ffstat,"                 Base Line  Channel %i  =%.1f   bank 1\n",ich+1,baseline);
fclose(ffstat);
}
*/

	if ( (baseline>50) || (baseline<-50) ) {
	    printf("Cluster#%02d  Plata%i   ",Claster+1,PlataNum+1);
	    printf("!!!!!!!   GetThreshold:: ERROR - BAD BASE LINE !!!!\n");

/*
ffstat = fopen(statfile,"a");
fprintf(ffstat,"!!!!!!!   GetThreshold:: ERROR - BAD BASE LINE !!!!\n");
fclose(ffstat);
*/
//	    abort();
	}

	if (ich==0) {
//		printf("Before:    Threshold=%i\n",Threshold[(Claster-1)*8+PlataNum*2]);
	BaseLine[0] = (int)baseline;
//		printf("BL[0]=%i BaseLine=%.1f  del=%.1f\n",BL[0],baseline, fabs(baseline-(int)baseline) );
//	if ( fabs(baseline-(int)baseline) > 0.5 ) BL[0]+=1;
//		printf("After:    Threshold=%i\n",Threshold[(Claster-1)*8+PlataNum*2]);
	}
	if (ich==2) {
//		printf("Before:    Threshold=%i\n",Threshold[(Claster-1)*8+PlataNum*2+1]);
	BaseLine[1] = (int)baseline;
//		printf("BL[1]=%i BaseLine=%.1f  del=%.1f\n",BL[1],baseline,fabs(baseline-(int)baseline) );
//	if ( fabs(baseline-(int)baseline) > 0.5 ) BL[1]+=1;
//		printf("After:    Threshold=%i\n",Threshold[(Claster-1)*8+PlataNum*2+1]);
	}
    }

    GetPulseFormDim(Claster,PlataNum,1,MASS);

    for (ich=0; ich<4; ich++) {

        ibase=0;
	baseline=0;
        for (imas=0+ich*Aperture;imas<300+ich*Aperture;imas++) {
//	  if( MASS[imas]>-150 && MASS[imas]<150 ) {
	    baseline += (float)MASS[imas];
	    ibase++;
//        }
	}

	if (ibase==0) {
	    printf("Cluster#%02d  Plata%i   ",Claster+1,PlataNum+1);
	    printf("!!!!!!!   GetThreshold:: ERROR - NOT BASE LINE !!!!\n");

/*
ffstat = fopen(statfile,"a");
fprintf(ffstat,"!!!!!!!   GetThreshold:: ERROR - NOT BASE LINE !!!!\n");
fclose(ffstat);
*/
	    abort();
	}

        baseline /= (float)ibase;
//	printf("                 Base Line  Channel %i  =%.1f   bank 2\n",ich+1,baseline);
/*
if ( flag ) {
ffstat = fopen(statfile,"a");
fprintf(ffstat,"                 Base Line  Channel %i  =%.1f   bank 2\n",ich+1,baseline);
fclose(ffstat);
}
*/
	if ( (baseline>50) || (baseline<-50) ) {
	    printf("Cluster#%02d  Plata%i   ",Claster+1,PlataNum+1);
	    printf("!!!!!!!   GetThreshold:: ERROR - BAD BASE LINE !!!!\n");

/*
ffstat = fopen(statfile,"a");
fprintf(ffstat,"!!!!!!!   GetThreshold:: ERROR - BAD BASE LINE !!!!\n");
fclose(ffstat);
*/
//	    abort();
	}

	if (ich==0) {
//		printf("Before:    Threshold=%i\n",Threshold[(Claster-1)*8+PlataNum*2]);
	BaseLine[0] = (int)baseline;
//		printf("BL[0]=%i BaseLine=%.1f  del=%.1f\n",BL[0],baseline, fabs(baseline-(int)baseline) );
//	if ( fabs(baseline-(int)baseline) > 0.5 ) BL[0]+=1;
//		printf("After:    Threshold=%i\n",Threshold[(Claster-1)*8+PlataNum*2]);
	}
	if (ich==2) {
//		printf("Before:    Threshold=%i\n",Threshold[(Claster-1)*8+PlataNum*2+1]);
	BaseLine[1] = (int)baseline;
//		printf("BL[1]=%i BaseLine=%.1f  del=%.1f\n",BL[1],baseline,fabs(baseline-(int)baseline) );
//	if ( fabs(baseline-(int)baseline) > 0.5 ) BL[1]+=1;
//		printf("After:    Threshold=%i\n",Threshold[(Claster-1)*8+PlataNum*2+1]);
	}
    }




}
//====================================================================
void SetThresholdWithBaseLine (byte Claster, int *Threshold, int flag ) 
{
byte PlataNum=0;
int BaseLine[2]={0,0};

/*
ffstat = fopen(statfile,"a");
fprintf(ffstat,"Cluster#%02d::\n",Claster);
fclose(ffstat);
*/
//printf("Cluster#%02d::\n",Claster+1);

    ffstat = fopen(statfile,"a");
    fprintf(ffstat,"Claster#%02d\n",Claster+1);


    for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {

/*
if ( flag ) {
ffstat = fopen(statfile,"a");
fprintf(ffstat,"     Plata Number %d\n",PlataNum+1);
fclose(ffstat);
}
*/
//printf("     Plata Number %d\n",PlataNum+1);




	if ( DEFECT[Claster][PlataNum][0] ) continue;

	GetBaseLine (Claster, PlataNum, BaseLine, flag );

	SetThreshold ( Claster, PlataNum, 0, 
//	    Threshold[(Claster-1)*8+PlataNum*2+0]-BaseLine[0] );
//printf("Threshold=%i   BaseLine=%i   ThresholdWithBaseLine=%i\n",
//	    Threshold[(Claster-1)*8+PlataNum*2+0], BaseLine[0], Threshold[(Claster-1)*8+PlataNum*2+0]-BaseLine[0]);
	    Threshold[(Claster)*NDET+PlataNum*2+0]-BaseLine[0]);
//	    ClusterT[Claster].THcode[PlataNum*2+0]-BaseLine[0] );
//printf("Threshold=%i   BaseLine=%i   ThresholdWithBaseLine=%i\n",
//	    ClusterT[Claster].THcode[PlataNum*2+0], BaseLine[0], ClusterT[Claster].THcode[PlataNum*2+0]-BaseLine[0]);

//printf("Plata=%i   Channel=1   Threshold=%3i   BaseLine=%3i   ThresholdWithBaseLine=%4i\n",
//	    PlataNum+1, Threshold[(Claster)*NDET+PlataNum*2+0], BaseLine[0], Threshold[(Claster)*NDET+PlataNum*2+0]-BaseLine[0]);

/*
if ( flag ) {
ffstat = fopen(statfile,"a");
fprintf(ffstat,"Threshold=%i   BaseLine=%i   ThresholdWithBaseLine=%i\n",
	    ClusterT[Claster].THcode[PlataNum*2+0], BaseLine[0], ClusterT[Claster].THcode[PlataNum*2+0]-BaseLine[0]);
fclose(ffstat);
}
*/
	SetThreshold ( Claster, PlataNum, 1, 
//	    Threshold[(Claster-1)*8+PlataNum*2+1]-BaseLine[1] );
//printf("Threshold=%i   BaseLine=%i   ThresholdWithBaseLine=%i\n", 
//	    Threshold[(Claster-1)*8+PlataNum*2+1], BaseLine[1], Threshold[(Claster-1)*8+PlataNum*2+1]-BaseLine[1]);
	    Threshold[(Claster)*NDET+PlataNum*2+1]-BaseLine[1]);
//	    ClusterT[Claster].THcode[PlataNum*2+1]-BaseLine[1] );
//printf("Threshold=%i   BaseLine=%i   ThresholdWithBaseLine=%i\n",
//	    ClusterT[Claster].THcode[PlataNum*2+1], BaseLine[1], ClusterT[Claster].THcode[PlataNum*2+1]-BaseLine[1]);


//printf("Plata=%i   Channel=3   Threshold=%3i   BaseLine=%3i   ThresholdWithBaseLine=%4i\n",
//	    PlataNum+1, Threshold[(Claster)*NDET+PlataNum*2+1], BaseLine[1], Threshold[(Claster)*NDET+PlataNum*2+1]-BaseLine[1]);


//if ( flag ) {
//}

fprintf(ffstat,"Plata=%i   Channel=1   Threshold=%3i   BaseLine=%3i   ThresholdWithBaseLine=%4i\n",
	    PlataNum+1, Threshold[(Claster)*NDET+PlataNum*2+0], BaseLine[0], Threshold[(Claster)*NDET+PlataNum*2+0]-BaseLine[0]);
fprintf(ffstat,"Plata=%i   Channel=2   Threshold=%3i   BaseLine=%3i   ThresholdWithBaseLine=%4i\n",
	    PlataNum+1, Threshold[(Claster)*NDET+PlataNum*2+1], BaseLine[1], Threshold[(Claster)*NDET+PlataNum*2+1]-BaseLine[1]);



    }


fclose(ffstat);


/*
if ( flag ) {
ffstat = fopen(statfile,"a");
fprintf(ffstat,"\n");
fclose(ffstat);
}
*/
}
//====================================================================
/*
void AfterTime ( time_t this_moment, time_t last_moment, int flag ) 
{
int jj=0;
//--------------   SAVE HIST and RESET  ------------------------

	ftime(&Now);   // Get Time and Date ( DOS and LINUX )

//		printf("QQQ 1 - ");
//		printf(ctime(&Now.time));

	if (this_moment<last_moment+TimePerHistogramm ) return;


	if (this_moment>last_moment+TimePerHistogramm ) {
//			|| ( (Data!=-1) && ( Data & 2 ) == 2 ) ) {

		    printf("QQQ 2 - ");  printf(ctime(&Now.time));

//	ev++;
//	SaveTime ( Claster, ev );


	    for(jj=0; jj<NCLASTREADY;jj++) {

		ReadAmplAnalizator( ClasterReady[jj],0);
	    }


	    for(jj=0; jj<NCLASTREADY;jj++) {

//	if ( ((flag&24)==24) && (ClasterReady[jj]==((flag>>6)-1)) ) continue;  //  WATER TANK
	if ( ((flag&24)==24) && (ClasterReady[jj]==20) ) continue;  //  WATER TANK


		if ( ClasterReady[jj] != 1 ) Temperature_in_Box ( ClasterReady[jj], NoPrint );

		Status ( ClasterReady[jj], Print );

	    }


	    last_moment = this_moment;

	}


	this_moment = Now.time;

//--------------   HIST end   ------------------------


}
*/
//====================================================================
//====================================================================
//====================================================================
#endif
