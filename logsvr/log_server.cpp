#include "logsys.h"

int main()
{
	LogSys LogSvr;
	bool bRet;
	

	bRet = LogSvr.ShmChanInit(9001);
	if(!bRet)
	{
		return -1;
	}
	
	LogSvr.ShmChanMainLoop();
	return 0;
}
