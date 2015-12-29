#include "Socket.h"

Socket::Socket()
{
	memset(&m_stSockAddrIn, 0, sizeof(m_stSockAddrIn));
	//memset(acHost, 0, sizeof(acHost));
	m_iSocket = -1;
}

Socket::Socket(int iType)
{
	memset(&m_stSockAddrIn, 0, sizeof(m_stSockAddrIn));
	//memset(acHost, 0, sizeof(acHost));
	m_iSocket = SocketAPI::socket_cl(AF_INET, iType, 0);
	//this->iPort = iPort;
}

int Socket::Send(int iFd, const void *pstBuff, size_t iSize, int iMs)
{
	int iRet;
	fd_set stWrite;
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	
	if(iMs)
	{
		tv.tv_sec = iMs / 1000;
		tv.tv_usec = (iMs % 1000) * 1000;
	
	
		FD_ZERO(&stWrite);
		FD_SET(iFd, &stWrite);
		
		iRet = ::select(iFd + 1, NULL, &stWrite, NULL, &tv);
		if(iRet < 0)
		{
			return -1;
		}
		else if(0 == iRet)
		{
			return 0;
		}
	}
	
	iRet = SocketAPI::send_cl(iFd, pstBuff, iSize, 0);
	if(iRet < 0)
	{
		if(EWOULDBLOCK == errno || EAGAIN == errno)
		{
			if(iMs)
				usleep(200000);  //会不会太长？
			return 0;
		}

		return -1;
	}
	else if(0 == iRet)
	{
		return -1;
	}
	
	return iRet;
}

int Socket::Recv(int iFd, void *pstBuff, size_t iSize, int iMs)
{
	int iRet;
	fd_set stRead;
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	
	if(iMs)
	{
		tv.tv_sec = iMs / 1000;
		tv.tv_usec = (iMs % 1000) * 1000;
	
		FD_ZERO(&stRead);
		FD_SET(iFd, &stRead);
		
		iRet = ::select(iFd + 1, &stRead, NULL, NULL, &tv);
		if(iRet < 0)
		{
			return -1;
		}
		else if(0 == iRet)
		{
			return 0;
		}
	}
	
	iRet = SocketAPI::recv_cl(iFd, pstBuff, iSize, 0);
	if(iRet < 0)
	{
		if(EWOULDBLOCK == errno || EAGAIN == errno)
		{
			if(iMs)
				usleep(200000);  //会不会太长？
			return 0;
		}

		return -1;
	}
	else if(0 == iRet)
	{
		return -1;
	}
	
	return iRet;
}

int Socket::Sendto(int iFd, void *pstBuff, size_t iLen, int iMs, 
			struct sockaddr *pstTo, size_t iToLen)
{
	int iRet;
	fd_set stWrite;
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	
	if(iMs)
	{
		tv.tv_sec = iMs / 1000;
		tv.tv_usec = (iMs % 1000) * 1000;
	}
	
	FD_ZERO(&stWrite);
	FD_SET(iFd, &stWrite);
	
	iRet = ::select(iFd + 1, NULL, &stWrite, NULL, &tv);
	if(iRet < 0)
	{
		return -1;
	}
	else if(0 == iRet)
	{
		return 0;
	}
	
