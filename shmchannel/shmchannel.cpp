#include "Shmchannel.h"
#include <cstring>
#include <iostream>

#define DEBUG_LOG(p) std::cout<< p <<std::endl

ShmChannel::ShmChannel()
{
}

ShmChannel::~ShmChannel()
{
}

void ShmChannel::SC_sendchan_check()
{
	std::cout<<"Head : " << m_stSendChan.m_ShmMem->Head ;
	std::cout<<", Tail : " << m_stSendChan.m_ShmMem->Tail <<std::endl;
}

void ShmChannel::SC_recvchan_check()
{
	std::cout<<"Head : " << m_stRecvChan.m_ShmMem->Head ;
	std::cout<<", Tail : " << m_stRecvChan.m_ShmMem->Tail <<std::endl;
}

bool ShmChannel::SC_init(int iKey1, int iKey2)
{
	bool bRet;
	bRet = m_stSendChan.Shm_init(iKey1, 0666 | IPC_CREAT);
	m_stSendChan.m_ShmMem->Tail = 0;
	m_stSendChan.m_ShmMem->Head = 0;
	if(bRet)
	{
		bRet = m_stRecvChan.Shm_init(iKey2, 0666 | IPC_CREAT);
		m_stRecvChan.m_ShmMem->Tail = 0;
		m_stRecvChan.m_ShmMem->Head = 0;
		return true;
	}
	return false;
}

bool ShmChannel::SC_load(int iSendKey, int iRecvKey)
{
	bool bRet;
	if(iSendKey)
	{
		bRet = m_stSendChan.Shm_init(iSendKey, 0666 | IPC_CREAT);
		if(!bRet)
		{
			return false;
		}
	}
	
	if(iRecvKey)
	{
		bRet = m_stRecvChan.Shm_init(iRecvKey, 0666 | IPC_CREAT);
		if(!bRet && iSendKey)		//必须两块内存都成功，否则处理掉第一块
		{
			m_stSendChan.Shm_del();
			return false;
		}
	}
	return true;
}

void ShmChannel::SC_del()
{
	m_stSendChan.Shm_del();
	m_stRecvChan.Shm_del();
}

bool ShmChannel::SC_send(char *pSendbuf, int iLen)  //有六种情况
{
	ChannelInfo *pstInfo = m_stSendChan.m_ShmMem;
	int iSize = iLen + CHANNEL_MSG_LEN;
	int iTmp;
	int iTail = 0;
	char cTmp[CHANNEL_MSG_LEN] = {0};

	if(iSize > MAX_CHANNEL_SIZE - 1)
	{
		return false;
	}

	if((pstInfo->Tail + 1) % MAX_CHANNEL_SIZE == pstInfo->Head)
	{
		//DEBUG_LOG("full");
		return false;
	}

	if(pstInfo->Tail + iSize < MAX_CHANNEL_SIZE)
	{
		if(pstInfo->Tail < pstInfo->Head && pstInfo->Tail + iSize >= pstInfo->Head)
		{
			//DEBUG_LOG("error 1");    //正常错误，send数据比剩余空间长，passed
			return false;
		}
		else
		{
			memcpy(pstInfo->Channel + pstInfo->Tail, &iSize, CHANNEL_MSG_LEN);
			memcpy(pstInfo->Channel + pstInfo->Tail + CHANNEL_MSG_LEN, pSendbuf, iLen);
			pstInfo->Tail += iSize;
		}
	}
	else	//pstInfo->Tail + iSize >= MAX_CHANNEL_SIZE
	{
		if(pstInfo->Head > pstInfo->Tail)
		{
			//DEBUG_LOG("pstInfo->Head > pstInfo->Tail");  //正常错误，send数据比剩余空间长，passed
			return false;
		}
		else
		{
			iTmp = pstInfo->Tail + iSize - MAX_CHANNEL_SIZE;
			if(iTmp >= pstInfo->Head)
			{
				//DEBUG_LOG("iTmp >= pstInfo->Head");//正常错误，send数据比剩余空间长,passed
				return false;
			}
			else
			{
				iTail = pstInfo->Tail + CHANNEL_MSG_LEN;
				if(iTail < MAX_CHANNEL_SIZE)
				{
					memcpy(pstInfo->Channel + pstInfo->Tail, &iSize, CHANNEL_MSG_LEN);
					
					memcpy(pstInfo->Channel + iTail,	pSendbuf, MAX_CHANNEL_SIZE - iTail);
					memcpy(pstInfo->Channel, pSendbuf + (MAX_CHANNEL_SIZE - iTail), iTmp);
				}
				else
				{					
					int iTmp2 = MAX_CHANNEL_SIZE - pstInfo->Tail;
					if(iTmp2 > CHANNEL_MSG_LEN)
					{
						DEBUG_LOG("iTmp2 > CHANNEL_MSG_LEN");  //unexpected error...
						return false;
					}
					
					memcpy(cTmp, &iSize, sizeof(cTmp));
					
					memcpy(pstInfo->Channel + pstInfo->Tail, cTmp, iTmp2);
					
					memcpy(pstInfo->Channel, cTmp + iTmp2, CHANNEL_MSG_LEN - iTmp2);
					memcpy(pstInfo->Channel + CHANNEL_MSG_LEN - iTmp2, pSendbuf, iLen);
				}
				
				pstInfo->Tail = iTmp;
			}
		}
	}

	//SC_sendchan_check();

	return true;
}

