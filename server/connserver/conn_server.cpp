#include "connserver.h"

int main()
{
	#if 0
	ConnServer stServer(5120);
	stServer.Init();
	stServer.MainLoop();
	stServer.Exit();
	#endif
	Server *pServer = new ConnServer;
	
	pServer->ServerOpt();
	
	pServer->Init(5120, 9411);
	
	pServer->MainLoop();
	
	pServer->Exit();

	SAFE_DELETE(pServer)
	return 0;
}
