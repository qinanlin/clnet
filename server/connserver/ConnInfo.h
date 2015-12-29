#ifndef _CONNINFO_H_
#define _CONNINFO_H_

#include "Socket.h"
#include "CommMisc.h"
#include <iostream>

#define MAX_RECVBUFF_SIZE 1024
#define MAX_SENDBUFF_SIZE 1024

class ConnInfo
{
public:
	ConnInfo();
	~ConnInfo();
	
	Socket *GetSocket() const
	{
		return m_pstSocket;
	};

	void CleanConnInfo();

	void SetActive()
	{
		m_bActive = true;
	};

	void SetInActive()
	{
		m_bActive = false;
	};

	bool IsActive() const
	{
		return m_bActive;
	};
	
	
public:
	bool m_bActive; //0δ���� ��1����
	Socket *m_pstSocket;
	char *m_pRecvBuff;
	char *m_pSendBuff;
	unsigned int m_iData;		//���յ������ݳ���
	unsigned int m_iMsgLen;		//���ݰ�����,����length + proto
	unsigned int m_iOff;		//����ƫ����
	//int m_iNeedFree;			//�ͷ�����
};

#endif
