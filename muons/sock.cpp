//#include "sock.h"
#include "newsock.h"

int sock::constr_name(struct sockaddr_in& addr, const char* hostnm, int port)
{
	addr.sin_family = domain;
	if(!hostnm)
		addr.sin_addr.s_addr = INADDR_ANY;
	else
	{
		struct hostent *hp = gethostbyname(hostnm);
		memcpy((char*)&addr.sin_addr,(char*)hp->h_addr,hp->h_length);
	}
	addr.sin_port = htons(port);

	return sizeof(addr);
}

int sock::constr_name(struct sockaddr& addr, const char* Pathnm)
{

	addr.sa_family = domain;
	strcpy(addr.sa_data,Pathnm);

	return sizeof(addr.sa_family) + strlen(Pathnm) + 1;
}

char* sock::ip2name(const struct in_addr in)
{

	u_long laddr;

	if((int)(laddr = inet_addr(inet_ntoa(in))) == -1)
		return 0;
	struct hostent *hp = gethostbyaddr((char *)&laddr,sizeof (laddr),AF_INET);
	if (hp == NULL)
		return 0;
	for (char **p = hp->h_addr_list; *p != 0; p++)
	{
		(void) memcpy((char*)&in.s_addr, *p, sizeof (in.s_addr));
		if (hp->h_name)
			return hp->h_name;
	}
	return 0;
}

int sock::Close_sock()
{
	shutdown();
	close(sid);
	return 0;
}

sock::~sock()
{
	shutdown();
	close(sid);
}

int sock::fd()
{
	return sid;
}

int sock::good()
{
	return sid >= 0;
}

int sock::bind(const char* name, int port)
{
	if(port == -1)
	{
		struct sockaddr addr;
		int len = constr_name(addr,name);
		if((rc = ::bind(sid,&addr,len)) < 0)
			perror("Bind error....");
	}
	else
	{
		struct sockaddr_in addr;
		int len = constr_name(addr,name,port);
		if((rc = ::bind(sid,(struct sockaddr *)&addr, len)) < 0 ||
				(rc = getsockname(sid, (struct sockaddr *)&addr,(socklen_t *)&len)) < 0)
			perror("Bind or getsockname error....");
		else
			fprintf(stderr,"Socket port : %d\n", ntohs(addr.sin_port));
	}
	if((rc != -1) && (socktype != SOCK_DGRAM) && (rc = listen(sid,BACKLOG_NUM)) < 0)
		perror("Listen error....");
	return rc;
}

int sock::accept(char* name, int* port_p) {

	if(!name) return ::accept(sid,0,0);
	if(!port_p || *port_p == -1) {
		struct sockaddr addr;
		int size = sizeof(addr);
		if ((rc = ::accept(sid,&addr,(socklen_t *)&size)) > -1)
			strncpy(name,addr.sa_data,size);name[size] = '\0';
	}
	else {
		struct sockaddr_in addr;
		int size = sizeof (addr);
		if((rc = ::accept(sid,(struct sockaddr*)&addr,(socklen_t *)&size)) > -1) {
			if(name) strcpy(name,ip2name(addr.sin_addr));
			if(port_p) *port_p = ntohs(addr.sin_port);
		}
	}

	return rc;
}

int sock::connect(const char* hostnm, int port) {

	if (port == -1) {
		struct sockaddr addr;
		int len = constr_name(addr,hostnm);
		if((rc = ::connect(sid,&addr,len)) < 0) perror("Bind error....");
	}
	else {
		struct sockaddr_in addr;
		int len = constr_name(addr,hostnm,port);
		if((rc = ::connect(sid,(struct sockaddr *)&addr,len)) < 0) perror("Bind error....");
	}

	return rc;
}

int sock::write(unsigned char* buf, int len, int flag,int nsid) {

	return ::send(nsid == -1 ? sid : nsid, buf, len, flag);
}

int sock::read(unsigned char* buf, int len, int flag, int nsid) {

	return ::recv(nsid == -1 ? sid : nsid, buf, len, flag);
}

int sock::writeto(unsigned char* buf, int len, int flag, const char* name, const int port, int nsid) {

	if(port == -1) {
		struct sockaddr addr;
		int size = constr_name(addr,name);
		return ::sendto(nsid == -1 ? sid : nsid, buf, len, flag, &addr, size);
	}
	else {
		struct sockaddr_in addr;
		char buf1[80];

		if (!name) {
			perror("Sysinfo error....");
			name = buf1;
		}

		int size = constr_name(addr, name, port);
		return ::sendto(nsid == -1 ? sid : nsid, buf, len, flag, (struct sockaddr*)&addr,size);

	}
}

int sock::readfrom(unsigned char* buf, int len, int flag, char* name, int *port_p, int nsid) {

	if(!port_p || *port_p == -1) {
		struct sockaddr addr;
		int size = sizeof(addr);
		if((rc = ::recvfrom(nsid == -1 ? sid : nsid, buf, len, flag, &addr,(socklen_t *)&size)) > -1 && name)
			strncpy(name, addr.sa_data,rc), name[rc] = '\0';
	}
	else {
		struct sockaddr_in addr;
		int size = sizeof(addr);
		if((rc = ::recvfrom(nsid == -1 ? sid : nsid, buf, len, flag,(struct sockaddr*)&addr,(socklen_t *)&size)) > -1) {
			if(name) strcpy(name,ip2name(addr.sin_addr));
			if(port_p) *port_p = ntohs(addr.sin_port);
		}
	}

	return rc;
}

int sock::shutdown(int mode) {

	return ::shutdown(sid,mode);
}

