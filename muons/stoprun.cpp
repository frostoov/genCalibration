#include "stoprun.h"

int h_stop=0,m_stop=0,s_stop=0;
//====================================================================
void ReadStopTime (void)
{
FILE *fff;
char ch[10];

fff = fopen("STOPRUN.CNF","r");
fscanf(fff,"%2d%1s%2d%1s%2d",&h_stop,&ch,&m_stop,&ch,&s_stop);
fscanf(fff,"%ld",&EventsPerEndMeasure);
printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
printf("		RUN will be stoped at    %02d %02d %02d\n",h_stop,m_stop,s_stop);
printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
fclose(fff);
}
//====================================================================
int GetStopRun  ( void )
{
int hs=0,ms=0,ss=0;

    if (h_stop==0 && m_stop==0 && s_stop==0) return 0;


ftime(&Stop);
t=*localtime(&Stop.time);

   hs=t.tm_hour;
   ms=t.tm_min;
   ss=t.tm_sec;

    if ( (hs>=19) && (h_stop<=7) ) return 0;

    if ( (hs >= h_stop) && (ms>=m_stop) ) return 1;
    else return 0;

}
//====================================================================





