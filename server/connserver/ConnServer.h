#ifndef _CONNSERVER_H_
#define _CONNSERVER_H_

#include "Server.h"
#include "ConnPool.h"
#include "ConnEpoll.h"
#include <signal.h>
#include "Proto.h"
#include "logapi.h"


class ConnServer : public Server
{
public:
	ConnServer(){};
	//ConnServer(unsigned int);
	~ConnServer();
	
	int Loop();
	bool Init(unsigned int, int);
	bool Tick(int);
	bool Load();
	bool Exit();
	int ConnRecv();
	int RecvProc(int);
	int ConnSend();
	bool AddConn();
	bool DelConn(ConnInfo &);
	bool ParseBuff(ConnInfo &);
	int GetMsgLen(ConnInfo *);
	
public:
	ConnPool *m_pstConnPool;
	unsigned int m_uiSize;
	ConnEpoll *m_pstEpoll;
private:
	LogApi m_cLogApi;
	//ServerSocket *pstServerSocket;
};

#endif
