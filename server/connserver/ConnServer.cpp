#include "ConnServer.h"
#include "ConnInfo.h"
#include <cstdio>
#include <iostream>


/*ConnServer::ConnServer(unsigned int iSize)
{
	if(false == Init(iSize))
	{
		throw 1;
	}
}*/

ConnServer::~ConnServer()
{
	SAFE_DELETE_ARRAY(m_pstConnPool)
	SAFE_DELETE_ARRAY(m_pstEpoll)
}

bool ConnServer::Init(unsigned int iSize, int iPort)
{
	m_cLogApi.Init(9001, (char*)"conn_svr");
	
	this->m_uiSize = iSize;
	
	/*iStream = SOCK_STREAM;
	iPort = 9411;*/

	//attach share memory?
	iSize = this->m_uiSize;
	m_pstConnPool = new ConnPool(iSize);
	if(!m_pstConnPool)
	{
		std::cout << "new connpool error" << std::endl;
		return false;
	}
	
	m_pstSerSock = new ServerSocket(SOCK_STREAM, iPort);
	if(!m_pstSerSock)
	{
		std::cout << "new serversocket error" << std::endl;
		return false;
	}

	m_pstSerSock->CreateTcp();
	std::cout<<"server sockfd = " << m_pstSerSock->GetSocketFd()<<std::endl;

	
	m_pstEpoll = new ConnEpoll (iSize, m_pstSerSock->GetSocketFd());
	if(!m_pstEpoll)
	{
		std::cout << "new connepoll error" << std::endl;
		return false;
	}

	m_cLogApi.WriteLog((char*)"conn_svr start...");

	signal(SIGPIPE,SIG_IGN);  //忽略SIGPIPE信号，否则进程会退出
	return true;
}

bool ConnServer::AddConn()
{
	int iRet;
	Socket stSocket, *pstSocket = NULL;
	ConnInfo *pstConnInfo = NULL;
	bool bRet;
	
	if(m_pstSerSock->Accept(stSocket))
	{
		pstConnInfo = m_pstConnPool->GetConnInfo(stSocket.m_iSocket);
		if(pstConnInfo)
		{
			pstSocket = pstConnInfo->GetSocket();
			if(pstSocket)
			{	
				memcpy(pstSocket, &stSocket, sizeof(stSocket));
				stSocket.m_iSocket = -1; //否则stSocket释放时会把fd close掉，造成连接断开
				m_pstEpoll->EpollAdd(pstSocket->m_iSocket);
				pstSocket->SetNonBlocking(1);
				pstConnInfo->SetActive();

				std::cout<<"add conn : "<<pstSocket->m_iSocket <<std::endl;
				return true;
			}
		}
		else
		{
			//std::cout<< "full" << std::endl;
			close(stSocket.m_iSocket);  //暂时先这样处理
			return false;
		}
	}
	
	return false;
}
bool ConnServer::DelConn(ConnInfo &stConnInfo)
{
	Socket *pstSocket = stConnInfo.GetSocket();
	if(!pstSocket)
	{
		std::cout<<"DelConn failed" << std::endl;
		return false;
	}

	if(!stConnInfo.IsActive())	//已释放或未激活
	{
		return true;
	}
	
	std::cout<<"del conn fd = "<<pstSocket->m_iSocket<<std::endl;	
	pstSocket->Close();
	m_pstEpoll->EpollDel(pstSocket->m_iSocket);
	stConnInfo.SetInActive();

	stConnInfo.m_iData = 0;
	stConnInfo.m_iOff = 0;
	stConnInfo.m_iMsgLen = 0;
	return true;
}

//完整数据包长度= PROTO_MSG_LENTH + 协议长度
int ConnServer::GetMsgLen(ConnInfo *pstInfo)
{
	if(!pstInfo->m_pRecvBuff)
	{
		return -1;
	}

	//判断接收到的长度小于PROTO_MSG_LENTH
	if(pstInfo->m_iData < PROTO_MSG_LENTH)			//这样判断是否合理?
	{
		//pstInfo->m_iMsgLen = 0;
		return -1;
	}

	switch(PROTO_MSG_LENTH)
	{
	//此长度为协议体长度,不包括MSG_LENTH本身
	case 2:
		pstInfo->m_iMsgLen = ntohs(*(unsigned short*)(pstInfo->m_pRecvBuff + pstInfo->m_iOff));
		break;
	case 4:
		pstInfo->m_iMsgLen = ntohl(*(unsigned int*)(pstInfo->m_pRecvBuff + pstInfo->m_iOff));
		break;
	default:
		//pstInfo->m_iMsgLen = 0;
		std::cout<<"GetMsgLen error"<<std::endl;
		return -1;
		break;
	}
	
	if(pstInfo->m_iMsgLen <= 0)
	{
		DelConn(*pstInfo);
		return -1;
	}

	pstInfo->m_iMsgLen += PROTO_MSG_LENTH;		//加上协议前面的长度，才为数据包长度

	//协议体最大长度, 不能大于缓冲区的最大长度
	if(pstInfo->m_iMsgLen >= MAX_MSG_LENTH || pstInfo->m_iMsgLen >= MAX_RECVBUFF_SIZE)		
	{
		DelConn(*pstInfo);
		return -1;
	}
	
	return pstInfo->m_iMsgLen;
}