	iRet = SocketAPI::sendto_cl(iFd, pstBuff, iLen, 0, pstTo, iToLen);
	if(iRet < 0)
	{
		if(EWOULDBLOCK == errno || EAGAIN == errno)
		{
			usleep(200000);  //会不会太长？
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else if(0 == iRet)
	{
		return -1;
	}
	
	return iRet;
}

int Socket::Recvfrom(int iFd, void *pstBuff, size_t iLen, int iMs,
				struct sockaddr *pstFrom, size_t *pstLen)
{
	int iRet;
	fd_set stRead;
	struct timeval tv;
	memset(&tv, 0, sizeof(tv));
	
	if(iMs)
	{
		tv.tv_sec = iMs / 1000;
		tv.tv_usec = (iMs % 1000) * 1000;
	}
	
	FD_ZERO(&stRead);
	FD_SET(iFd, &stRead);
	
	iRet = ::select(iFd + 1, NULL, &stRead, NULL, &tv);
	if(iRet < 0)
	{
		return -1;
	}
	else if(0 == iRet)
	{
		return 0;
	}
	
	iRet = SocketAPI::recvfrom_cl(iFd, pstBuff, iLen, 0, pstFrom, pstLen);
	if(iRet < 0)
	{
		if(EWOULDBLOCK == errno || EAGAIN == errno)
		{
			usleep(200000);  //会不会太长？
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else if(0 == iRet)
	{
		return -1;
	}
	
	return iRet;
}

bool Socket::SetNonBlocking(int iFlag)
{
	return SocketAPI::set_nonblocking(m_iSocket, iFlag);
}

int Socket::Accept(struct sockaddr *pstAdd, size_t *pstLen)
{
	
	return SocketAPI::accept_cl(m_iSocket, pstAdd, pstLen);
}

bool Socket::Listen(int iBacklog)
{
	return SocketAPI::listen_cl(m_iSocket, iBacklog);
}

bool Socket::Bind(const struct sockaddr *pstAdd, size_t iLen)
{
	return SocketAPI::bind_cl(m_iSocket, pstAdd, iLen);
}

bool Socket::CheckConnect()
{
	fd_set stWrite;
	int iRet;
	int iError;
	int iLen;
	
	FD_ZERO(&stWrite);
	FD_SET(m_iSocket, &stWrite);

	iRet = select(m_iSocket + 1, NULL, &stWrite, NULL, NULL);
	if(iRet < 0)
	{
		return false;
	}
	else if(0 == iRet)
	{
		usleep(20000);
		return true;
	}

	iError = 0;
	iLen = sizeof(iLen);

	//getsockopt(m_iSocket, SOL_SOCKET, SO_ERROR, &iError, &iLen);
	SocketAPI::getsockopt_cl(m_iSocket, SOL_SOCKET, SO_ERROR, (void*)&iError, (size_t *)&iLen);
	if(0 == iError)
		return true;
	else
		return false;
}

bool Socket::Connect(const struct sockaddr *pstAdd, size_t iLen)
{
	if(false == SocketAPI::connect_cl(m_iSocket, pstAdd, iLen))
	{
		return false;
	}
	return CheckConnect();
}

void Socket::Close()
{
	if(m_iSocket != -1)
	{
		SocketAPI::close_cl(m_iSocket);
	}
	memset(&m_stSockAddrIn, 0, sizeof(m_stSockAddrIn));
	//iPort = 0;
	//memset(acHost, 0, sizeof(acHost));
}

bool Socket::Shutdown(int iHowto)
{
	return SocketAPI::shutdown_cl(m_iSocket, iHowto);
}

bool Socket::SetReuseAddr()
{
	return SocketAPI::set_reuseaddr(m_iSocket);
}

bool Socket::SetLinger(int iTime)
{
	struct linger stling;
	
	stling.l_onoff = iTime > 0 ? 1 : 0;
	stling.l_linger = iTime;
	
	return SocketAPI::setsockopt_cl(m_iSocket, SOL_SOCKET, SO_LINGER, 
				&stling, sizeof(stling));
}

bool Socket::Getsockopt(int iLevel, int iOpt, void *pstOptVal, 
				size_t *pstOptLen) const
{
	return SocketAPI::getsockopt_cl(m_iSocket, iLevel, iOpt, 
				pstOptVal, pstOptLen);
}

bool Socket::Setsockopt(int iLevel, int iOpt, const void *pstOptVal,
				size_t iOptLen)
{
	return SocketAPI::setsockopt_cl(m_iSocket, iLevel, iOpt, 
				pstOptVal, iOptLen);
}

bool Socket::SetSendBuffSize(int iSize)
{
	return SocketAPI::setsockopt_cl(m_iSocket, SOL_SOCKET , SO_SNDBUF, 
				&iSize, sizeof(iSize));
}

int Socket::GetSendBuffSize() const
{
	int iBufSize;
	size_t iSize = sizeof(iBufSize);
	SocketAPI::getsockopt_cl(m_iSocket, SOL_SOCKET , SO_SNDBUF, 
				&iBufSize, &iSize);
	return iBufSize;
}

bool Socket::SetRecvBuffSize(int iSize)
{
	return SocketAPI::setsockopt_cl(m_iSocket, SOL_SOCKET , SO_RCVBUF, 
				&iSize, sizeof(iSize));
}

int Socket::GetRecvBuffSize() const
{
	int iBufSize;
	size_t iSize = sizeof(iBufSize);
	SocketAPI::getsockopt_cl(m_iSocket, SOL_SOCKET , SO_RCVBUF, 
				&iBufSize, &iSize);
	return iBufSize;
}

bool Socket::IsNonBlocking() const
{
	return SocketAPI::is_nonblocking(m_iSocket);
}
