//===============================================================
//===============================================================
//
//   SUBRUOTUNES FOR SAVE TREATMENT's DATA
//   Summer.2006
//
//
//
//===============================================================
//===============================================================
#include <sys/timeb.h>
//#include <curses.h>

//int SecondData(int claster, unsigned long lll, int por, int Data[16][Aperture],
//						  unsigned long Nev, int dtime )

//int SecondData(int claster, unsigned long lll, int por, short int Data[16][Aperture])

//===============================================================
//int GetPulseParam(int iiii, int claster, unsigned long nnn, short int (*Data)[Aperture] )
int GetSummaPulse(int clast, short int (*DATA)[1024*4], float *PLS )
{
int ii,idet;

FILE *ftm, *fdin;
char fname[80];
int b1, b2, b3, bb[9], be[9], bm[9];
//float m1,sgm;
float Ampl[9]={0.,0.,0.,0.,0.,0.,0.,0.,0.};
float CC[9] = {1.,1.,1.,1.,1.,1.,1.,1.,1.};
float Ampl_tmp=0;
int multi=0;
int kk=0,keya=0,keyd=0;
//int tmp;

float Fwhm[9];
float bbb[9];

float dlcab[16];
float smin[16];


struct timeb Now;
time_t this_moment,last_moment;

float NewData[16][Aperture];
int iii=0;


int moda=1;

int fsize=0;

float Anode=0, Dinode=0;
int kf=0;

FILE *fff;

int kkk=0;

int j;
float mdl,m1,m2,sgm,mn,md,me,ms;

float max,ampl;


int ibeg=0,iend=0,imax=0;

int PlataNum,ChannelNum;



//    printf("filename=%s\n",outname);
//    printf("QQQQQQQQQQQ\n");





/*
	for ( idet=0; idet<8; idet++ ) {
		PLS[idet] = ZeroParam();
	}
*/

    for (PlataNum=0; PlataNum<NPLAT; PlataNum++) {

    for (ChannelNum=0; ChannelNum<NPIN; ChannelNum++) {


	ii = PlataNum*NDET + ChannelNum;


	if ( ii%2 ) continue;

	idet=ii/2;
	PLS[idet] = 0;


//printf("ii=%i   idet=%i   PlataNum=%i   ChannelNum=%i\n",
//ii,idet,PlataNum,ChannelNum);



	m1=0;
	m2=0;
	mn=0;
	for(j=0;j<300;j++) {
	    m1 += DATA[PlataNum][j+1024*ChannelNum ];
	    m2 += DATA[PlataNum][j+1024*ChannelNum]*DATA[PlataNum][j+1024*ChannelNum];
	    mn++;
	}

	m1/=mn;
	m2/=(mn-1);
	md = (m2) - (m1)*(m1)*mn/(mn-1);
	ms = sqrt(md);
	me = (ms)/sqrt(mn);
//	printf("middle=%f;   sgm=%f;   exact=%f  nn=%.f\n",m1,ms,me,mn);
	mdl=(-1.)*m1;
	sgm=ms;

//	printf("middle=%f;   sgm=%f;   exact=%f  nn=%.f\n",mdl,sgm,me,mn);

	max = mdl;
	imax=0;

	for(j=400;j<600;j++) {
	    ampl = (-1)*(DATA[PlataNum][j+1024*ChannelNum]);
	    if ( ampl > max ) {
		max = ampl;
		imax = j;
	    }
//printf("j=%i  data=%i    ampl=%.2f   max=%.2f    sgm=%.2f   imax=%i;\n",
//j,DATA[PlataNum][j+1024*ChannelNum],ampl,max,sgm,imax);
	}

//press_any_key();
//scanf("%i",&kk);

	ibeg=0;

	for(j=300;j<imax;j++) {
	    ampl = (-1)*(DATA[PlataNum][j+1024*ChannelNum]);
	    if ( ampl<(mdl+sgm) ){
		ibeg=j;
	    }
//printf("j=%i  data=%i    ampl=%.2f   mdl=%.2f   mdl+sgm=%.2f    sgm=%.2f   ibeg=%i;   iend=%i;\n",
//j,DATA[ii][j],ampl,mdl,mdl+sgm,sgm,ibeg,iend);
	}

	iend=0;
	for(j=600;j>imax;j--) {
	    ampl = (-1)*(DATA[PlataNum][j+1024*ChannelNum]);
	    if ( ampl<(mdl+sgm) ){
		iend=j;
	    }
//printf("j=%i  data=%i    ampl=%.2f   mdl+sgm=%.2f    sgm=%.2f   ibeg=%i;   iend=%i;\n",
//j,DATA[ii][j],ampl,mdl+sgm,sgm,ibeg,iend);
	}

//printf("ii=%i    idet=%i    max=%.2f   sgm=%.2f   ibeg=%i;   iend=%i;   imax=%i;\n",
//ii,idet,max,sgm,ibeg,iend,imax);


	if ( (ibeg==0) || (iend==0) ) {
	    idet=ii/2;
	    PLS[idet] = 0;
	    continue;
	}

	if ( (iend-ibeg)<5 ) {
	    idet=ii/2;
	    PLS[idet] = 0;
	    continue;
	}



	Anode = 0;
	for(j=ibeg-1;j<=iend+1;j++) {
	    ampl = (-1)*(DATA[PlataNum][j+1024*ChannelNum]);
	    Anode += (ampl-mdl);

//printf("j=%i  data=%i    Anode=%.2f    middle=%f;   sgm=%f;   exact=%f  nn=%.f\n",
//j,DATA[PlataNum][j+1024*ChannelNum],Anode,mdl,sgm,me,mn);

	}
//	Anode *= (-1.);
//printf("ii=%i   Anode=%.2f    middle=%f;   sgm=%f;   exact=%f  nn=%.f\n",ii,Anode,m1,ms,me,mn);

	idet=ii/2;
	PLS[idet] = Anode;
//printf("ii=%i  idet=%i Anode=%.2f    middle=%.2f;   sgm=%.2f;   imax=%i   ibeg=%i  iend=%i\n",
//ii,idet,Anode,mdl,sgm,imax,ibeg,iend);


//printf("idet=%i   PLs=%.2f    middle=%f;   sgm=%f;   exact=%f  nn=%.f\n",idet,PLS[idet].ampl,m1,ms,me,mn);


//press_any_key();

    }
    }


return 210;
}
//===============================================================
