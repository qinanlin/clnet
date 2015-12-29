#ifndef _CONNPOOL_H_
#define _CONNPOOL_H_

#include "ConnInfo.h"


#define MAX_CONNPOOL_NUM 8000

class ConnPool
{
public:
	ConnPool(unsigned int);
	~ConnPool()
	{
		SAFE_DELETE_ARRAY(m_pastConnPool)
	}
	bool SetConnActive(int);
	bool NewConnInfo(Socket &);
	//Socket *NewConnInfo();
	bool DelConnInfo(int);
	ConnInfo * GetConnInfo(int);
	void AddConnNum()
	{
		++m_iUseNum;
	};

public:
	//int *pPoolIdx;
	ConnInfo *m_pastConnPool;
protected:
	unsigned int m_iPoolSize;
	//unsigned int iIdxSize;
	int m_iUseNum;
	unsigned int m_iValidIdx;
};

#endif