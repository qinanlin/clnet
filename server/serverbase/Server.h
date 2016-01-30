#ifndef _SERVER_H_
#define _SERVER_H_

#include "ServerSocket.h"
/*
	���е�Server���ɰѹ��õĽӿڹ鼯�ڴ�
*/

class Server
{
public:
	Server(){};
	
	virtual ~Server();
	
	void MainLoop();
	virtual bool Init(unsigned int, int){};
	virtual bool Exit(){};
	void ServerOpt();
protected:
	virtual bool Load(){};
	virtual bool Tick(int){};
	virtual int Loop(){};
	void ListenSignal();
	void SetDaemon();
};

#endif
