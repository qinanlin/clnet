#include "client.h"

Client::Client(int iStream)
{
	m_pstSocket = new Socket(iStream);
	if(!m_pstSocket)
	{
		throw 1;
	}
}

bool Client::Connect(int iPort, char *szIpAddr)
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

int Client::Send(char *pstBuff, size_t iSize)
{
	return m_pstSocket->Send(m_pstSocket->m_iSocket, (void * )pstBuff, iSize, 500);
}

int Client::Recv(char *pstBuff, size_t iSize)
{
	return m_pstSocket->Recv(m_pstSocket->m_iSocket,(void *) pstBuff, iSize, 500);
}