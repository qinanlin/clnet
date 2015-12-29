#include "ServerSocket.h"
#include <iostream>

ServerSocket::ServerSocket(int iStream, int iPort)
{
	this->m_iPort = iPort;
	m_pstSocket = new Socket(iStream);
	if(!m_pstSocket)
	{
		throw 1;
	}
}

bool ServerSocket::CreateTcp(int iBacklog)
{
	bool bRet;
	
	if(m_pstSocket)
	{
		m_pstSocket->m_stSockAddrIn.sin_family = AF_INET;
		m_pstSocket->m_stSockAddrIn.sin_port = htons(m_iPort);
		m_pstSocket->m_stSockAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
		
		bRet = m_pstSocket->SetReuseAddr();
		if(false == bRet)
		{
			std::cout<<"set reuse failed"<<std::endl;
			return false;
		}

		m_pstSocket->SetNonBlocking(1);
		
		bRet = m_pstSocket->Bind((struct sockaddr*)&m_pstSocket->m_stSockAddrIn,
					sizeof(m_pstSocket->m_stSockAddrIn));
		if(false == bRet)
		{
			return false;
		}
		
		bRet = m_pstSocket->Listen(iBacklog);
		if(false == bRet)
		{
			return false;
		}
		
		return true;
	}
	
	return false;
}

ServerSocket::~ServerSocket()
{
	if(m_pstSocket)
	{
		m_pstSocket->Close();
		SAFE_DELETE(m_pstSocket);
	}
}

void ServerSocket::Close()
{
	if(m_pstSocket)
	{
		m_pstSocket->Close();
	}
}

bool ServerSocket::Accept(Socket &stSock)
{
	size_t iLen;
	

	iLen = sizeof(stSock.m_stSockAddrIn);

	stSock.m_iSocket = 
		m_pstSocket->Accept((struct sockaddr *)&stSock.m_stSockAddrIn, &iLen);
	if(-1 == stSock.m_iSocket)
	{
		return false;
	}
	else if(0 == stSock.m_iSocket)
	{
		//timeout 
		return false;
	}

	return true;
}

int ServerSocket::SendMsg(int iFd, char *pstBuff, int iSize, int iMs)
{
	//根据send返回值pstBuff做相应的移动
	return m_pstSocket->Send(iFd, (void *)pstBuff, iSize, iMs);
}

int ServerSocket::RecvMsg(int iFd, char *pstBuff, int iSize, int iMs)
{
	return m_pstSocket->Recv(iFd, (void *)pstBuff, iSize, iMs);
}