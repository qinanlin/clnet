#include "clientsocket.h"
#include <iostream>

ClientSocket::ClientSocket()
{
}

bool ClientSocket::Init(int iStream)
{
	m_pstSocket = new Socket(iStream);
	if(!m_pstSocket)
	{
		return false;
	}

	m_SendBuf = new char [MAX_MSG_LENTH]();
	if(!m_SendBuf)
	{
		return false;
	}

	m_RecvBuf = new char [MAX_MSG_LENTH]();
	if(!m_RecvBuf)
	{
		return false;
	}

	return true;
}

bool ClientSocket::Connect(int iPort, char *szIpAddr)
{
	size_t iLen;
	if(!m_pstSocket)
	{
		return false;
	}
	
	m_pstSocket->m_stSockAddrIn.sin_family = AF_INET;
    m_pstSocket->m_stSockAddrIn.sin_port = htons(iPort);
    m_pstSocket->m_stSockAddrIn.sin_addr.s_addr = inet_addr(szIpAddr);

	iLen = sizeof(m_pstSocket->m_stSockAddrIn);
	
	return m_pstSocket->Connect((struct sockaddr *)&m_pstSocket->m_stSockAddrIn, iLen);
}

int ClientSocket::Send(char *pstBuff, size_t iSize)
{
	return m_pstSocket->Send(m_pstSocket->m_iSocket, (void * )pstBuff, iSize, 500);
}

int ClientSocket::Recv(char *pstBuff, size_t iSize)
{
	return m_pstSocket->Recv(m_pstSocket->m_iSocket,(void *) pstBuff, iSize, 500);
}

int ClientSocket::SendMsg(char *pstBuff, size_t iSize)
{
	unsigned short uiLen = htons(iSize);
	
	if(!pstBuff || !iSize)
	{
		return -1;
	}
	
	if(iSize >= MAX_MSG_LENTH - PROTO_MSG_LENTH)
	{
		return -1;
	}
	
	memcpy(m_SendBuf, &uiLen, sizeof(uiLen));
	memcpy(m_SendBuf + sizeof(uiLen), pstBuff, iSize);
	
	return  m_pstSocket->Send(m_pstSocket->m_iSocket, (void * )m_SendBuf, sizeof(uiLen) + iSize, 500);
}