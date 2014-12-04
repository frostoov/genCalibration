#ifndef __VME_H
#define __VME_H

#include <sys/types.h>
#include <signal.h>
#include <sys/io.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
   
#include "sock.h"
#include "globals_clT.h"

#define         HEADER_LEN          	3//bytes
#define			ADDR_LEN				4 //bytes
#define			BFT_LEN					2// BYTES_FOR_TRANSFER length
#define			STAT_LEN				1//status bytes
//#define 		MAX_RD_BUFFER_LEN		0xA000//bytes


typedef sock Socket;


class VME
{
public:

	VME(){
		connected = false;
		SyncFlag = 0;
		host = "192.168.1.89";
		Socket_Port = 5858;
		val = 1;
		mess_len = 0;
		packet_length = 0;
		hd = HD;
//		SetHost(host,sizeof(host));
	};

	~VME(){ free(host); UnConnectVME();};

	int ErrCode(bool PrErr); //return error code and print error if PrErr = true
	byte GetStat(){return Stat;};//return status from read Packet
	bool GetConnected(){ return connected;};//return connected flag
	
	void SetHost(const char *Host, byte SizeByte)
	{ 
	printf("host=%s   size=%i\n",host,SizeByte);
//	free(host);
	host = (char*)malloc(SizeByte);
	strcpy(host,Host);
	printf("!!!!!!!!!!!!!host=%s\n",host);	
	};
	void SetSocketPort(int Sock_Port) {
//	printf("qqqqqqqqq\n");
	Socket_Port = Sock_Port;
	};

	int SockID(){ return nsid;};//return socket's nsid
	void SockSHDN(){sc->shutdown();};//shutdown socket

	int ConnectVME();

	int UnConnectVME(void) {
//	    free(sc);
//	    sc->Close_sock();
		connected = false;
	    close(nsid);
	    delete sc;
	    return 0;
	};

	int GetSync(void) {
	    return SyncFlag;
	};
	void ClearSync() {
	    SyncFlag=0;
	
	};
	void SetSyncErr() {
	    SyncFlag=1;
	
	};

	int ConnectClient();
	
	int WR(longint Addr, word NumBytes, char *Data);//write data
	int WRpack(word NumBytes, char *Data);//Data will be written to socket
	int RD(longint Addr, word NumBytes, char *Data);//read data
	int RDbuf(unsigned char *buf, int NumBytes, int flag, int nsid);//read data iat buf
	int RDpack(word NumBytes, char *Data);//Data will be read from socket
	int AnswerClient();
	


private:
	
	int err;//error code from last operation
	byte Stat;//status from last r/w operation

	bool connected;
	Socket *sc;//pointer to socket for that VME

	char *host;
	int Socket_Port;

	int SyncFlag;

	int              sc_fd;
	int              val, rd;
	int              rc, nsid, res;
    fd_set           select_set;
    fd_set           read_set, write_set;

	struct timeval   timeRec;

	byte buf[64];
	word mess_len;
	u_int16_t packet_length;
	int hd;//header
	byte comm;
};

#endif
