#include "connserver.h"

int main()
{
	#if 0
	ConnServer stServer(5120);
	stServer.Init();
	stServer.MainLoop();
	stServer.Exit();
	#endif
	Server *pstServer = new ConnServer;
	
	pstServer->ServerOpt();
	
	pstServer->Init(5120, 9411);
	
	pstServer->MainLoop();
	
	pstServer->Exit();

	SAFE_DELETE(pstServer)
	return 0;
}
