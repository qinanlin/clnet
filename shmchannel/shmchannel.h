#ifndef SHMCHANNEL_H_
#define SHMCHANNEL_H_

#include "CommMisc.h"
#include "sharemem.h"
#include "sharemem.cpp"


#define MAX_CHANNEL_SIZE 10240000
#define CHANNEL_MSG_LEN 4

/*#ifdef __cplusplus
extern "C"
{
#endif*/

	struct ChannelInfo
	{
		int Ver;
		unsigned int Head;
		unsigned int Tail;
		char Channel[MAX_CHANNEL_SIZE];
	};


/*#ifdef __cplusplus
}
#endif*/



class ShmChannel
{
public:
	ShmChannel();
	~ShmChannel();

	bool SC_init(int, int);
	bool SC_load(int , int);
	void SC_del();

	/*bool SC_msgsend(char *, int);
	int SC_msgrecv(char *);
private:*/
	bool SC_send(char *, int);
	int SC_recv(char *, int);

	void SC_recvchan_check();
	void SC_sendchan_check();
	
private:
	ShareMem<ChannelInfo> m_stSendChan;		//两个进程的ipckey必须相反
	ShareMem<ChannelInfo> m_stRecvChan;
};

#endif