#include "vme.h"

int VME::ConnectVME()
{
	int conn_state =0;
	FD_ZERO( &select_set );
	sc = new Socket( Socket_Port != -1 ? AF_INET : AF_UNIX, SOCK_STREAM );

	if( !sc->good() )
	{
		err = -2;
		ErrCode(true);
		return -1;
	}
	sc_fd = sc->fd();
	rd = setsockopt( sc_fd, SOL_SOCKET, SO_DONTROUTE, &val, sizeof ( val ) );
	rd = setsockopt( sc_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof ( val ) );
	printf("rd = %d  sc_fd = %d\n", rd, sc_fd );
	if( sc->bind(host, Socket_Port ) < 0 )
	{
		printf( "Bind error ....... \n");
		err = -3;
		ErrCode(true);
		return -1;
	}
	printf("Bind ok !!!!\n");
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
				err = -4;
				ErrCode(true);
			}
			printf( "Accept Ok! \nrc = %d nsid = %d\n", rc, nsid );
			connected = true;
		}
		if ((conn_state == 10) && ( connected == false)) {
			err = -5;
			ErrCode(true);
			return -1;
		}
	}
	return 0;
}
//====================================================================
int VME::WR(longint Addr, word NumBytes, char *Data)//write data
{
	int add=0;

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
	memcpy( &buf[0], &hd, 2 );         // 0xFFFF
	memcpy( &buf[2], &comm, 1 );       // command
	memcpy( &buf[3], &Addr, 4 ); 		//first addr for write
	memcpy( &buf[7], &NumBytes, 2 ); 	//bytes for transfer
	memcpy( &buf[9], Data, NumBytes);

	mess_len = HEADER_LEN+ADDR_LEN+BFT_LEN+NumBytes;

	res = sc->write( buf, mess_len, 0, nsid ); //sending command message
	if (res == -1)
	{
		err = -10;
		ErrCode(true);
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
	res = RDbuf(buf, HEADER_LEN+STAT_LEN, 0, nsid);

	if( res == HEADER_LEN+STAT_LEN )
	{
		memcpy( &hd, &buf[0], 2 );
		memcpy( &comm, &buf[2], 1 );
		memcpy( &Stat,  &buf[3], 1 );
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
		if (hd != HD)
		{
			err = -7;
			ErrCode(true);
			return -1;
		}
		if (comm != (CMD_WR+add))
		{
			err = -8;
			ErrCode(true);
			return -1;
		}
		if (Stat != STAT_OK)
		{
			err = -9;
			ErrCode(true);
			printf("<VME::WR> Clast=%i, Addr=%x\n", add+1, Addr );
			ferr = fopen("//home//DATA//T133//errors.txt","a");
			fprintf(ferr,"<VME::WR> Stat= %x  Clast=%i, Addr=%lx\n", Stat, add+1, (unsigned long)Addr );
			fclose(ferr);
			return -1;
		}
		return 0;
	}
	//answer from controller lost
	err = -6;
	ErrCode(true);
	return -1;
}
//====================================================================
int VME::RDbuf(unsigned char *buf, int NumBytes, int flag, int nsid )
{
	int SumBytes=0;

	//AAA:

	for(int j=0;j<NumBytes;j++) {
		buf[j]=0;
	}

	timeRec.tv_sec  = 59;
	timeRec.tv_usec = 0;

	FD_ZERO( &read_set );
	FD_ZERO( &write_set );

	FD_SET( nsid, &read_set );

	int retval = select( FD_SETSIZE, &read_set, NULL, NULL, &timeRec );

	if (retval) {  // answer from controller
		if( FD_ISSET( nsid, &read_set ) ) { // who is answer ?
			SumBytes = sc->read( buf, NumBytes, flag, nsid );
			while(SumBytes!=NumBytes) {
				printf("NOT ALL BYTES::  res=%i  NumBytes=%i\n",SumBytes,NumBytes);
				SumBytes += sc->read( buf+SumBytes, NumBytes-SumBytes, 0, nsid );
			}
		}
	}
	else {   //  time out or error
		printf("retval=%i     \n",retval);
		err = -20;
		ErrCode(true);
		return -20;
	}
	return SumBytes;
}
//====================================================================
int VME::RD(longint Addr, word NumBytes, char *Data)
{
	int add=0;

	for(int j=0;j<NumBytes;j++)
	{
		Data[j]=0;
	}
	mess_len = 0;
	hd = HD;
	comm = CMD_RD;
	if ( ( Addr&0x400000 ) == 0x400000 )
	{
		add=16;
	}
	else
	{
		add = (int)(Addr>>23);
	}
	comm+= add;
	Addr&=0x7fffff;
	memcpy( &buf[0], &hd, 2 );         // 0xFFFF
	memcpy( &buf[2], &comm, 1 );       // command
	memcpy( &buf[3], &Addr, 4 ); 		//first addr for write
	memcpy( &buf[7], &NumBytes, 2 ); 	//bytes for transfer
	mess_len = HEADER_LEN+ADDR_LEN+BFT_LEN;
	res = sc->write( buf, mess_len, 0, nsid ); //sending command message
	if (res == -1)
	{
		err = -15;
		ErrCode(true);
		return -1;
	}
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
	if( res == HEADER_LEN+STAT_LEN )
	{
		memcpy( &hd, &buf[0], 2 );
		memcpy( &comm, &buf[2], 1 );
		memcpy( &Stat,  &buf[3], 1 );

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
		if (hd != HD)
		{
			err = -12;
			ErrCode(true);
			return -1;
		}
		if (comm != (CMD_RD+add))
		{
			err = -13;
			ErrCode(true);
			return -1;
		}
		if (Stat != STAT_OK)
		{
			err = -14;
			ErrCode(true);
			printf("<VME::RD> Clast=%i, Addr=%x\n", add+1, Addr );
			ferr = fopen("//home//DATA//T133//errors.txt","a");
			fprintf(ferr,"<VME::RD> Stat= %x  Clast=%i, Addr=%lx\n", Stat, add+1, (unsigned long)Addr );
			fclose(ferr);
			return -1;
		}
		res = RDbuf( (unsigned char *)Data, NumBytes, 0, nsid );
		if ( res == -20 )
		{
			err = -11;
			ErrCode(true);
			return -1;
		}
		if ( res!= NumBytes )
		{
			printf("!!!!!!!!!!!!   <VME::RD> - error read NumBytes\n");
			return -1;
		}
		return 0;
	}
	err = -16;
	ErrCode(true);
	return -1;
}
//====================================================================
int VME::ErrCode(bool PrErr)
{
	if (PrErr)
	{
		switch(err)
		{
		case -2://error during creating socket
			fprintf( stderr, "Error during creating socket\n");
			break;
		case -3://bind error
			fprintf( stderr, "Bind error\n");
			break;
		case -4://accept error
			fprintf( stderr, "Accept error\n");
			break;
		case -5://accept timeout error
			fprintf( stderr, "Accept timeout expired\n");
			break;
		case -6://lost answer from controller
			fprintf( stderr, "<VME::WR> - no answer from controller\n");
			break;
		case -7://error in returned packet header
			fprintf( stderr, "<VME::WR> - error in Header\n");
			break;
		case -8://error in returned packet command
			fprintf( stderr, "<VME::WR> - error in Command\n");
			break;
		case -9://error in returned packet status
			fprintf( stderr, "<VME::WR> - error in Status; Stat= %x\n",Stat);
			break;
		case -10://error write to socket
			fprintf( stderr, "<VME::WR> - error in writing to socket");
			break;
		case -11://error reading data
			fprintf( stderr, "<VME::RD> - error in reading data from socket");
			break;
		case -12://error in returned packet header
			fprintf( stderr, "<VME::RD> - error in Header\n");
			break;
		case -13://error in returned packet command
			fprintf( stderr, "<VME::RD> - error in Command\n");
			break;
		case -14://error in returned packet status
			fprintf( stderr, "<VME::RD> - error in Status; Stat= %x\n",Stat);
			break;
		case -15://error write to socket
			fprintf( stderr, "<VME::WR> - error in writing to socket");
			break;
		case -16://lost answer from controller
			fprintf( stderr, "<VME::RD> - no answer from controller\n");
			break;
		case -20:// Time Out on Read
			fprintf( stderr, "<VME::RDbuf> - Time Out on Read Buffer\n");
			break;
		default:fprintf( stderr, "Unknown error\n");
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
	memcpy( &buf[0], Data, NumBytes);
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
	int conn_state =0;

	do {  //reading answer from controller
		timeRec.tv_sec  = 0;
		timeRec.tv_usec = 1000;
		FD_ZERO( &read_set );
		FD_ZERO( &write_set );
		FD_SET( nsid, &read_set );
		select( FD_SETSIZE, &read_set, NULL, NULL, &timeRec );
		if( FD_ISSET( nsid, &read_set ) )
		{
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
	conn_state = 0;
	while ((connected == false) && (conn_state<10))  {
		conn_state++;

		timeRec.tv_sec  = 1;  // 1 second polling for controller connections
		timeRec.tv_usec = 0;

		FD_SET( sc_fd, &select_set );
		rc = select( FD_SETSIZE, &select_set, 0, 0, &timeRec );
		if(( rc > 0 ) && ( FD_ISSET( sc_fd, &select_set ) ) ) {
			printf("host=%s   port=%i\n",host,Socket_Port);

			if(( nsid = sc->connect( host , Socket_Port ) ) < 0 ) {
				printf( "Connect Error!\n" ); //return;
				err = -4;
#ifdef VME_DEBUG
				ErrCode(true);
#endif
			}
			else {
				printf( "Connect Ok! \nrc = %d nsid = %d\n", rc, nsid );
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
	return 0;
}
//====================================================================
int VME::AnswerClient()
{
	unsigned long Addr=0L;
	unsigned char data[2048];
	int NumBytes=0;
	int sel=0;
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

			if (comm==128 || comm==129 || comm==130 || comm==131 || comm==144)
			{
				sc->write( data, NumBytes, 0, nsid );
			}
		}  //read
	}   // FD_ISSET
	return 0;
}
