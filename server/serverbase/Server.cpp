#include "Server.h"
#include <sys/stat.h>
#include <sys/resource.h>
#include <iostream>
#include <cstdlib>
#include <signal.h>

Server::Server(int iStream, int iPort)
{
	m_pstSerSock = new ServerSocket(iStream, iPort);
	if(!m_pstSerSock)
	{
		throw 1;
	}
}
	
Server::~Server()
{
	SAFE_DELETE(m_pstSerSock)
}

void Server::ListenSignal()
{
	//监听信号
}

void Server::ServerOpt()
{
	//处理setopt_long  
}

void Server::SetDaemon()
{
	struct rlimit r1;
	pid_t pid;
	struct sigaction sa;
	int i, fd0, fd1, fd2;

	//清除文件创建掩码
	umask(0);  

	//每个进程能打开的最多文件数
	if(getrlimit(RLIMIT_NOFILE, &r1) < 0)
	{
		std::cout<<"cannot get file limit"<<std::endl;
		exit(0);
	}
	
	//父进程退出
	if(pid = fork() < 0)
	{
		std::cout<<"fork failed"<<std::endl;
		exit(0);
	}
	else if(pid > 0)
	{
		exit(0);
	}
	
	//创建新会话(成为新会话的首进程；新进程组组长；没有控制终端)
	setsid();

	//确定避免获得控制终端
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGHUP, &sa, NULL) < 0)
	{
		std::cout<<"cannot ignore SIGHUP"<< std::endl;
		exit(0);
	}
	
	if((pid = fork()) < 0)
	{
		std::cout<<"cannot fork"<<std::endl;
	}
	else if(pid != 0)
	{
		exit(0);
	}
	
	//将当前工作目录更改为根目录
	if(chdir("/") < 0)
	{
		std::cout<<"cannot change directory to /"<<std::endl;
		exit(0);
	}

	//关闭不需要的描述符
	if(r1.rlim_max == RLIM_INFINITY)
	{
		r1.rlim_max = 1024;
	}
	for(i = 0; i < r1.rlim_max; ++i)
	{
		close(i);
	}


	//
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	if(fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		//std::cout
		exit(1);
	}
}

void Server::MainLoop()
{
	int iBusy = 0;
	
	while(1)
	{
		ListenSignal();
		
		iBusy = Loop();

		if(iBusy)   //busy
		{
			usleep(2000000);
		}
		else
		{
			usleep(1000000);
		}
		
		Tick(iBusy);
	}
	
}

/*int Server::Loop()
{
	return 0;
}
bool Server::Init(unsigned int, int)
{
	return true;
}
bool Server::Tick(int)
{
	return true;
}
bool Server::Load()
{
	return true;
}

bool Server::Exit()
{
	return true;
}*/



