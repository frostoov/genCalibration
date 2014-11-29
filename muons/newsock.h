#ifndef NEWSOCK_H
#define NEWSOCK_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <sys/sysinfo.h>

const int BACKLOG_NUM = 5;

class sock
{
private :

	int sid;
	int domain;
	int socktype;
	int rc;

	int constr_name(struct sockaddr_in& addr, const char* hostnm, int port);
	int constr_name(struct sockaddr& addr, const char* Pathnm);
	char* ip2name(const struct in_addr in);

public:
	sock( int dom, int type, int protocol = 0 ) : domain(dom),socktype(type)
	{
		if((sid = socket(domain = dom, socktype = type, protocol)) < 0) perror("Socket error ......");
	}
	int Close_sock();
	~sock();
	int fd();
	int good();
	int bind(const char* name, int port=-1);
	int accept(char* name, int* port_p);
	int connect(const char* hostnm, int port = -1);
	int write(unsigned char* buf, int len, int flag = 0,int nsid = -1);
	int read(unsigned char* buf, int len, int flag = 0, int nsid = -1);
	int writeto(unsigned char* buf, int len, int flag, const char* name, const int port, int nsid = -1);
	int readfrom(unsigned char* buf, int len, int flag, char* name, int *port_p, int nsid = -1);
	int shutdown(int mode = 2);
};

#endif // NEWSOCK_H
