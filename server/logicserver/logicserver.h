#ifndef LOGICSVR_H
#define LOGICSVR_H

#include "Server.h"
#include "logapi.h"
#include "CommMisc.h"


class LogicSvr:public Server
{
public:
	LogicSvr(){};
	~LogicSvr()
	{
		SAFE_DELETE_ARRAY(m_pRecvbuf)
		SAFE_DELETE_ARRAY(m_pSendbuf)
	}

	bool ParseBuf(char *buf, int iLen);
	bool Init(unsigned int, int);
	bool Exit();
	bool Load();
	bool Tick(int);
	int Loop();
private:
	int m_iRecvSize;
	int m_iSendSize;
	char *m_pRecvbuf;
	char *m_pSendbuf;
	LogApi m_cLogApi;
	ShmChannel m_cChannel;
	
};

#endif