bool ConnServer::ParseBuff(ConnInfo &stConnInfo)
{
	
	//memset(stConnInfo.m_pRecvBuff, 0, sizeof(uiLen) + PROTO_MSG_LENTH);
	char *pszBuff = stConnInfo.m_pRecvBuff + stConnInfo.m_iOff + PROTO_MSG_LENTH;	//协议
	unsigned int uiRealPkgLen = stConnInfo.m_iMsgLen - PROTO_MSG_LENTH;		//协议真实长度

	//test
	std::cout<<"Client " << stConnInfo.GetSocket()->m_iSocket << ":" ;
	std::cout<< "pszBuff :" << pszBuff <<std::endl;
	//test end


	
	//protobuf parser and send to logic svr
	return true;
}


int ConnServer::RecvProc(int iFd)
{
	//std::cout<<"recv proc start" <<std::endl;
	int iLen;
	ConnInfo *pstConnInfo = NULL;
	Socket *pstSocket = NULL;
	unsigned int uiLen  = 0;
	bool bRet;
	
	pstConnInfo = m_pstConnPool->GetConnInfo(iFd);
	if(!pstConnInfo)
	{
		DelConn(*pstConnInfo);
		return -1;
	}

	if(!pstConnInfo->IsActive())
	{
		DelConn(*pstConnInfo);
		return -1;
	}
	
	pstSocket =	pstConnInfo->GetSocket();
	if(!pstSocket)
	{
		DelConn(*pstConnInfo);
		return -1;
	}

	iLen = m_pstSerSock->RecvMsg(pstSocket->m_iSocket, 
		pstConnInfo->m_pRecvBuff + pstConnInfo->m_iData, MAX_RECVBUFF_SIZE - pstConnInfo->m_iData, 200);
	if(iLen < 0)
	{
		DelConn(*pstConnInfo);
		return -1;
	}
	else if(0 == iLen)
	{
		//timeout
		return 0;
	}
	else
	{
		pstConnInfo->m_iData += iLen;

		if(pstConnInfo->m_iMsgLen && pstConnInfo->m_iData < pstConnInfo->m_iMsgLen)
		{
			return 0;	
		}

		if(0 == pstConnInfo->m_iMsgLen)
		{
			if(0 > GetMsgLen(pstConnInfo))
			{
				return -1;
			}
		}
		
		while(pstConnInfo->m_iData >= pstConnInfo->m_iMsgLen)
		{
			bRet = ParseBuff(*pstConnInfo);

			if(bRet)
			{
				pstConnInfo->m_iData -= pstConnInfo->m_iMsgLen;
				pstConnInfo->m_iOff += pstConnInfo->m_iMsgLen;
				pstConnInfo->m_iMsgLen = 0;

				//获取下个包的长度
				if(0 > GetMsgLen(pstConnInfo))
				{
					break;
				}
			}
			else
			{
				pstConnInfo->m_iData = 0;
				pstConnInfo->m_iOff = 0;
				pstConnInfo->m_iMsgLen = 0;

				break;
			}
		}
	}

	if(pstConnInfo->m_iOff)
	{
		if(pstConnInfo->m_iData)
		{
			if(MAX_RECVBUFF_SIZE < pstConnInfo->m_iData)
			{
				DelConn(*pstConnInfo);
				pstConnInfo->m_iData = 0;
			}
			else
			{
				memmove(pstConnInfo->m_pRecvBuff, pstConnInfo->m_pRecvBuff + pstConnInfo->m_iOff, pstConnInfo->m_iData);
			}
		}
		pstConnInfo->m_iOff = 0;
	}
	//std::cout<<"recv proc end" <<std::endl;
	return 0;
}

int ConnServer::ConnRecv()
{
	int iFdNum;
	int i;
	int iFd = -1;
	Epoll_event *pstEvent = NULL;
	int iRet;
	
	iFdNum = m_pstEpoll->EpollWait();
	if(0 > iFdNum)
	{
		//error log
		return -1;
	}
	else if(0 == iFdNum)
	{
		//timeout
		return 0;
	}

	for(i = 0; i < iFdNum; ++i)
	{
		//std::cout<< "[ FdNum = " <<iFdNum<<" ]"<<std::endl;
		pstEvent = m_pstEpoll->EpollGetEvent(i);
		if(!pstEvent)
		{
			//log
			continue;
		}
		
		iFd = pstEvent->data.fd;
		
		
		if(m_pstSerSock->GetSocketFd() == iFd)
		{
			AddConn();
			continue;
		}
		else if(pstEvent->events & EPOLLIN)
		{
			//recv and send to bus channel
			RecvProc(iFd);

			continue;
		}
		std::cout<<"fd = " << iFd <<std::endl;
	}

	return iFdNum;
}

int ConnServer::ConnSend()
{
	return 0;
}

int ConnServer::Loop()
{
	//int iRecvRet;
	//int iSendRet;
	int iRet = 0;

	for(int i = 0;i < 5; ++i)
	{
		iRet += ConnRecv();
		iRet += ConnSend();
		//usleep(100000);
	}

	if(iRet > 5000)
		return 1;
	else
		return 0;
	//return iRet;
}

bool ConnServer::Tick(int iNum)
{
	return true;
}

bool ConnServer::Load()
{
	return true;
}

bool ConnServer::Exit()
{
	return true;
}
