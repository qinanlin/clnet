#include "logicserver.h"

int main()
{
	Server *pServer = new LogicSvr;
	
	pServer->ServerOpt();
	
	pServer->Init(0, 0);
	
	pServer->MainLoop();
	
	pServer->Exit();

	SAFE_DELETE(pServer)
	return 0;
}

