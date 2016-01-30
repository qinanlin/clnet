#include "logicserver.h"
#include <iostream>
#include "msg.pb.h"

#define BUFF_SIZE 10240		//以后改为读取配置

bool LogicSvr::Init(unsigned int uiunUse, int iunUse)
{
	if(false == m_cChannel.SC_load(123451, 123450))
	{
		std::cout<< "sc_load error"<<std::endl;
		return false;
	}

	m_pRecvbuf = new char[BUFF_SIZE]();
	if(NULL == m_pRecvbuf)
	{
		std::cout<<"init recvbuf error"<< std::endl;
		return false;
	}
	
	m_iRecvSize = BUFF_SIZE;

	m_pSendbuf = new char[BUFF_SIZE]();
	if(NULL == m_pSendbuf)
	{
		std::cout<<"init sendbuf error"<< std::endl;
		return false;
	}
	
	m_iSendSize = BUFF_SIZE;

	
	return true;
	
}
bool LogicSvr::Exit()
{
	return true;
}

bool LogicSvr::Load()
{
	return true;
}

bool LogicSvr::Tick(int num)
{
	return true;
}

bool LogicSvr::ParseBuf(char *buf, int iLen)
{
	//TODO
	CSMSGPKG::CSMsg clte;
	clte.ParseFromArray(buf, iLen);
	std::cout<< "ver = " << clte.ver() << std::endl;
	std::cout<< "cmd = " << clte.cmd()<< std::endl;
	std::cout<< clte.body().te1().str().c_str()<<std::endl;
	std::cout<< clte.body().te1().num() << std::endl;
	std::cout<< iLen << std::endl;

	std::cout<< clte.body().te2().b() << std::endl;
	std::cout<< clte.body().te2().d() << std::endl;
	
	std::cout<< clte.body().te2().f_size() << std::endl;
	std::cout<<  clte.body().te2().f(0).num() << std::endl;
	std::cout<<  clte.body().te2().f(1).num() << std::endl;
	std::cout<<  clte.body().te2().f(2).num() << std::endl;
	return true;
}

int LogicSvr::Loop()
{
	int iLen;

	iLen = m_cChannel.SC_recv(m_pRecvbuf, m_iRecvSize);
	if(iLen < 0)
	{
		std::cout<< "recv error" << std::endl;
	}
	else if(iLen == 0)
	{
		//empty
	}
	else
	{
		ParseBuf(m_pRecvbuf , iLen);
	}

	return 0;
}

