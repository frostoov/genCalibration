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
//===============================================================
int GetSummaPulse(short int (*DATA)[1024*4], float *PLS )
{
	int ii,idet;

	float Anode=0;

	int j;
	float mdl,m1,m2,sgm,mn,md,ms;

	float max,ampl;

	int ibeg=0,iend=0,imax=0;

	int PlataNum,ChannelNum;
	for (PlataNum=0; PlataNum<NPLAT; PlataNum++)
	{
		for (ChannelNum=0; ChannelNum<NPIN; ChannelNum++)
		{
			ii = PlataNum*NDET + ChannelNum;
			if ( ii%2 ) continue;
			idet=ii/2;
			PLS[idet] = 0;
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
			mdl=(-1.)*m1;
			sgm=ms;
			max = mdl;
			imax=0;

			for(j=400;j<600;j++) {
				ampl = (-1)*(DATA[PlataNum][j+1024*ChannelNum]);
				if ( ampl > max ) {
					max = ampl;
					imax = j;
				}
			}
			ibeg=0;

			for(j=300;j<imax;j++) {
				ampl = (-1)*(DATA[PlataNum][j+1024*ChannelNum]);
				if ( ampl<(mdl+sgm) ){
					ibeg=j;
				}
			}

			iend=0;
			for(j=600;j>imax;j--) {
				ampl = (-1)*(DATA[PlataNum][j+1024*ChannelNum]);
				if ( ampl<(mdl+sgm) ){
					iend=j;
				}
			}
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
			}
			idet=ii/2;
			PLS[idet] = Anode;
		}
	}
	return 210;
}
//===============================================================
