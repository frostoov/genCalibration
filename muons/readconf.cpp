//===========================================================================
#include "readconf.h"
//===========================================================================
void ReadConfigFile(int *Thres, int *HV)
{
int Claster=0;
int PlataNum,ChannelNum;
int ready=0;

	NCLASTREADY = 0;
	for(Claster=0; Claster<NCLAST; Claster++) {

		ready = ReadConfiguration(Claster,Thres,HV,NoPrint);
		if ( ready ) {
			ClasterReady[NCLASTREADY] = Claster;
			printf("   Claster=%d\n",ClasterReady[NCLASTREADY]+1 );
			NCLASTREADY++;

		    for(PlataNum=0; PlataNum<NPLAT; PlataNum++) {
			printf("Claster=%i   Plata %i   Status::",Claster+1,PlataNum+1);
			DEFECT[Claster][PlataNum][0] ^= 1;
			DEFECT[Claster][PlataNum][1] = DEFECT[Claster][PlataNum][0];
			if (DEFECT[Claster][PlataNum][0]==1) printf(" not connected\n");
			else printf(" connected\n");
		    }
		}
	}
	if ( !NCLASTREADY ) {
	printf("\n!!!!   NOT READY CLUSTERS   !!!!\n\n");
	    abort();
	}
}
//===========================================================================
int ReadConfiguration(byte Claster, int *TH, int *HV, int flag)
{
FILE *fsub, *ftmp;
char fname[80], fntmp[80];
int PlataNum,ChannelNum;
int DetNum,thres,hv;
int ready=0;
int led,plat,chan;
int ii;

	sprintf(fname,"CLAST_%02d.CNF",(Claster+1));
	if ( (fsub = fopen(fname,"r")) == NULL ) {
		printf( "!!!   NOT Control file:   %s   !!!\n",fname);
		return 0;
	}

	fscanf(fsub,"%i",&ready);
	if ( !ready ) return 0;
	COINS[Claster] = ready;

	for(ii=0; ii<NDET; ii++) {

		fscanf(fsub,"%i%i%i%i%i%i",&DetNum,&thres,&hv,&led,&plat,&chan);
		if ( feof(fsub) ) {
		printf( "!!!   ERROR in CONFIG File:  CLUSTER %i  !!!\n",Claster+1);
		printf( "!!!   In CLUSTER %i NOT Detector %i   !!!\n",Claster+1,ii+1);
		return 0;
		}

		PlataNum = (DetNum-1)>>1;
		ChannelNum = (DetNum-1)&1;

		HV [Claster*NDET+PlataNum*2+ChannelNum] = hv;
		TH [Claster*NDET+PlataNum*2+ChannelNum] = thres;
		LED [Claster*NDET+PlataNum*2+ChannelNum] = led;
		DEFECT[Claster][PlataNum][ChannelNum]=chan;

		flag=1;

		if ( flag ) printf("plata=%i  chan=%i:   thresh=%i  HV=%i  LED=%i\n",
						plat,DEFECT[Claster][PlataNum][ChannelNum],
						TH[Claster*NDET+PlataNum*2+ChannelNum],
						HV[Claster*NDET+PlataNum*2+ChannelNum],
						LED[Claster*NDET+PlataNum*2+ChannelNum]);
	}
	fclose (fsub);

	if ( ready ) {

		sprintf(fntmp,"./CNF.tmp/CLAST_0%1i.TMP",(Claster+1));
		ftmp = fopen(fntmp,"w");

		fprintf(ftmp,"%1i\n",COINS[Claster]);

		for(DetNum=1; DetNum<=NDET; DetNum++) {

			PlataNum = (DetNum-1)>>1;
			ChannelNum = (DetNum-1)&1;
			fprintf(fsub,"%2i%7i%7i%7i%7i%7i\n",DetNum,
				TH [Claster*NDET+PlataNum*2+ChannelNum],
				HV [Claster*NDET+PlataNum*2+ChannelNum],
				LED [Claster*NDET+PlataNum*2+ChannelNum],
				PlataNum+1,DEFECT[Claster][PlataNum][ChannelNum]);
		}
		fprintf(fsub,"\nDet  Thresh   HV    LED   Plata  Channel\n");
		fprintf(fsub,"Num   code   code   code   num    num\n");
		fprintf(fsub,"\nCoinsidence\n");
		fclose (ftmp);
	}

return ready;
}
//===========================================================================
void SaveConfigFile(int Claster, int *TH, int *HV)
{
FILE *fsub;
char fname[80];
int DetNum,PlataNum,ChannelNum;

	sprintf(fname,"./CNF.tmp/CLAST_%02d.CNF",(Claster+1));
	fsub = fopen(fname,"w");

	fprintf(fsub,"%1i\n",COINS[Claster]);

	for ( DetNum=1; DetNum<=NDET; DetNum++ ) {

		PlataNum = (DetNum-1)>>1;
		ChannelNum = (DetNum-1)&1;

		fprintf(fsub,"%2i%7i%7i%7i%7i%7i\n",DetNum,
			TH [Claster*NDET+PlataNum*2+ChannelNum],
			HV [Claster*NDET+PlataNum*2+ChannelNum],
			LED [Claster*NDET+PlataNum*2+ChannelNum],
			PlataNum+1,DEFECT[Claster][PlataNum][ChannelNum]^1);
	}
	fprintf(fsub,"\nDet  Thresh   HV    LED   Plata  Channel\n");
	fprintf(fsub,"Num   code   code   code   num    num\n");
	fprintf(fsub,"\nCoinsidence\n");
	fclose(fsub);
}
//===========================================================================

