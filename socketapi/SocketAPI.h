/* Name:	SocketAPI.h
 * Description:		Socket API for Linux
 * Author:	Chris Lin
 * Creation Date:	2014/3/2
 * Modified:	**** by ***
*/

#ifndef __SOCKETAPI_H_
#define __SOCKETAPI_H_


#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>


#ifndef __cplusplus

typedef int bool
#define true 	1
#define false	0

#endif

#define SOCKET_ERROR	-1

#ifdef __cplusplus
namespace SocketAPI
{
#endif

	int socket_cl(int iDomain, int iType, int iProtocol);
	
	bool bind_cl(int iFd, const struct sockaddr *pstAdd, size_t iLen);
	
	bool listen_cl(int iFd, int iBacklog);
	
	int close_cl(int iFd);
	
	bool connect_cl(int iFd, const struct sockaddr *pstAdd, size_t iLen);
	
	int accept_cl(int iFd, struct sockaddr *pstAdd, size_t *pstLen);
	
	/*int select_cl(int iFdNum, fd_set *pstRead, fd_set *pstWrite, 
				fd_set *pstErr, struct timeval *pstTimeOut);*/
	
	bool setsockopt_cl(int iFd, int iLevel, int iOpt, const void *pstOptVal,
				size_t iOptLen);
	
	bool getsockopt_cl(int iFd, int iLevel, int iOpt, void *pstOptVal, 
				size_t *pstOptLen);
				
	bool is_nonblocking(int iFd);
	
	bool set_reuseaddr(int iFd);
	
	bool set_nonblocking(int iFd, int iNon);
	
	bool shutdown_cl(int iFd, int iHowto);
	
	int send_cl(int iFd, const void *pstBuff, size_t iSize, int iFlag);
	
	int recv_cl(int iFd, void *pstBuff, size_t iSize, int iFlag);
	
	int sendto_cl(int iFd, void *pstBuff, size_t iLen, int iFlag, 
			struct sockaddr *pstTo, size_t iToLen);
	
	int recvfrom_cl(int iFd, void *pstBuff, size_t iLen, int iFlag,
				struct sockaddr *pstFrom, size_t *pstLen);

#ifdef __cplusplus			
}
#endif


#endif
