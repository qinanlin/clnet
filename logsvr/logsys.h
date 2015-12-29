#ifndef _LOGSYS_H_
#define _LOGSYS_H_

#include <cstdio>
#include "shmchannel.h"
#include <iostream>

#define MAX_FILENAME_LEN 64

#define MAX_LOGFILE_SIZE (2048 * 1000 * 100)		//200M

#define MAX_RECVBUFFER_SIZE 256

#define SHMCHAN_TICK_SEC (60 * 20)	//20∑÷÷”

class LogSys
{
public:
	LogSys(){};
	~LogSys();

	bool InitRecvbuf(int);
	
	bool UdpInit(char *url);

	bool ShmChanInit(int);

	void ShmChanMainLoop();

private:
	void WirteFile(char *pBuf, int iLen);
	bool CheckFileName();
	
	void RecvLogInfo();

	void ShmChanTick();
	
private:
	
	int m_iBufSize;
	ShmChannel m_ShmChan;
	char m_FileName[MAX_FILENAME_LEN];
	char *m_pRecvbuf;
	FILE *m_pStream;
};
#endif
