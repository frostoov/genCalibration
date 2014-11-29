//====================================================================
#include "vme.hpp"
#include "defines.h"
//#define VME_DEBUG

//====================================================================

int VME::ConnectVME()
{
int conn_state =0;
	
 FD_ZERO( &select_set );



        sc = new Socket( Socket_Port != -1 ? AF_INET : AF_UNIX, SOCK_STREAM );
    

	if( !sc->good() ) {
		err = -2;
//		#ifdef VME_DEBUG 
		ErrCode(true);
//		#endif
		return -1;
	}



	sc_fd = sc->fd();



    rd = setsockopt( sc_fd, SOL_SOCKET, SO_DONTROUTE, &val, sizeof ( val ) );    
	rd = setsockopt( sc_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof ( val ) );    
	
	printf("rd = %d  sc_fd = %d\n", rd, sc_fd );



        if( sc->bind(host, Socket_Port ) < 0 ) { 
//		nrerror( "Bind error ....... ");     
		printf( "Bind error ....... \n");     
			err = -3;
//			#ifdef VME_DEBUG
			ErrCode(true);
//			#endif
			return -1;
		}
		


    
        printf("Bind ok !!!!\n");
    
	//for( i = 0; i < Real_BPI_num; i++ ) {//в цикле пытаемся подключиться к контроллеру
	//  printf("connected[i] = %d\n", connected[i] );
	  conn_state = 0;
	  while ((connected == false) && (conn_state<10))  { 	
             conn_state++; 	  

	     timeRec.tv_sec  = 1;  // 1 second polling for controller connections 
             timeRec.tv_usec = 0;
	     
	     FD_SET( sc_fd, &select_set );
	     rc = select( FD_SETSIZE, &select_set, 0, 0, &timeRec );	

             printf("select ...... rc = %d conn_state = %d\n",rc, conn_state );

	     if(( rc > 0 ) && ( FD_ISSET( sc_fd, &select_set ) ) ) {
	       if(( nsid = sc->accept( 0 , 0 ) ) < 0 ) {
	         printf( "Accept Error!\n" ); 
//		 return;
			   err = -4;
//			   #ifdef VME_DEBUG
				ErrCode(true);
//				#endif
//			   return -1;
		}
//	       else {
	         printf( "Accept Ok! \nrc = %d nsid = %d\n", rc, nsid );
//		} 
	       connected = true;	
	      }
		  if ((conn_state == 10) && ( connected == false)) {
			err = -5;
//			#ifdef VME_DEBUG
			ErrCode(true);
//			#endif
			return -1;
		  }
 	   }
	 //}	



	return 0;
}
//====================================================================
int VME::WR(longint Addr, word NumBytes, char *Data)//write data
{
int add=0;	
long GlobalAddr=0L;


	GlobalAddr=Addr;
	mess_len = 0;    
	hd = HD;
	comm = CMD_WR;


	if ( ( Addr&0x400000 ) == 0x400000 ) {
	    add=16;
	}
	else {
	    add = (int)(Addr>>23);
	}

	    comm+=add;
	    Addr&=0x7fffff;
//	printf("Addr       : 0x%lx\n", Addr );

	
	int conn_state =0;
	

//comm=2;


	memcpy( &buf[0], &hd, 2 );         // 0xFFFF
	memcpy( &buf[2], &comm, 1 );       // command
	memcpy( &buf[3], &Addr, 4 ); 		//first addr for write
	memcpy( &buf[7], &NumBytes, 2 ); 	//bytes for transfer
	memcpy( &buf[9], Data, NumBytes);
	
	mess_len = HEADER_LEN+ADDR_LEN+BFT_LEN+NumBytes;
	
	res = sc->write( buf, mess_len, 0, nsid ); //sending command message
	if (res == -1){
		err = -10;
//	#ifdef VME_DEBUG
	ErrCode(true);
//	#endif
		return -1;
	}
	#ifdef VME_DEBUG 
	printf("\nWRITE:   Send to Controller:\n");
//	printf("res      : %4d\n", res );
	printf("comm     : %4d\n", comm );
//	printf("mess_len : %4d\n", mess_len );
//	printf("hd       : 0x%x\n", hd );
	for(int j = 0; j < mess_len; j++) {
	printf("buf[%d] = 0x%x\n",j , buf[j] );
	}
//	printf("=========================\n");
	#endif
	
	



    res = RDbuf( buf, HEADER_LEN+STAT_LEN, 0, nsid );

    if( res == HEADER_LEN+STAT_LEN ) { 
	memcpy( &hd, &buf[0], 2 );
	memcpy( &comm, &buf[2], 1 );
	memcpy( &Stat,  &buf[3], 1 );

//	if ( ( Addr&0x400000 ) == 0x400000 ) {
//	    comm-=16;
//	}
//	else {
//	    comm-=add;
//	}

	#ifdef VME_DEBUG
		printf( " WRITE::Answer from Controller : \n");
		printf( "    hd_out   = 0x%x\n", hd ); 
		printf( "    command  = 0x%x(%d)\n", comm,comm );
		printf( "    Stat     = 0x%x\n", Stat ); 
//		for(int j = 0; j < HEADER_LEN+STAT_LEN; j++) {
//		for(int j = 0; j < HEADER_LEN+STAT_LEN+NumBytes; j++) {
//			printf("buf[%d] = 0x%x\n",j , buf[j] );
//		}
	#endif


		//check answer
		if (hd != HD){
			err = -7;
			ErrCode(true);
			return -1;
		}
		if (comm != (CMD_WR+add)){
			err = -8;
			ErrCode(true);
			return -1;
		}
		if (Stat != STAT_OK){
			err = -9;
			ErrCode(true);
//			printf("<VME::WR> Addr=%x\n",GlobalAddr);
			printf("<VME::WR> Clast=%i, Addr=%x\n", add+1, Addr );
//			fprintf(stderr,"<VME::WR> GlobalAddr=%x,  Clast=%i, Addr=%x\n\n",GlobalAddr, add+1, Addr );
	ferr = fopen("//home//DATA//T133//errors.txt","a");
	fprintf(ferr,"<VME::WR> Stat= %x  Clast=%i, Addr=%lx\n", Stat, add+1, Addr );
	fclose(ferr);



			return -1;
//			kk++;
//			if(kk>10) {
//			    printf("<VME::WR>return -1\n");
//			    return -1;
//			}
//			printf("!!!!!!!!!!!!!  kk=%i\n",kk);
//			goto QQQ;
		}


		return 0;

	}  //  if HEADER+STAT is OK




/*
int j=0;
int sel=0;
//		printf( "\n Answer from Controller : \n");		   


	do {  //reading answer from controller


	    timeRec.tv_sec  = 1;
	    timeRec.tv_usec = 1000; 
		
		FD_ZERO( &read_set ); 
	    FD_ZERO( &write_set );
		
		FD_SET( nsid, &read_set ); //добавляем дискрипторы для функции select
		
		sel = select( FD_SETSIZE, &read_set, NULL, NULL, &timeRec );
		
		if( FD_ISSET( nsid, &read_set ) ) { //проверка готов ли какой-нибудь сокет для чтения
			

		if( ( res = sc->read( buf, HEADER_LEN+STAT_LEN, 0, nsid ) ) == HEADER_LEN+STAT_LEN ) { 
			memcpy( &hd, &buf[0], 2 );
			memcpy( &comm, &buf[2], 1 );
			memcpy( &Stat,  &buf[3], 1 );
			

	if ( ( Addr&0x400000 ) == 0x400000 ) {
	    comm-=16;
	}
//	else {
//	    comm-=(int)(Addr>>23);
//	}


			#ifdef VME_DEBUG
			printf( " WRITE::Output from Controller : \n");		   
			printf( "    hd_out   = 0x%x\n", hd ); 
			printf( "    command  = 0x%x(%d)\n", comm,comm );
			printf( "    Stat     = 0x%x\n", Stat ); 
			#endif



//for(;;) {

//
//		res = sc->read( buf, 1, 0, nsid );  
//		printf("buf[%d] = 0x%x       select=%i\n",j , buf[0], sel );
//		printf("buf[%d] = 0x%x\n",j , buf[0] );
//		j++;




			//check answer
			if (hd != HD){
				err = -7;
				ErrCode(true);
//				return -1;
			}
			if (comm != CMD_WR){
				err = -8;
				ErrCode(true);
//				return -1;
			}
			if (Stat != STAT_OK){
				err = -9;
				ErrCode(true);
//				return -1;
			}
			return 0;

		}  //  read
	}   //  FD_ISSET
		conn_state++;
	} while (conn_state <10);

*/


	//answer from controller lost
	err = -6;
	ErrCode(true);
return -1;
}
//====================================================================
int VME::RDbuf(unsigned char *buf, int NumBytes, int flag, int nsid )
{
int flagsh=0;
int SumBytes=0;
int conn_state =0;

//AAA:

for(int j=0;j<NumBytes;j++) {
    buf[j]=0;
}

    timeRec.tv_sec  = 59;
    timeRec.tv_usec = 0; 
		
    FD_ZERO( &read_set ); 
    FD_ZERO( &write_set );
		
    FD_SET( nsid, &read_set ); //добавляем дискрипторы для функции select
		
    int retval = select( FD_SETSIZE, &read_set, NULL, NULL, &timeRec );
    
    if (retval) {  // answer from controller
		

        if( FD_ISSET( nsid, &read_set ) ) { // who is answer ?
			
//if ( NumBytes>100) {
//	SumBytes = sc->read( buf, NumBytes-6, flag, nsid );
//}
//else {
	SumBytes = sc->read( buf, NumBytes, flag, nsid );
//}
//	    printf("retval=%i -- ZAPROS OK  \n",retval);

        while(SumBytes!=NumBytes) {
	    printf("NOT ALL BYTES::  res=%i  NumBytes=%i\n",SumBytes,NumBytes);
	    SumBytes += sc->read( buf+SumBytes, NumBytes-SumBytes, 0, nsid );
	}
	}
//goto AA2;

    }
    else {   //  time out or error

//AA2:
	printf("retval=%i     \n",retval);
	err = -20;
	ErrCode(true);

/*
	if ( !flagsh ) {
	
	flagsh=1;
	
	sc->shutdown();
//	delete(sc);

//	close(sc);

//	ConnectVME();
	

sleep(2);


	if( !sc->good() ) {
		err = -2;
		#ifdef VME_DEBUG 
		ErrCode(true);
		#endif
		return -1;
	}




	sc_fd = sc->fd();



    rd = setsockopt( sc_fd, SOL_SOCKET, SO_DONTROUTE, &val, sizeof ( val ) );    
	rd = setsockopt( sc_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof ( val ) );    


        if( sc->bind(host, Socket_Port ) < 0 ) { 
//		nrerror( "Bind error ....... ");     
		printf( "Bind error ....... \n");     
			err = -3;
			#ifdef VME_DEBUG
			ErrCode(true);
			#endif
			return -1;
		}
		


    
    //    printf("Bind ok !!!!\n");
    
	//for( i = 0; i < Real_BPI_num; i++ ) {//в цикле пытаемся подключиться к контроллеру
	//  printf("connected[i] = %d\n", connected[i] );
	  conn_state = 0;
	  while ((connected == false) && (conn_state<10))  { 	
             conn_state++; 	  

	     timeRec.tv_sec  = 1;  // 1 second polling for controller connections 
             timeRec.tv_usec = 0;
	     
	     FD_SET( sc_fd, &select_set );
	     rc = select( FD_SETSIZE, &select_set, 0, 0, &timeRec );	

//             printf("select ...... rc = %d conn_state = %d\n",rc, conn_state );

	     if(( rc > 0 ) && ( FD_ISSET( sc_fd, &select_set ) ) ) {
	       if(( nsid = sc->accept( 0 , 0 ) ) < 0 ) {
	         //printf( "Accept Error!\n" ); return;
			   err = -4;
			   #ifdef VME_DEBUG
				ErrCode(true);
				#endif
			   return -1;
		}
//	       else {
	         printf( "Accept Ok! \nrc = %d nsid = %d\n", rc, nsid );
//		} 
	       connected = true;	
	      }
		  if ((conn_state == 10) && ( connected == false)) {
			err = -5;
			#ifdef VME_DEBUG
			ErrCode(true);
			#endif
			return -1;
		  }
 	   }
	 //}	




	goto AAA;


	}
*/
	return -20;
    }
return SumBytes;
}
//====================================================================
int VME::RD(longint Addr, word NumBytes, char *Data)
{
int kk=0;
int add=0;
long GlobalAddr=0L;

	GlobalAddr=Addr;


for(int j=0;j<NumBytes;j++) {
    Data[j]=0;
}


QQQ:
	mess_len = 0;    
	hd = HD;
	comm = CMD_RD;

//printf("%x\n",Addr);

	if ( ( Addr&0x400000 ) == 0x400000 ) {
	    add=16;
	}
	else {
	    add = (int)(Addr>>23);
	}
	
	int conn_state =0;
	

//comm=128+2;
	    comm+= add;
	    Addr&=0x7fffff;
//	printf("Addr       : 0x%lx\n", Addr );



	memcpy( &buf[0], &hd, 2 );         // 0xFFFF
	memcpy( &buf[2], &comm, 1 );       // command
	memcpy( &buf[3], &Addr, 4 ); 		//first addr for write
	memcpy( &buf[7], &NumBytes, 2 ); 	//bytes for transfer
	
	
	mess_len = HEADER_LEN+ADDR_LEN+BFT_LEN;
	
	res = sc->write( buf, mess_len, 0, nsid ); //sending command message
	if (res == -1){
		err = -15;
//	#ifdef VME_DEBUG
	ErrCode(true);
//	#endif
		return -1;
	}

//	printf("addr =%x   comm     : %4d i\n", Addr, comm );


	#ifdef VME_DEBUG 
	printf("\nREAD:   Send to Controller:\n");
//	printf("res      : %4d i\n", res );
	printf("comm     : 0x%x(%d)\n", comm,comm );
//	printf("mess_len : %4d i\n", mess_len );
//	printf("hd       : 0x%x\n", hd );
	for(int j = 0; j < mess_len; j++) {
	printf("buf[%d] = 0x%x\n",j , buf[j] );
	}
	#endif
	
	


    res = RDbuf( buf, HEADER_LEN+STAT_LEN, 0, nsid );

//printf("qqqqqqqq\n");
    if( res == HEADER_LEN+STAT_LEN ) { 
	memcpy( &hd, &buf[0], 2 );
	memcpy( &comm, &buf[2], 1 );
	memcpy( &Stat,  &buf[3], 1 );

//	if ( ( Addr&0x400000 ) == 0x400000 ) {
//	    comm-=16;
//	}
//	else {
//	    comm-=add;
//	}

	#ifdef VME_DEBUG
		printf( " READ::Output from Controller : \n");
		printf( "    hd_out   = 0x%x\n", hd ); 
		printf( "    command  = 0x%x(%d)\n", comm,comm );
		printf( "    Stat     = 0x%x\n", Stat ); 
//		for(int j = 0; j < HEADER_LEN+STAT_LEN; j++) {
//		for(int j = 0; j < HEADER_LEN+STAT_LEN+NumBytes; j++) {
//			printf("buf[%d] = 0x%x\n",j , buf[j] );
//		}
	#endif


		//check answer
		if (hd != HD){
			err = -12;
			ErrCode(true);
			return -1;
		}
		if (comm != (CMD_RD+add)){
			err = -13;
			ErrCode(true);
			return -1;
		}
		if (Stat != STAT_OK){
			err = -14;
			ErrCode(true);
//			printf("<VME::RD> Addr=%x\n",GlobalAddr);
			printf("<VME::RD> Clast=%i, Addr=%x\n", add+1, Addr );
//			fprintf(stderr,"<VME::RD> GlobalAddr=%x,  Clast=%i, Addr=%x\n\n",GlobalAddr, add+1, Addr );
	ferr = fopen("//home//DATA//T133//errors.txt","a");
	fprintf(ferr,"<VME::RD> Stat= %x  Clast=%i, Addr=%lx\n", Stat, add+1, Addr );
	fclose(ferr);

			return -1;
/*
			kk++;
			if(kk>10) {
			    printf("<VME::RD>return -1\n");
			    return -1;
			}
			printf("!!!!!!!!!!!!!  kk=%i\n",kk);
//			goto QQQ;
*/
		}


		//take data from Socket_Port
		res = RDbuf( (unsigned char *)Data, NumBytes, 0, nsid );
		if ( res == -20 ) { 
			err = -11;
			ErrCode(true);
//		exit(0);
			return -1;
		}
		if ( res!= NumBytes ) {
		    printf("!!!!!!!!!!!!   <VME::RD> - error read NumBytes\n");
			return -1;
		}

/*
		for(int j = 0; j < NumBytes; j++) {
			printf("data[%d] = 0x%x\n",j , Data[j] );
		}
*/

		return 0;

	}  //  if HEADER+STAT is OK


	//answer from controller lost
        err = -16;
	ErrCode(true);
return -1;
}
//====================================================================
int VME::ErrCode(bool PrErr) //return error code and print error
{
if (PrErr)
{

	switch(err)
	{
	case -2://error during creating socket
			fprintf( stderr, "Error during creating socket\n");
//			exit(1);
		break;
	case -3://bind error
			fprintf( stderr, "Bind error\n");
//			exit(1);
		break;
	case -4://accept error
			fprintf( stderr, "Accept error\n");
//			exit(1);
		break;
	case -5://accept timeout error
			fprintf( stderr, "Accept timeout expired\n");
//			exit(1);
		break;
	case -6://lost answer from controller
			fprintf( stderr, "<VME::WR> - no answer from controller\n");
//			exit(1);
		break;
	case -7://error in returned packet header
			fprintf( stderr, "<VME::WR> - error in Header\n");
//			exit(1);
		break;
	case -8://error in returned packet command
			fprintf( stderr, "<VME::WR> - error in Command\n");
//			exit(1);
		break;
	case -9://error in returned packet status
			fprintf( stderr, "<VME::WR> - error in Status; Stat= %x\n",Stat);
//			exit(1);
		break;
	case -10://error write to socket
			fprintf( stderr, "<VME::WR> - error in writing to socket");
//			exit(1);
		break;
	case -11://error reading data
			fprintf( stderr, "<VME::RD> - error in reading data from socket");
//			exit(1);
		break;
	case -12://error in returned packet header
			fprintf( stderr, "<VME::RD> - error in Header\n");
//			exit(1);
		break;
	case -13://error in returned packet command
			fprintf( stderr, "<VME::RD> - error in Command\n");
//			exit(1);
		break;
	case -14://error in returned packet status
			fprintf( stderr, "<VME::RD> - error in Status; Stat= %x\n",Stat);
//			exit(1);
		break;
	case -15://error write to socket
			fprintf( stderr, "<VME::WR> - error in writing to socket");
//			exit(1);
		break;
	case -16://lost answer from controller
			fprintf( stderr, "<VME::RD> - no answer from controller\n");
//			exit(1);
		break;
	case -20:// Time Out on Read
			fprintf( stderr, "<VME::RDbuf> - Time Out on Read Buffer\n");
//			exit(1);
		break;
	default:fprintf( stderr, "Unknown error\n");
//			exit(1);
		break;
	}
}

	return err;
}
//====================================================================
int VME::WRpack(word NumBytes, char *Data)//write data
{
		
	mess_len = 0;    
	hd = HD;
	comm = CMD_WR;
	
	int conn_state =0;
	
	//memcpy( &buf[0], &hd, 2 );         // 0xFFFF
	//memcpy( &buf[2], &comm, 1 );       // command
	//memcpy( &buf[3], &Addr, 4 ); 		//first addr for write
	//memcpy( &buf[7], &NumBytes, 2 ); 	//bytes for transfer
	memcpy( &buf[0], Data, NumBytes);
	
	//mess_len = HEADER_LEN+ADDR_LEN+BFT_LEN+NumBytes;
	mess_len = NumBytes;
	res = sc->write( buf, mess_len, 0, nsid ); //sending command message
	if (res == -1){
		err = -10;
	#ifdef VME_DEBUG
	ErrCode(true);
	#endif
		return -1;
	}
	#ifdef VME_DEBUG 
	printf("res      : %4d\n", res );
	#endif
return 0;
}
//====================================================================
int VME::RDpack(word NumBytes, char *Data)//write data
{


//    res = RDbuf( (unsigned char *)Data, NumBytes, 0, nsid );
//    if ( res == NumBytes) return 0;




	
	int conn_state =0;

	do {  //reading answer from controller
	    timeRec.tv_sec  = 0;
	    timeRec.tv_usec = 1000; 
		
		FD_ZERO( &read_set ); 
	    FD_ZERO( &write_set );
		


		FD_SET( nsid, &read_set ); //добавляем дискрипторы для функции select
		
		select( FD_SETSIZE, &read_set, NULL, NULL, &timeRec );
		


		if( FD_ISSET( nsid, &read_set ) ) { //проверка готов ли сокет для чтения

			

			//take data from Socket_Port
			if( ( res = sc->read( (unsigned char*)Data, NumBytes, 0, nsid ) ) == -1 ) { 

				err = -11;
				#ifdef VME_DEBUG
				ErrCode(true);
				#endif
				return -1;
			}
			
			return 0;
		
		}
		conn_state++;
	} while (conn_state <1);


	//answer from controller lost
	err = -16;
	ErrCode(true);
return -1;
}
//====================================================================
int VME::ConnectClient()
{
int conn_state =0;
	
 FD_ZERO( &select_set );



        sc = new Socket( Socket_Port != -1 ? AF_INET : AF_UNIX, SOCK_STREAM );
    

	if( !sc->good() ) {
		err = -2;
		#ifdef VME_DEBUG 
		ErrCode(true);
		#endif
		return -1;
	}



	sc_fd = sc->fd();



    rd = setsockopt( sc_fd, SOL_SOCKET, SO_DONTROUTE, &val, sizeof ( val ) );    
	rd = setsockopt( sc_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof ( val ) );    
	
	//printf("rd = %d  sc_fd = %d\n", rd, sc_fd );


/*
        if( sc->connect(host, Socket_Port ) < 0 ) { //	nrerror( "Bind error ....... ");     
			err = -3;
			#ifdef VME_DEBUG
			ErrCode(true);
			#endif
			return -1;
		}
		
*/

    
    //    printf("Bind ok !!!!\n");
    
	//for( i = 0; i < Real_BPI_num; i++ ) {//в цикле пытаемся подключиться к контроллеру
	//  printf("connected[i] = %d\n", connected[i] );
	  conn_state = 0;
	  while ((connected == false) && (conn_state<10))  { 	
             conn_state++; 	  

	     timeRec.tv_sec  = 1;  // 1 second polling for controller connections 
             timeRec.tv_usec = 0;
	     
	     FD_SET( sc_fd, &select_set );
	     rc = select( FD_SETSIZE, &select_set, 0, 0, &timeRec );	

//             printf("select ...... rc = %d conn_state = %d\n",rc, conn_state );



	     if(( rc > 0 ) && ( FD_ISSET( sc_fd, &select_set ) ) ) {
printf("host=%s   port=%i\n",host,Socket_Port);

	       if(( nsid = sc->connect( host , Socket_Port ) ) < 0 ) {
	    	  printf( "Connect Error!\n" ); //return;
			   err = -4;
			   #ifdef VME_DEBUG
				ErrCode(true);
				#endif
//			   return -1;
		}
	       else {
	         printf( "Connect Ok! \nrc = %d nsid = %d\n", rc, nsid );
//		} 
	       connected = true;	
	       }
	      }
		  if ((conn_state == 10) && ( connected == false)) {
			err = -5;
			#ifdef VME_DEBUG
			ErrCode(true);
			#endif
			return -1;
		  }
 	   }
	 //}	



	return 0;
}
//====================================================================
int VME::AnswerClient()
{
unsigned long Addr=0L;
unsigned char data[2048];
int NumBytes=0;
int sel=0;

//printf("qqqqqqqqqqqqqqqqq\n");

	    timeRec.tv_sec  = 5;
	    timeRec.tv_usec = 0; 

	    FD_ZERO( &read_set ); 
	    FD_ZERO( &write_set );

            FD_SET( nsid, &read_set );


	    sel = select( FD_SETSIZE, &read_set, NULL, NULL, &timeRec );                
	    printf("sel=%i   nsid=%i\n",sel,nsid);


                res = sc->read( buf, 9, 0, nsid );
printf("res=%i\n",res);
	      if( FD_ISSET( nsid, &read_set ) ) {

                res = sc->read( buf, 9, 0, nsid );
printf("res=%i\n",res);

                if ( res >=0 ) { // 1 



    memcpy(&hd,&buf[0],2);
    memcpy(&comm,&buf[2],1);
    memcpy(&Addr,&buf[3],4);
    memcpy(&NumBytes,&buf[7],2);

    Stat=STAT_OK;

	memcpy(&buf[3],&Stat,1);
        sc->write( buf, 4, 0, nsid ); 

    if (comm==128 || comm==129 || comm==130 || comm==131 || comm==144) {

        sc->write( data, NumBytes, 0, nsid ); 

    }    


//		for(j=0;j<res;j++) {
//                    printf("buf[%d] = %4x(hex) %4d(dec)\n",j , buf[j], buf[j] );
//		}


	       }  //read
	     }   // FD_ISSET


//printf("Press s or S\n");


return 0;
}
//====================================================================
