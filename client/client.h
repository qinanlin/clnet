#ifndef CLIENT_H_
#define CLIENT_H_

#include "Socket.h"
#include "CommMisc.h"

class Client
{
public:
	Client(int);
	~Client()
	{
		SAFE_DELETE(m_pstSocket)
	};
	bool Connect(int iPort, char *szIpAddr);
	int Send(char *pstBuff, size_t iSize);
	int Recv(char *pstBuff, size_t iSize);
public:
	Socket *m_pstSocket;
};
#endif