#include "ConnPool.h"
#include <iostream>

ConnPool::ConnPool(unsigned int iSize)
{
	int i;
	
	if(iSize < 0 || iSize > MAX_CONNPOOL_NUM)
	{
		throw 1;
	}
	
	m_iPoolSize = iSize;

	m_pastConnPool = new ConnInfo[m_iPoolSize];
	if(!m_pastConnPool)
	{
		std::cout << "new conninfo error" << std::endl;
		throw 1;
	}

	//todo check conninfo->m_pstSocket if null??
	for(i = 0; i < iSize; ++i)
	{
		if(NULL == m_pastConnPool[i].m_pstSocket)
		{
			std::cout<<i << "NULL" << std::endl;
		}
	}
}

bool ConnPool::SetConnActive(int iIndex)
{
	if(iIndex < 0)
	{
		return false;
	}
	
	m_pastConnPool[iIndex].SetActive();
	++m_iUseNum;
	return true;
}

bool ConnPool::NewConnInfo(Socket &stSock)
{
	Socket *pNewSock = NULL;
	
	if(m_iUseNum >= m_iPoolSize || !m_pastConnPool)
	{
		return false;
	}
	
	if(m_iValidIdx < 0)
	{
		for(int i = 0; i < m_iPoolSize; ++i)
		{
			if(!m_pastConnPool[i].IsActive())
			{
				m_iValidIdx = i;
				pNewSock = m_pastConnPool[m_iValidIdx].GetSocket();
				break;
			}
		}
	}
	else
	{
		int i = 0;
		do
		{
			m_iValidIdx = (m_iValidIdx + 1) % m_iPoolSize;
			if(!m_pastConnPool[m_iValidIdx].IsActive())
			{
				pNewSock = m_pastConnPool[m_iValidIdx].GetSocket();
				break;
			}
			++i;
		}while(i < m_iPoolSize);
	}

	if(pNewSock && m_iValidIdx >= 0)
	{
		AddConnNum();
		SetConnActive(m_iValidIdx);
		memcpy(pNewSock, &stSock, sizeof(*pNewSock));
		return true;
	}
	return false;
}

ConnInfo *ConnPool::GetConnInfo(int iFd)
{	
	if(iFd < 0 || !m_pastConnPool || iFd >= m_iPoolSize)
	{
		//log
		std::cout<< "full" << std::endl;
		return NULL;
	}
	
	return &m_pastConnPool[iFd];
}

bool ConnPool::DelConnInfo(int iFd)
{
	ConnInfo *pstConnInfo = GetConnInfo(iFd);
	
	pstConnInfo->SetInActive();
	pstConnInfo->CleanConnInfo();
}

/*Socket *ConnPool::NewConnInfo()
{
	if(iValidIdx < 0)
	{
		for(int i = 0; i < iPoolSize; ++i)
		{
			if(!pastConnPool[i].IsActive())
			{
				iValidIdx = i;
				return pastConnPool[iValidIdx].GetSocket();
			}
		}
	}
	else
	{
		int i = 0;
		do
		{
			iValidIdx = (iValidIdx + 1) % iPoolSize;
			if(!pastConnPool[iValidIdx].IsActive())
			{
				return pastConnPool[iValidIdx].GetSocket();
			}
			++i;
		}while(i < iPoolSize);
	}

	if(iValidIdx >= 0)
	{
		AddConnNum();
		SetConnActive(iValidIdx);
		pPoolIdx[pNewSock->iSocket] = iValidIdx;
		return pastConnPool[iValidIdx].GetSocket();
	}
}*/

