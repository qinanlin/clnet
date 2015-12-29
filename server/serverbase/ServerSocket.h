#ifndef _SERVERSOCKET_H_
#define _SERVERSOCKET_H_

#include "Socket.h"
#include "CommMisc.h"


class ServerSocket
{
public:
	ServerSocket(){};
	ServerSocket(int, int);
	~ServerSocket();
	bool CreateTcp(int = 5);
	//bool CreateUdp();
	
	bool Accept(Socket &);
	void Close();
	
	int GetSocketFd()
	{ 
		if(!m_pstSocket)
		{
			return -1;
		}
		return m_pstSocket->m_iSocket; 
	}
	
	bool IsNonBlocking() const
	{ 
		if(!m_pstSocket)
		{
			return false;
		}
		return m_pstSocket->IsNonBlocking();
	}
	
    bool SetNonBlocking(int iFlag = 1)
    {
    	if(!m_pstSocket)
		{
			return false;
		}
		return m_pstSocket->SetNonBlocking(iFlag);
	}
 
    // get/set receive buffer size
    int GetRecvBuffSize() const
    { 
    	if(!m_pstSocket)
		{
			return -1;
		}
		return m_pstSocket->GetRecvBuffSize(); 
	}
    
    bool SetRecvBuffSize(int iSize)
    {
    	if(!m_pstSocket)
		{
			return false;
		}
		return m_pstSocket->SetRecvBuffSize(iSize);
	}
 
    // get/set send buffer size
    int GetSendBufferSize() const 
    { 
    	if(!m_pstSocket)
		{
			return -1;
		}
		return m_pstSocket->GetSendBuffSize(); 
	}
    
    bool SetSendBufferSize(int iSize)
    { 
    	if(!m_pstSocket)
		{
			return false;
		}
		return m_pstSocket->SetSendBuffSize(iSize); 
	}

	bool SetReUseAddr()
	{
		if(!m_pstSocket)
		{
			return false;
		}
		return m_pstSocket->SetReuseAddr();
	}

	int SendMsg(int ,char *, int , int);//tcp
	int RecvMsg(int ,char *, int , int);//tcp

	int SendtoMsg(){};	//udp
	int RecvfromMsg(){};//udp
protected:
	Socket *m_pstSocket;
	int m_iPort;
};

#endif
