#include "logsys.h"
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <sys/time.h>
/*LogSys::LogSys()
{
	
}*/
LogSys::~LogSys()
{
	SAFE_DELETE_ARRAY(m_pRecvbuf);
	
	if(m_pStream)
	{
		fclose(m_pStream);
	}
}

bool LogSys::InitRecvbuf(int iSize)
{
	m_pRecvbuf = new char[iSize]();
	if(NULL == m_pRecvbuf)
	{
		std::cout<<"init recvbuf error"<< std::endl;
		return false;
	}
	m_iBufSize = iSize;
	return true;
}

bool LogSys::UdpInit(char *url)
{
	return true;
}

bool LogSys::CheckFileName()
{
	char FileName[MAX_FILENAME_LEN + 8] = {0};		//文件名实例:XXXX_N.log    N为数字最大为三位数
	FILE *pFile;
	long lSize = 0;

	for(int i = 1; i <= 999; ++i)		//文件名:XXX_N.log，N为三位数，所以循环999次
	{
		//memset(FileName, 0, sizeof(FileName));
		sprintf(FileName, "%s_%d.log", m_FileName, i);

		if(0 == access(FileName, 0))		//文件存在
		{
			pFile = fopen(FileName, "r");
			if(NULL == pFile)
			{
				std::cout<<"open failed(in for)" <<std::endl;
				continue;
			}
			fseek (pFile, 0, SEEK_END);  		//将文件指针移动文件结尾
			lSize = ftell (pFile); 			//求出当前文件指针距离文件开始的字节数
			
			if(lSize >= MAX_LOGFILE_SIZE)
			{
				fclose (pFile);
				continue;
			}
            else
			{
				fclose(pFile);
            }
		}

		if(m_pStream)
		{
			fclose(m_pStream);
		}
		
		if((m_pStream = fopen(FileName, "a+")) == NULL)
		{
			std::cout<<"open failed" << std::endl;
			continue;
		}
		else
		{
			break;
		}
	}

	if(NULL == m_pStream)
	{
		return false;
	}

	return true;
}

bool LogSys::ShmChanInit(int iKey)
{
	int iLen = 0;

	m_pStream = NULL;

	memset(m_FileName, 0, sizeof(m_FileName));

	if(false == InitRecvbuf(MAX_RECVBUFFER_SIZE))
	{
		return false;
	}
	
	if(false == m_ShmChan.SC_load(0, iKey))
	{
		std::cout<< "sc_init error"<< std::endl;
		return false;
	}
	
	strcpy(m_FileName, "conn_svr");		//以后改为读配置文件

	if(0 ==  strlen(m_FileName))
	{
		std::cout<< "FileName error" << std::endl;
		return false;
	}
	
	return CheckFileName();
	
	//return true;
}

void LogSys::WirteFile(char *pBuf, int iLen)
{
	char str[128] = {0};
	time_t tt = time(0);
	tm *t = localtime(&tt);
	sprintf(str,"[%4d-%02d-%02d %02d:%02d:%02d] ", 
		t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec);
	fwrite(str, strlen(str), 1, m_pStream);

	fwrite(pBuf, iLen, 1, m_pStream);
	fwrite("\n", 1, 1, m_pStream);			//换行
	fflush(m_pStream);
}

void LogSys::RecvLogInfo()
{
	int iLen;
	iLen = m_ShmChan.SC_recv(m_pRecvbuf, m_iBufSize);

	if(iLen > 0)
	{
		WirteFile(m_pRecvbuf, iLen);
	}
}

void LogSys::ShmChanMainLoop()
{
	int iBusy = 0;
	
	while(1)
	{
		RecvLogInfo();
		
		usleep(150000);

		ShmChanTick();
	}
}

void LogSys::ShmChanTick()
{
	static struct timeval stTv;
	struct timeval stNow;

	gettimeofday(&stNow, NULL);

	if(SHMCHAN_TICK_SEC <= stNow.tv_sec - stTv.tv_sec)
	{
		CheckFileName();
		stTv.tv_sec = stNow.tv_sec;
	}
}