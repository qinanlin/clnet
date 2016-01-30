#include "clientsocket.h"
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "msg.pb.h"

#define MAX_PTHREAD_NUM 1

void write_file(const char *buf, int size, FILE *p)
{
	char str[128] = {0};
	time_t tt = time(0);
	tm *t = localtime(&tt);
	sprintf(str,"[%4d-%02d-%02d %02d:%02d:%02d] %s\n", 
		t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,
		buf);
	fwrite(str, strlen(str), 1, p);
	fflush(p);
}

void *thread_run(void *arg)
{
	
	ClientSocket cl;
	pid_t pid = getpid();
	char SendBuf[32] = {0};
	char RecvBuf[64] = {0};
	//char IpAddr[32] = "114.215.198.123";
	char IpAddr[32] = "127.0.0.1";
	int iRet;
	FILE *stream;
	char file[32] = {0};
	int res;

	if(false == cl.Init(SOCK_STREAM))
	{
		std::cout<< "Init failed" << std::endl;
		pthread_exit(0);
	}
	
	sprintf(SendBuf, "client : %d", pid);

	
	
	if(false == cl.Connect(9411, IpAddr))
	{
		std::cout<< "Connect failed" << std::endl;
		//pthread_exit(0);
	}
	
	char buf[1024] = {0};
	char data[128] = {0};

	CSMSGPKG::CSMsg clMsg;
	clMsg.set_ver(10);
	clMsg.set_cmd(CSMSGPKG::CMD_TEST1);

	CSMSGPKG::CSBody *pBody = clMsg.mutable_body();
	CSMSGPKG::Test1 *pTe1 = pBody->mutable_te1();
	pTe1->set_str("hello protocol buffer");
	pTe1->set_num(10086);

	CSMSGPKG::Test2 *pTe2 = pBody->mutable_te2();
	pTe2->set_b(false);
	pTe2->set_d(3.1415926);
	pTe2->add_f()->set_num(1.0);
	pTe2->add_f()->set_num(2.0);
	pTe2->add_f()->set_num(3.0);
	int length = clMsg.ByteSize();
	clMsg.SerializeToArray(SendBuf, length);
	

	CSMSGPKG::CSMsg clte;
	clte.ParseFromArray(SendBuf, length);
	std::cout<< "ver = " << clte.ver() << std::endl;
	std::cout<< "cmd = " << clte.cmd()<< std::endl;
	std::cout<< clte.body().te1().str().c_str()<<std::endl;
	std::cout<< clte.body().te1().num() << std::endl;
	
		//char *str = "hello world";
		iRet = cl.SendMsg(SendBuf, length);
		if(0 >= iRet)
		{
			std::cout<< "send failed" << std::endl;
		}
		std::cout<< iRet << std::endl;
		if(0)
		{
			short iLen = 0;
			short iHLen = 0;
			int iOff = 0;
			

			char realdata[128] = {0};
			strcpy(data, "hey gilrs");
			iHLen = strlen(data) + 1;
			iLen = htons(iHLen);
			memcpy(buf, &iLen, sizeof(iLen));

			strncpy(realdata, data, iHLen - 3);
			memcpy(buf + sizeof(iLen), realdata, iHLen - 3);
			iRet = cl.Send(buf, sizeof(iLen) + iHLen - 3);
			if(0 >= iRet)
			{
				std::cout<< "send failed" << std::endl;
			}
			std::cout<< "Ret =" << iRet << std::endl;
			sleep(3);

			memcpy(buf, data + iHLen - 3, 3);
			iRet = cl.Send(buf, 3);
			if(0 >= iRet)
			{
				std::cout<< "send failed" << std::endl;
			}
			std::cout<< "Ret =" << iRet << std::endl;
			
			strcpy(data, "hello world");
			iHLen = strlen(data) + 1;
			iLen = htons(iHLen);
			memcpy(buf, &iLen, sizeof(iLen));
			memcpy(buf + sizeof(iLen), data, iHLen);

			iOff += sizeof(iLen) + iHLen;
			memset(data, 0, sizeof(data));
			strcpy(data, "abcdefghijklmnopqrstuvwxyz");
			iHLen = strlen(data) + 1;
			iLen = htons(iHLen);
			memcpy(buf + iOff, &iLen, sizeof(iLen));
			memcpy(buf + iOff + sizeof(iLen), data, iHLen);

			iOff += sizeof(iLen) + iHLen;
			memset(data, 0, sizeof(data));
			strcpy(data, "1234567890");
			iHLen = strlen(data) + 1;
			iLen = htons(iHLen);
			memcpy(buf + iOff, &iLen, sizeof(iLen));
			memcpy(buf + iOff + sizeof(iLen), data, iHLen);

			std::cout<< "in thread" << std::endl;
			iRet = cl.Send(buf, iOff + sizeof(iLen) + iHLen);
			if(0 >= iRet)
			{
				std::cout<< "send failed" << std::endl;
			}
			std::cout<< "Ret =" << iRet << std::endl;

			
			
		}
	
	//sleep(8);
	while(1)
		{
			usleep(10000000);
		}
	#if 0
	while(1)
	{

		if(0>= cl.Send(SendBuf, sizeof(SendBuf)))
		{
			//fwrite( "send failed" , sizeof("send failed"), 1, stream);
			write_file("send failed" , sizeof("send failed"), stream);
			break;
		}

		//std::cout<<"send"<<std::endl;
		usleep(500000);
		test = rd;
		res = select(fd + 1, &test, NULL, NULL, NULL);
		if(res < 0)
			write_file("select failed", sizeof("select failed"), stream);
		else if(res == 0)
			write_file("select timeout", sizeof("select timeout"), stream);
		else if(FD_ISSET(fd, &test))
		{
			memset(RecvBuf, 0, sizeof(RecvBuf));
			iRet = cl.Recv(RecvBuf, sizeof(RecvBuf));
			if(iRet < 0)
			{
				write_file("recv failed", sizeof("recv failed"), stream);
				break;
			}
			else if (0 == iRet)
			{
				write_file("recv time out", sizeof("recv time out"), stream);
				//std::cout<<"time out" << std::endl;
				continue;
			}
			
			//std::cout<<RecvBuf<<std::endl;
			/*{
				char *p = RecvBuf;
				for(int i = 0;i < iRet; ++i)
				{
					if(*p == '\0')
						++p;
					else
					{	
						std::cout<<*p;
						++p;
					}
				}
				std::cout<<"---end"<<std::endl;
			}*/
			write_file(RecvBuf, iRet, stream);
			//std::cout<< iRet << std::endl;
		}
		else
		{
			write_file("unknown", sizeof("unknown"), stream);
		}
	}
	fclose(stream);
	#endif
	//pthread_exit(0);
	return (void*)NULL;
}

int main()
{
	pthread_t ptid[MAX_PTHREAD_NUM] = {0};
	
	for(int i = 0; i < MAX_PTHREAD_NUM; ++i)
	{
		if(0 != pthread_create(&ptid[i], NULL, thread_run, NULL))
		{
			std::cout<<"create pthread error : " << i << std::endl;
			continue;
		}
	}

	while(1)
	{
		sleep(5);
	}
	return 0;
}
