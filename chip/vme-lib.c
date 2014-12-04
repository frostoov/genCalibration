
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



#include <unistd.h>
//#include <errno.h>

typedef unsigned long ULONG;
typedef unsigned int UWORD;


VME cl;
VME *clA=&cl;
//VME *clA;


void UnConnect( void) 
{

    clA->UnConnectVME();

	printf( "Controller Un Connected \n");
	

//return res;
}


void Connect(char* host,int size) 
{
int res=0;
int kk=0;


//clA = new VME;

//clA = (VME*)malloc(sizeof(VME));

printf("host=%s, size=%i\n",host,size);


	clA->SetHost(host,size);

	clA->SetSocketPort(5858);


	res = clA->ConnectVME();


	if (res!=0) {
    	    printf("Connecting error!\n");
	    abort();
	}
	else printf( "Controller connected \n");
	

//return res;
}




int ReadWordExt12(ULONG adr,UWORD AModifier)     /* get word from arbitrary address */
//ULONG adr; UWORD AModifier;
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

//	UnConnect();
//	Connect(host,SIZE);

       }

    } while ( (res!=0) && (kk<2) );


  Data=((buf[1]&0xf)<<8)+(buf[0]&0xff);


//print_in_bits(buf[0]);
//print_in_bits(buf[1]);
//print_in_bits(Data);

//  printf("RD: word=0x%x(%i)   buf[1]=0x%x(%i),  buf[0]=0x%x(%i)\n",
//  Data,Data,buf[1],buf[1],buf[0],buf[0]);
//  printf("RD: buf[1]=0x%x(%i),  buf[0]=0x%x(%i),   word=0x%x(%i)   \n",
//  buf[1],buf[1],buf[0],buf[0],Data,Data);
//  printf("RD: word=0x%x(%i)\n",Data,Data);
  return(Data);
}


int ReadWordExt(ULONG adr,UWORD AModifier)     /* get word from arbitrary address */
//ULONG adr; UWORD AModifier;
{
  char buf[2];
  int Data=0;
  int res=0;
  int kk=0;

    buf[0]=0x0;
    buf[1]=0x0;


    do {

      res = (clA->RD(adr,2,buf));
//	printf("res=%i !!!!!!!!\n\n",res);


       if (res == -1) {  //  Connection ERROR happened
	kk++;
	printf("\n\n!!!!!!!!!!!!!!     Connection ERROR happened   kk=%i !!!!!!!!\n\n",kk);
	clA->UnConnectVME();
sleep(1);
	clA->ConnectVME();
	clA->SetSyncErr();


//	UnConnect();
//	Connect(host,SIZE);
       }

    } while ( (res!=0) && (kk<2) );

  Data=((buf[1]&0xff)<<8)+(buf[0]&0xff);


//print_in_bits(buf[0]);
//print_in_bits(buf[1]);
//print_in_bits(Data);

//  printf("RD: word=0x%x(%i)   buf[1]=0x%x(%i),  buf[0]=0x%x(%i)\n",
//  Data,Data,buf[1],buf[1],buf[0],buf[0]);
//  printf("RD: buf[1]=0x%x(%i),  buf[0]=0x%x(%i),   word=0x%x(%i)   \n",
//  buf[1],buf[1],buf[0],buf[0],Data,Data);
//  printf("RD: word=0x%x(%i)\n",Data,Data);
  return(Data);
}




int ReadDimExt12(ULONG adr, word nwords, short int *Data)     /* get word from arbitrary address */
//ULONG adr; UWORD AModifier;
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


//	UnConnect();
//	Connect(host,SIZE);

       }

    } while ( (res!=0) && (kk<2) );

  for(ii=0; ii<nwords*2; ii+=2) {

    Data[ii/2]=((buf[ii+1]&0xf)<<8)+(buf[ii]&0xff);

//    Data[ii/2]=(buf[ii+1]<<8)+(buf[ii]&0xff);

/*
if (Data[ii/2] > 3000 ) {
  printf("RD: buf[1]=0x%x(%i),  buf[0]=0x%x(%i),   word=0x%x(%i)   \n",
  buf[ii+1],buf[ii+1],buf[ii],buf[ii],Data[ii/2],Data[ii/2]);
}
*/

  }
  return(ii);
}


int ReadDimExt(ULONG adr, word nwords, int *Data)     /* get word from arbitrary address */
//ULONG adr; UWORD AModifier;
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


//	UnConnect();
//	Connect(host,SIZE);

       }

    } while ( (res!=0) && (kk<2) );



  for(ii=0; ii<nwords*2; ii+=2) {

    Data[ii/2]=((buf[ii+1]&0xff)<<8)+(buf[ii]&0xff);

//    Data[ii/2]=(buf[ii+1]<<8)+(buf[ii]&0xff);


//if (Data[ii/2] != 0 ) {
//  printf("RD: buf[1]=0x%x(%i),  buf[0]=0x%x(%i),   word=0x%x(%i)   \n",
//  buf[ii+1],buf[ii+1],buf[ii],buf[ii],Data[ii/2],Data[ii/2]);
//}


  }
  return(ii);
}






                                /* write word to an arbitrary address */
void WriteWordExt(ULONG adr, int Data,UWORD AModifier)
//ULONG adr; int Data; UWORD AModifier;
{
  char buf[2];
  int res=0;
  int kk=0;


//Data = 6155;
  buf[0]=(unsigned char)(Data)&0xff;
  buf[1]=(unsigned char)((Data>>8)&0xff);

//print_in_bits(Data);
//print_in_bits(buf[1]);
//print_in_bits(buf[0]);


//abort();
//  printf("WR: word=0x%x(%i)   buf[1]=0x%x(%i),  buf[0]=0x%x(%i)\n",
//	Data,Data,buf[1],buf[1],buf[0],buf[0]);


    do {

      res = (  clA->WR(adr,2,buf));


       if (res == -1) {  //  Connection ERROR happened
	kk++;
	printf("\n\n!!!!!!!!!!!!!!     Connection ERROR happened   kk=%i !!!!!!!!\n\n",kk);
	clA->UnConnectVME();
sleep(1);
	clA->ConnectVME();
	clA->SetSyncErr();


//	UnConnect();
//	Connect(host,SIZE);

       }

    } while ( (res!=0) && (kk<2) );

  return;
}


