#ifndef CLIENTSOCKET_H_
#define CLIENTSOCKET_H_

#include "Socket.h"
#include "CommMisc.h"
#include "proto.h"

class ClientSocket
{
public:
	ClientSocket();
	~ClientSocket()
	{
		SAFE_DELETE_ARRAY(m_RecvBuf)
		SAFE_DELETE_ARRAY(m_SendBuf)
		SAFE_DELETE(m_pstSocket)
	};

	bool Init(int);
	bool Connect(int iPort, char *szIpAddr);
	int Send(char *pstBuff, size_t iSize);
	int Recv(char *pstBuff, size_t iSize);
	int SendMsg(char * pstBuff,size_t iSize);
public:
	Socket *m_pstSocket;
private:
	char *m_SendBuf;
	char *m_RecvBuf;
};
#endif