int ShmChannel::SC_recv(char *pRecvbuf, int iLen)	//有三种情况, iLen为pRecvbuf长度
{
	ChannelInfo *pstInfo = m_stRecvChan.m_ShmMem;
	int iCur, iTmp, iSize;

	if(pstInfo->Head == pstInfo->Tail)
	{
		//DEBUG_LOG("empty");
		return 0;
	}

	memcpy(&iSize, pstInfo->Channel + pstInfo->Head, CHANNEL_MSG_LEN);
	
	if(iSize > MAX_CHANNEL_SIZE || iSize < CHANNEL_MSG_LEN || iLen < iSize - CHANNEL_MSG_LEN)
	{
		//DEBUG_LOG("error 2");
		return -1;
	}
	
	if(pstInfo->Head > pstInfo->Tail)
	{
		if(pstInfo->Head + iSize < MAX_CHANNEL_SIZE)
		{
			memcpy(pRecvbuf, pstInfo->Channel + pstInfo->Head + CHANNEL_MSG_LEN, iSize - CHANNEL_MSG_LEN);
			pstInfo->Head += iSize;
		}
		else
		{
			iCur = pstInfo->Head + iSize - MAX_CHANNEL_SIZE;
			if(iCur > pstInfo->Tail)
			{
				DEBUG_LOG("iCur > pstInfo->Tail");    //unexpected error
				return -1;
			}
			else
			{
				iTmp = MAX_CHANNEL_SIZE - pstInfo->Head - CHANNEL_MSG_LEN;
				if(iTmp >= 0)
				{
					memcpy(pRecvbuf, pstInfo->Channel + pstInfo->Head + CHANNEL_MSG_LEN, iTmp);
					memcpy(pRecvbuf + iTmp, pstInfo->Channel, iCur);
				}
				else
				{
					memcpy(pRecvbuf, pstInfo->Channel - iTmp, iCur + iTmp);  //iTmp < 0
				}
				pstInfo->Head = iCur;
			}
		}
	}
	else
	{
		if(pstInfo->Head + iSize <= pstInfo->Tail)
		{
			memcpy(pRecvbuf, pstInfo->Channel + pstInfo->Head + CHANNEL_MSG_LEN, iSize - CHANNEL_MSG_LEN);
			pstInfo->Head += iSize;
		}
		else
		{
			DEBUG_LOG("error 3");  //unexpected error
			return -1;
		}
	}

	//SC_recvchan_check();
	
	return iSize - CHANNEL_MSG_LEN;
}

#if 0
bool ShmChannel::SC_msgsend(char *pSendbuf, int iSize)
{
	/*int iLen = iSize + CHANNEL_MSG_LEN;

	if(!pSendbuf)
	{
		return false;
	}

	memcpy(pSendbuf, &iLen, CHANNEL_MSG_LEN);	//整个msg的长度，放于msg的头部，方便解析
	memmove(pSendbuf + CHANNEL_MSG_LEN, pSendbuf, iSize);*/

	return SC_send(pSendbuf, iSize);
}

int ShmChannel::SC_msgrecv(char *pRecvbuf)
{
	int iLen = 0;
	if(!pRecvbuf)
	{
		return -1;
	}
	
	if(SC_recv(pRecvbuf, iLen))
	{
		memmove(pRecvbuf, pRecvbuf + CHANNEL_MSG_LEN, iLen - CHANNEL_MSG_LEN);
		return iLen - CHANNEL_MSG_LEN;
	}
	return -1;
}
#endif