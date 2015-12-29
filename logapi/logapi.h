#ifndef LOGAPI_H
#define LOGAPI_H

#include "shmchannel.h"

class LogApi
{
public:
	LogApi(){};
	~LogApi(){};
	bool Init(int, char*);
	bool WriteLog(char *);
	bool WriteLog_v(char *, ...);

private:
	//char *m_pSendbuf;
	ShmChannel m_ShmChan;
};

#endif
