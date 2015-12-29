#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "SocketAPI.h"
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>
#include <cstring>

//
#define IP_SIZE 24

class Socket
{
public:
	Socket();
	Socket(int);
	virtual ~Socket()
	{
		Close();
	}
	//~Socket();
		
	int Accept(struct sockaddr *, size_t *);
	bool Connect(const struct sockaddr *, size_t );
	bool Bind(const struct sockaddr *, size_t );
	bool Listen(int);
	
	int Send(int , const void *, size_t , int);
	int Recv(int , void *, size_t , int );
	int Sendto(int , void *, size_t , int ,	struct sockaddr *, size_t );
	int Recvfrom(int , void *, size_t , int ,struct sockaddr *, size_t *);
	
	bool IsNonBlocking() const;
	bool SetNonBlocking(int);
	bool SetReuseAddr();
	bool SetLinger(int);
	bool CheckConnect();
	void Close();
	
	bool Shutdown(int);
	bool Getsockopt(int ,int ,void *, size_t *) const;
	bool Setsockopt(int ,int ,const void *,	size_t);
	bool SetSendBuffSize(int);
	int GetSendBuffSize() const;
	bool SetRecvBuffSize(int);
	int GetRecvBuffSize() const;
public:
	int m_iSocket;
	//int iPort;
	struct sockaddr_in m_stSockAddrIn;  //可记录Svr的sockaddr_in信息，也可记录Client的sockaddr_in信息，由派生类具体实现
	//char acHost[IP_SIZE];
};

#endif
