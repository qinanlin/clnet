#ifndef _CONNEPOLL_H_
#define _CONNEPOLL_H_

#include <sys/epoll.h>

#include "CommMisc.h"



typedef struct epoll_event Epoll_event;

class ConnEpoll
{
public:
	ConnEpoll();
	ConnEpoll(int, int);
	~ConnEpoll()
	{
		SAFE_DELETE_ARRAY(m_pstEvent)
	}

	void EpollCreate(int );
	int EpollWait();
	int EpollAdd(int);
	int EpollDel(int);
	
	bool EpollInit(unsigned int, int);

	int EpollGetEventFd(int) const;

	
	Epoll_event *EpollGetEvent(int iId) const;	
private:
	int m_Epfd;
	unsigned int m_MaxNum;
	Epoll_event m_ev;
	Epoll_event *m_pstEvent;
};

#endif
