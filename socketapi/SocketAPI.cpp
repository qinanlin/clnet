#include "SocketAPI.h"

extern int errno;

int SocketAPI::socket_cl(int iDomain, int iType, int iProtocol)
{
	int iFd = ::socket(iDomain, iType, iProtocol);
	
	if(SOCKET_ERROR == iFd)
	{
		switch(errno)  //error info
		{
		case EPROTONOSUPPORT:
		case EMFILE: 
		case ENFILE: 
		case EACCES: 
		case ENOBUFS: 
		default:
			break;
		}
		
		return SOCKET_ERROR;
	}
	
	return iFd;
}
	
bool SocketAPI::bind_cl(int iFd, const struct sockaddr *pstAdd, size_t iLen)
{	
	if(SOCKET_ERROR == ::bind(iFd, pstAdd, iLen))
	{
		switch(errno)
		{
		case EADDRINUSE:	//Address already in use
		case EBADF:			//Bad file number
		case ENOTSOCK:		//Not a socket
		case EINVAL:		//Invalid argument
		case EADDRNOTAVAIL:	//Address not available
		case EACCES:		//Permission denied
		case ENOTDIR:		//Not a directory
		case ENAMETOOLONG:	//Filename too long
		default:
			break;
		}
		return false;
	}
	
	return true;
}

bool SocketAPI::listen_cl(int iFd, int iBacklog)
{
	if(SOCKET_ERROR == ::listen(iFd, iBacklog))
	{
		switch(errno)
		{
		case EBADE:
		case EINVAL:
		case ENOTSOCK:
		case EOPNOTSUPP:
		default:
			break;
		}
		
		return false;
	}
	
	return true;
}

int SocketAPI::close_cl(int iFd)
{
	return ::close(iFd);
}

bool SocketAPI::connect_cl(int iFd, const struct sockaddr *pstAdd, size_t iLen)
{
	if(SOCKET_ERROR == ::connect(iFd, pstAdd, iLen))
	{
		switch(errno)
		{
		case EALREADY: 
		case EINPROGRESS: 
		case ECONNREFUSED: 
		case EISCONN: 
		case ETIMEDOUT: 
		case ENETUNREACH: 
		case EADDRINUSE: 
		case EBADF: 
		case EFAULT: 
		case ENOTSOCK:
		default:
			break; 
		}
		return false;
	}
	
	return true;
}

int SocketAPI::accept_cl(int iFd, struct sockaddr *pstAdd, size_t *pstLen)
{
	int iClient = ::accept(iFd, pstAdd, (socklen_t*)pstLen);
	if(SOCKET_ERROR == iClient)
	{
		switch(errno)
		{
		case EWOULDBLOCK: 
		case ECONNRESET:
		case ECONNABORTED:
		case EPROTO:
		case EINTR:
			// with nonblocking-socket, ignore above errors
			return 0;
		case EBADF: 
		case ENOTSOCK: 
		case EOPNOTSUPP: 
		case EFAULT:
		default:
			break;
		}
		return SOCKET_ERROR;
	}
	return iClient;
}

/*int select_cl(int iFdNum, fd_set *pstRead, fd_set *pstWrite, 
				fd_set *pstErr, struct timeval *pstTimeOut)
{
}*/
bool SocketAPI::setsockopt_cl(int iFd, int iLevel, int iOpt, const void *pstOptVal,
				size_t iOptLen)
{
	if(SOCKET_ERROR == ::setsockopt(iFd, iLevel, iOpt, pstOptVal, iOptLen))
	{
		switch ( errno ) 
		{
		case EBADF: 
		case ENOTSOCK: 
		case ENOPROTOOPT: 
		case EFAULT: 
		default:
			break;
		}
		return false;
	}
	return true;
}

bool SocketAPI::getsockopt_cl(int iFd, int iLevel, int iOpt, void *pstOptVal, 
				size_t *pstOptLen)
{
	if(SOCKET_ERROR == ::getsockopt(iFd, iLevel, iOpt, pstOptVal, (socklen_t*)pstOptLen))
	{
		switch ( errno ) 
		{
		case EBADF: 
		case ENOTSOCK: 
		case ENOPROTOOPT: 
		case EFAULT: 
		default:
			break;
		}
		return false;
	}
	return true;
}	

