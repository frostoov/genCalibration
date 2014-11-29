//===========================================================================
#ifndef __TOOLS_C
#define __TOOLS_C
#include "thresholds.h"
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
			baseline += (float)MASS[imas];
			ibase++;
			//        }
		}

		if (ibase==0) {
			printf("Cluster#%02d  Plata%i   ",Claster+1,PlataNum+1);
			printf("!!!!!!!   GetThreshold:: ERROR - NOT BASE LINE !!!!\n");
			abort();
		}

		baseline /= (float)ibase;

		if ( (baseline>50) || (baseline<-50) ) {
			printf("Cluster#%02d  Plata%i   ",Claster+1,PlataNum+1);
			printf("!!!!!!!   GetThreshold:: ERROR - BAD BASE LINE !!!!\n");
		}

		if (ich==0) {
			BaseLine[0] = (int)baseline;
		}
		if (ich==2) {
			BaseLine[1] = (int)baseline;
		}
	}

	GetPulseFormDim(Claster,PlataNum,1,MASS);

	for (ich=0; ich<4; ich++) {

		ibase=0;
		baseline=0;
		for (imas=0+ich*Aperture;imas<300+ich*Aperture;imas++) {
			baseline += (float)MASS[imas];
			ibase++;
		}

		if (ibase==0) {
			printf("Cluster#%02d  Plata%i   ",Claster+1,PlataNum+1);
			printf("!!!!!!!   GetThreshold:: ERROR - NOT BASE LINE !!!!\n");

			abort();
		}

		baseline /= (float)ibase;
		if ( (baseline>50) || (baseline<-50) ) {
			printf("Cluster#%02d  Plata%i   ",Claster+1,PlataNum+1);
			printf("!!!!!!!   GetThreshold:: ERROR - BAD BASE LINE !!!!\n");

		}

		if (ich==0) {
			BaseLine[0] = (int)baseline;
		}
		if (ich==2) {
			BaseLine[1] = (int)baseline;
		}
	}




}
//====================================================================
void SetThresholdWithBaseLine (byte Claster, int *Threshold, int flag ) 
{
	byte PlataNum=0;
	int BaseLine[2]={0,0};
	ffstat = fopen(statfile,"a");
	fprintf(ffstat,"Claster#%02d\n",Claster+1);


	for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {

		if ( DEFECT[Claster][PlataNum][0] ) continue;

		GetBaseLine (Claster, PlataNum, BaseLine, flag );

		SetThreshold ( Claster, PlataNum, 0,
					   Threshold[(Claster)*NDET+PlataNum*2+0]-BaseLine[0]);
		SetThreshold ( Claster, PlataNum, 1,
					   Threshold[(Claster)*NDET+PlataNum*2+1]-BaseLine[1]);
		fprintf(ffstat,"Plata=%i   Channel=1   Threshold=%3i   BaseLine=%3i   ThresholdWithBaseLine=%4i\n",
				PlataNum+1, Threshold[(Claster)*NDET+PlataNum*2+0], BaseLine[0], Threshold[(Claster)*NDET+PlataNum*2+0]-BaseLine[0]);
		fprintf(ffstat,"Plata=%i   Channel=2   Threshold=%3i   BaseLine=%3i   ThresholdWithBaseLine=%4i\n",
				PlataNum+1, Threshold[(Claster)*NDET+PlataNum*2+1], BaseLine[1], Threshold[(Claster)*NDET+PlataNum*2+1]-BaseLine[1]);



	}


	fclose(ffstat);
}

#endif
