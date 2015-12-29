#include "ConnInfo.h"
#include <iostream>

ConnInfo::ConnInfo():m_iData(0), m_iMsgLen(0), m_iOff(0)
{	
	m_pRecvBuff = new char [MAX_RECVBUFF_SIZE]();
	if(!m_pRecvBuff)
	{
		std::cout << "new recvbuff error" << std::endl;
		throw 1;
	}

	//memset(m_pRecvBuff, 0, sizeof(char) * MAX_RECVBUFF_SIZE);
	
	m_pSendBuff = new char [MAX_SENDBUFF_SIZE]();
	if(!m_pSendBuff)
	{
		std::cout << "new sendbuf error" << std::endl;
		throw 1;
	}

	//memset(m_pSendBuff, 0, sizeof(char) * MAX_SENDBUFF_SIZE);
	
	m_pstSocket = new Socket();
	if(!m_pstSocket)
	{
		std::cout << "new socket error" << std::endl;
		throw 1;
	}
	
	SetInActive();
}

ConnInfo::~ConnInfo()
{
	SAFE_DELETE_ARRAY(m_pRecvBuff);
	SAFE_DELETE_ARRAY(m_pSendBuff);
	SAFE_DELETE(m_pstSocket);
}

void ConnInfo::CleanConnInfo()
{
	 
	m_pstSocket->Close();
	 
}

