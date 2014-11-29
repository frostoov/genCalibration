
/**********************************************************************/
/*                                                                    */
/* VMELIB.C                                                           */
/* Library of access functions for AT_VME, Turbo C, MS-DOS            */
/* for AT_VME Revision 1.0, 2.0, ITVME6 und ITVME3                    */
/*                                                                    */
/* (c) pro_VME Industrielle Rechnertechnik GmbH                       */
/* Postfach 1236, Wiesenbacherstr. 77                                 */
/* D-6903 Neckargemuend                                               */
/*                                                                    */
/*                                                                    */
/* Last edit: 11.9.89 K.Hitschler                                     */
/*            20.9.90 K.Hitschler Anpassung an REL2.0                 */
/* Als meine genommen 27.11.91 R.Dux                                  */
/*                                                                    */
/**********************************************************************/

//#define INCLUDE_MIT_VARIABLEN

/*--------------------------------------------------------------------*/
/*                                                                    */
/* INIT functions   for AT_VME and VMEbus                             */
/*                                                                    */
/*                                                                    */
/*--------------------------------------------------------------------*/
/*                                                                    */
/* WRITE and READ to an arbitrary address                             */
/* The address is characterized with:                                 */
/*                                    ULONG adr                       */
/*                                    UWORD AModifier                 */
/*                                                                    */
/* The 'adr' uses in 'Standard' address mode only 24 bit and          */
/*                in 'Extended' address mode 32 bit of its content    */
/*                                                                    */
/* After each access the default DefAModifier = Short_NoPriv is       */
/*                                                   restored         */
/*                                                                    */
/* These accesses are slow compared to the ..Short operations         */
/*                                                                    */

#include "vme-lib.h"

VME cl;
VME* clA=&cl;

void UnConnect( void) 
{

    clA->UnConnectVME();

	printf( "Controller Un Connected \n");	
}


void Connect(char* host,int size) 
{
int res=0;
int kk=0;
printf("host=%s, size=%i\n",host,size);


	clA->SetHost(host,size);

	clA->SetSocketPort(5858);


	res = clA->ConnectVME();


	if (res!=0) {
    	    printf("Connecting error!\n");
	    abort();
	}
	else printf( "Controller connected \n");

}




int ReadWordExt12(ULONG adr,UWORD AModifier)     /* get word from arbitrary address */
{
  char buf[2];
  int Data=0;
  int res=0;
  int kk=0;

    buf[0]=0x0;
    buf[1]=0x0;


    do {

      res = (clA->RD(adr,2,buf));


       if (res == -1) {  //  Connection ERROR happened
	kk++;
	printf("\n\n!!!!!!!!!!!!!!     Connection ERROR happened   kk=%i !!!!!!!!\n\n",kk);
	clA->UnConnectVME();
sleep(1);
	clA->ConnectVME();

	clA->SetSyncErr();
       }

    } while ( (res!=0) && (kk<2) );


  Data=((buf[1]&0xf)<<8)+(buf[0]&0xff);
  return(Data);
}


int ReadWordExt(ULONG adr,UWORD AModifier)     /* get word from arbitrary address */
{
  char buf[2];
  int Data=0;
  int res=0;
  int kk=0;

    buf[0]=0x0;
    buf[1]=0x0;


    do {

      res = (clA->RD(adr,2,buf));

       if (res == -1) {  //  Connection ERROR happened
	kk++;
	printf("\n\n!!!!!!!!!!!!!!     Connection ERROR happened   kk=%i !!!!!!!!\n\n",kk);
	clA->UnConnectVME();
sleep(1);
	clA->ConnectVME();
	clA->SetSyncErr();
       }

    } while ( (res!=0) && (kk<2) );

  Data=((buf[1]&0xff)<<8)+(buf[0]&0xff);
  return(Data);
}




int ReadDimExt12(ULONG adr, word nwords, short int *Data)     /* get word from arbitrary address */
{
  char buf[2048];
  int ii;
  int res=0;
  int kk=0;


  for(ii=0; ii<2048; ii++) {
    buf[ii]=0;
  }
  for(ii=0; ii<nwords; ii++) {
    Data[ii]=0;
  }


    do {

      res = (clA->RD(adr,nwords*2,buf));


       if (res == -1) {  //  Connection ERROR happened
	kk++;
	printf("\n\n!!!!!!!!!!!!!!     Connection ERROR happened   kk=%i !!!!!!!!\n\n",kk);
	clA->UnConnectVME();
sleep(1);
	clA->ConnectVME();
	clA->SetSyncErr();
       }

    } while ( (res!=0) && (kk<2) );

  for(ii=0; ii<nwords*2; ii+=2) {

    Data[ii/2]=((buf[ii+1]&0xf)<<8)+(buf[ii]&0xff);
  }
  return(ii);
}


int ReadDimExt(ULONG adr, word nwords, int *Data)     /* get word from arbitrary address */
{
  char buf[2048];
  int ii;
  int res=0;
  int kk=0;


  for(ii=0; ii<2048; ii++) {
    buf[ii]=0;
  }
  for(ii=0; ii<nwords; ii++) {
    Data[ii]=0;
  }


    do {

      res = (clA->RD(adr,nwords*2,buf));


       if (res == -1) {  //  Connection ERROR happened
	kk++;
	printf("\n\n!!!!!!!!!!!!!!     Connection ERROR happened   kk=%i !!!!!!!!\n\n",kk);
	clA->UnConnectVME();
sleep(1);
	clA->ConnectVME();
	clA->SetSyncErr();
       }

    } while ( (res!=0) && (kk<2) );



  for(ii=0; ii<nwords*2; ii+=2) {

    Data[ii/2]=((buf[ii+1]&0xff)<<8)+(buf[ii]&0xff);


  }
  return(ii);
}






                                /* write word to an arbitrary address */
void WriteWordExt(ULONG adr, int Data,UWORD AModifier)
{
  char buf[2];
  int res=0;
  int kk=0;

  buf[0]=(unsigned char)(Data)&0xff;
  buf[1]=(unsigned char)((Data>>8)&0xff);


    do {

      res = (  clA->WR(adr,2,buf));


       if (res == -1) {  //  Connection ERROR happened
	kk++;
	printf("\n\n!!!!!!!!!!!!!!     Connection ERROR happened   kk=%i !!!!!!!!\n\n",kk);
	clA->UnConnectVME();
sleep(1);
	clA->ConnectVME();
	clA->SetSyncErr();
       }

    } while ( (res!=0) && (kk<2) );

  return;
}


