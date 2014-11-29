//====================================================================

#include "create_dir.h"

struct timeb Now,Stop;
struct tm t;
FILE *ffstat, *fftime, *fferr, *ffdat;
char pathfile[120];
char statfile[120];
char timefile[120];
char summfile[120];
char errfile[120];
char datafile[120];

//====================================================================
unsigned long Create_Dir_Files(void) 
{
FILE *fsub;
char st[20];
int res=0;
char datadir[120];
int year=0;


    if ( ( fsub = fopen ( "PATHFILES.CNF","r") ) == NULL  ) {
	printf("\n	ERROR::   NOT  PATHFILES.CNF   !!!!!!!\n\n");
	abort();
    }

    fscanf(fsub,"%s",pathfile);
    fclose(fsub);

    ftime(&Now);                   //  for DOS and LINUX
    t = *localtime(&Now.time);

    t.tm_mon+=1; 
    year = (int) fmod(t.tm_year,100);
    t.tm_year+=1900;

    nnn  =  (unsigned long)t.tm_mday*10000L + 
	    (unsigned long)t.tm_mon*100L + 
	    (unsigned long)year;
    printf ("Now is : day=%i,  month=%i,  year=%i,  nnn=%ld\n",
	    t.tm_mday,t.tm_mon,t.tm_year,nnn);
    printf ("Time is : %02d:%02d:%02d.%03d\n",
	    t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);


    sprintf(st,"%06ld//",nnn);
    strcat(pathfile,st);
	res = mkdir(pathfile,0x41FF);  //  create new directory

    if ( res==0 )   printf("NEW DIRECTORY FOR DATA IS CREATED :  %s\n",pathfile);
    if ( res==-1 )  printf("DIRECTORY FOR DATA WAS CREATED USIALLY :  %s\n",pathfile);

    strcpy(datadir,pathfile);
    strcat(datadir,"/txt/");
	res = mkdir(datadir,0x41FF);  //  create new directory

    strcpy(datadir,pathfile);
    strcat(datadir,"/hist/");
	res = mkdir(datadir,0x41FF);  //  create new directory

    strcpy(statfile,pathfile);
    strcat(statfile,"t133_status.txt");
    ffstat = fopen(statfile,"a");
    fprintf (ffstat,"\n================================================================================\n");
    fprintf (ffstat,"\n Program Start:");
    fprintf (ffstat,"  %02d-%02d-%02d  %02d:%02d:%02d.%03d LT\n",
	    t.tm_mday,t.tm_mon,t.tm_year,
	    t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);
    fclose(ffstat);
printf("  CREATED  statfile=%s\n",statfile);

    strcpy(errfile,pathfile);
    strcat(errfile,"t133_errors.txt");
    fferr = fopen(errfile,"a");
    fprintf (fferr,"\n================================================================================\n");
    fprintf (fferr,"\n Program Start:");
    fprintf (fferr,"  %02d-%02d-%02d  %02d:%02d:%02d.%03d LT\n",
	    t.tm_mday,t.tm_mon,t.tm_year,
	    t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);
    fclose(fferr);
printf("  CREATED  errfile =%s\n",errfile);

    sprintf(st,"%06ld.time",nnn);
    strcpy(timefile,pathfile);
    strcat(timefile,st);
    fftime = fopen(timefile,"a");
    fprintf (fftime,"\n================================================================================\n");
    fprintf (fftime,"\n Program Start:");
    fprintf (fftime,"  %02d-%02d-%02d  %02d:%02d:%02d.%03d LT\n",
	    t.tm_mday,t.tm_mon,t.tm_year,
	    t.tm_hour,t.tm_min,t.tm_sec,Now.millitm);
    fclose(fftime);
printf("  CREATED  timefile=%s\n",timefile);

    sprintf(st,"%06ld.sum",nnn);
    strcpy(summfile,pathfile);
    strcat(summfile,st);
    fftime = fopen(summfile,"w");
    fclose(fftime);
printf("  CREATED  summfile=%s\n",summfile);

    year = (int) fmod(t.tm_year,10);
    nnn  =  (unsigned long)t.tm_mday*1000L + 
	    (unsigned long)t.tm_mon*10L + 
	    (unsigned long)year;
    printf ("Now is : day=%i,  month=%i,  year=%i,  nnn=%ld\n",
	    t.tm_mday,t.tm_mon,year,nnn);

    mkdir("./CNF.tmp/",0x41FF);  //  create new directory

return nnn;
}
//====================================================================