bool SocketAPI::is_nonblocking(int iFd)
{
	int iFlag = ::fcntl(iFd, F_GETFL, 0);
	
	if(iFlag & O_NONBLOCK)
	{
		return true;
	}
	return false;
}

bool SocketAPI::set_reuseaddr(int iFd)
{
	int iReuse = 1;
	if(setsockopt(iFd, SOL_SOCKET, SO_REUSEADDR, &iReuse,
			sizeof(iReuse)) == -1)
	{
		return false;
	}
	return true;
}

bool SocketAPI::set_nonblocking(int iFd, int iNon)
{
	int iFlag = ::fcntl(iFd, F_GETFL, 0);
	
	if(0 > iFlag)
	{
		return false;
	}
	
	if(iNon)
	{
		iFlag |= O_NONBLOCK;
	}
	else
	{
		iFlag &= ~O_NONBLOCK;
	}

	if(fcntl(iFd, F_SETFL, iFlag) < 0)
	{
		return false;
	}
	return true;
}

/*iHowto:
	SHUT_RD   close read
	SHUT_WR	  close write
	SHUT_RDWR close read and write
*/
bool SocketAPI::shutdown_cl(int iFd, int iHowto)
{
	if(SOCKET_ERROR == ::shutdown(iFd, iHowto))
	{
		switch(errno)
		{
		case EBADF : 
		case ENOTSOCK : 
		case ENOTCONN : 
		default : 
			break;
		}
		return false;
	}
	return true;
}

int SocketAPI::send_cl(int iFd, const void *pstBuff, size_t iSize, int iFlag)
{
	int iRet = ::send(iFd, pstBuff, iSize, iFlag);
	
	if(SOCKET_ERROR == iRet)
	switch(errno)
	{
	case EINTR:
	case EWOULDBLOCK:	//may  be same value as EAGAIN
	//以上两种情况连接保持
	case ECONNRESET:
	case EPIPE:
	case EBADF: 
	case ENOTSOCK: 
	case EFAULT: 
	case EMSGSIZE: 
	case ENOBUFS:
	default:
		break;
	}
	else if(0 == iRet)
	{
		//the connection has been gracefully closed
	}
	
	return iRet;
}

int SocketAPI::recv_cl(int iFd, void *pstBuff, size_t iSize, int iFlag)
{
	int iRet = ::recv(iFd, pstBuff, iSize, iFlag);
	
	if(SOCKET_ERROR == iRet)
	{
		switch(errno)
		{
		case EWOULDBLOCK:
		case EINTR:
		//以上两种情况连接保持
		case ECONNRESET :
		case EPIPE :
		case EBADF : 
		case ENOTCONN : 
		case ENOTSOCK : 
		case EFAULT : 

		default:
			break;
		}
	}
	else if(0 == iRet)
	{
		//the connection has been gracefully closed
	}
	return iRet;
}

int SocketAPI::sendto_cl(int iFd, void *pstBuff, size_t iLen, int iFlag, 
			struct sockaddr *pstTo, size_t iToLen)
{
	int iRet = ::sendto(iFd, pstBuff, iLen, iFlag, pstTo, iToLen);
	
	if(SOCKET_ERROR == iRet)
	{
		switch(errno)
		{
		case EWOULDBLOCK:
			return 0;
		case ECONNRESET :
		case EPIPE :
		case EBADF : 
		case ENOTCONN : 
		case ENOTSOCK : 
		case EFAULT : 

		default:
			break;
		}
	}

	return iRet;
}

int SocketAPI::recvfrom_cl(int iFd, void *pstBuff, size_t iLen, int iFlag,
				struct sockaddr *pstFrom, size_t *pstLen)
{
	int iRet = ::recvfrom(iFd, pstBuff, iLen, iFlag, pstFrom, (socklen_t*)pstLen);
	if(SOCKET_ERROR == iRet) 
	{
		switch ( errno ) 
		{
		case EWOULDBLOCK: 
			
		case ECONNRESET:
		case EPIPE:

		case EBADF: 
		case ENOTCONN: 
		case ENOTSOCK: 
		case EINTR: 
		case EFAULT: 

		default : 
			break;
		}
	}
	return iRet;
}
