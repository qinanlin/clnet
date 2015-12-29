#include "ConnEpoll.h"

ConnEpoll::ConnEpoll(int iSize, int iFd)
{
	EpollInit(iSize, iFd);
}

void ConnEpoll::EpollCreate(int iFd)
{
	m_Epfd = epoll_create(m_MaxNum);
	EpollAdd(iFd);	
}

bool ConnEpoll::EpollInit(unsigned int iSize, int iFd)
{
	m_MaxNum = iSize;
	m_pstEvent = new struct epoll_event [iSize];
	if(!m_pstEvent)
	{
		return false;
	}

	EpollCreate(iFd);
	return true;
}

int ConnEpoll::EpollAdd(int iFd)
{
	int iRet;
	
	m_ev.data.fd = iFd;
	m_ev.events = EPOLLIN;
	
	iRet = epoll_ctl(m_Epfd, EPOLL_CTL_ADD, iFd, &m_ev);
    if (iRet)
    {
        
    }

    return iRet;
}

int ConnEpoll::EpollDel(int iFd)
{
	int iRet;
	iRet = epoll_ctl(m_Epfd, EPOLL_CTL_DEL, iFd, &m_ev);
	if(iRet)
	{
	}
	return iRet;
}

int ConnEpoll::EpollWait()
{
	return epoll_wait(m_Epfd, m_pstEvent, m_MaxNum, 0);
}

int ConnEpoll::EpollGetEventFd(int iId) const
{
	return m_pstEvent[iId].data.fd;
}

Epoll_event *ConnEpoll::EpollGetEvent(int iId) const
{
	return &m_pstEvent[iId];
}

