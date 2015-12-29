#include "logapi.h"
#include "logsys.h"
#include <stdarg.h>
#include <iostream>
#include <cstring>

bool LogApi::Init(int iKey, char *pLogFile)
{
	if(false == m_ShmChan.SC_load(iKey, 0))
	{
		std::cout<< "sc_init error"<< std::endl;
		return false;
	}

	return true;
}

bool LogApi::WriteLog(char *pSendbuf)
{
	int iLen = strlen(pSendbuf);

	if(MAX_RECVBUFFER_SIZE < iLen)
	{
		std::cout<<"The length of string is too long"<< std::endl;
		return false;
	}
	
	return m_ShmChan.SC_send(pSendbuf, iLen);
}

bool LogApi::WriteLog_v(char *pFormat, ...)
{
	char Sendbuf[MAX_RECVBUFFER_SIZE] = {0};
	va_list arg_ptr; 
	va_start(arg_ptr,pFormat); 

	vsnprintf(Sendbuf, MAX_RECVBUFFER_SIZE, pFormat, arg_ptr);
	va_end(arg_ptr);

	return m_ShmChan.SC_send(Sendbuf, strlen(Sendbuf));
}

