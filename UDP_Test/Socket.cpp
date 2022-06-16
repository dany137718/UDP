#include "Socket.h"

bool is_WSAStartup_RunSucceed = false;

Socket::Socket()
{
	portNum = 0;
	base_socket = act_socket = INVALID_SOCKET;
}

Socket::~Socket()
{
	closeSocket();
}

bool Socket::create(unsigned short in_portNum)
{
	portNum = in_portNum;
	base_socket = act_socket = INVALID_SOCKET;

#ifdef SAVE_SOCKET_SERVER_CONNECTIVITY_LOGS
	utilityFuncInstance->socketCreateLog(portNum , "create");
#endif

#ifndef __LINUX
	if(is_WSAStartup_RunSucceed == false)
	{
		WSADATA wsaData;
		int WSAStartup_ret = WSAStartup(MAKEWORD(2,2) , &wsaData);
		if (WSAStartup_ret != NO_ERROR)
		{
			printf("\nError at WSAStartup() \n");
			return false;
		}

		is_WSAStartup_RunSucceed = true;
	}
#endif

	base_socket = socket(AF_INET , SOCK_STREAM , 0);

	if ( base_socket == INVALID_SOCKET )
	{
		printf("\nSocket Creating Error -> port No = %i \n" , portNum);
		return false;
	}

	memset(&service , 0 , sizeof(service));

	service.sin_family = AF_INET;
	service.sin_port = htons( portNum );

	return true;
}

bool Socket::startServer(bool isExitProcessIfFail)
{
#ifdef SAVE_SOCKET_SERVER_CONNECTIVITY_LOGS
	utilityFuncInstance->socketCreateLog(portNum , "startServer");
#endif

	int th_flag = 1;
	setsockopt(base_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&th_flag, sizeof(th_flag));

#ifdef SO_REUSEPORT
	setsockopt(base_socket, SOL_SOCKET, SO_REUSEPORT, (char *)&th_flag, sizeof(th_flag));
#endif

	service.sin_addr.s_addr = INADDR_ANY;

	if ( ::bind(base_socket , (struct sockaddr *) &service , sizeof(service) ) != 0 )
	{
		printf("\nSocket Binding Error -> port No = %i \n" , portNum);

		closesocket(base_socket);

		if(isExitProcessIfFail == true)
		{
			char exitAndLog_stream[100];
			sprintf_s(exitAndLog_stream , _ArraySizeMacro_(exitAndLog_stream) ,  "Socket Binding Error -> port No = %i" , portNum);
			utilityFuncInstance->exitAndLog(ERROR_LOG_TYPE , exitAndLog_stream);
		}

		return false;
	}

	if ( listen(base_socket , 1) != 0 )
	{
		printf("\nError listening on socket -> port No = %i \n" , portNum);

		closesocket(base_socket);

		if(isExitProcessIfFail == true)
		{
			char exitAndLog_stream[100];
			sprintf_s(exitAndLog_stream , _ArraySizeMacro_(exitAndLog_stream) , "Error listening on socket -> port No = %i" , portNum);
			utilityFuncInstance->exitAndLog(ERROR_LOG_TYPE , exitAndLog_stream);
		}

		return false;
	}

	return true;
}

void Socket::waitForClientToConnect()
{
#ifdef SAVE_SOCKET_SERVER_CONNECTIVITY_LOGS
	utilityFuncInstance->socketCreateLog(portNum , "waitForClientToConnect");
#endif

	SOCKET AcceptSocket = INVALID_SOCKET;
	while ( AcceptSocket == INVALID_SOCKET )
	{
		AcceptSocket = accept( base_socket, NULL, NULL );
		Sleep(1);
	}

	act_socket = AcceptSocket;
	closesocket(base_socket);

#ifdef SAVE_SOCKET_SERVER_CONNECTIVITY_LOGS
	utilityFuncInstance->socketCreateLog(portNum , "waitForClientToConnect Succeed");
#endif
}

bool Socket::startClient(const char * in_ipAddr)
{
	strcpy_s(ipAddr , _ArraySizeMacro_(ipAddr) , in_ipAddr);
	service.sin_addr.s_addr = inet_addr( ipAddr );

	act_socket = base_socket;

	int th_connect_ret = connect(act_socket , (struct sockaddr *) &service , sizeof(service));
	if (th_connect_ret != 0)
	{

#ifdef CONSOL_WRITE_ACT
		printf("\nSocket Client Connecting Error -> port No = %i \n" , portNum);
#endif

		return false;
	}

	return true;
}

int Socket::sendBuf(char * sendbuf , int bufsize)
{
	int bytesSent = send(act_socket , sendbuf , bufsize , 0);
	return bytesSent;
}

int Socket::receiveBuf(char * recvbuf , int bufsize)
{
	int allBytesRecv = 0 , th_recvReturn = 0;
	do
	{
		th_recvReturn = recv(act_socket , recvbuf + allBytesRecv , bufsize - allBytesRecv , 0);
		allBytesRecv += th_recvReturn;

	}while(allBytesRecv < bufsize && th_recvReturn > 0);

	return allBytesRecv;
}

bool Socket::closeSocket()
{
#ifdef SAVE_SOCKET_SERVER_CONNECTIVITY_LOGS
	if(portNum > 0)
		utilityFuncInstance->socketCreateLog(portNum , "closeSocket");
#endif

	shutdown(act_socket , 2);

	Sleep(500);
	int closesocket_ret = closesocket(act_socket);

	base_socket = act_socket = INVALID_SOCKET;

	if(closesocket_ret == 0)
		return true;

	return false;
}

void Socket::setSocket_send_recv_timeOut(int in_timeOutVal_miliSecond)
{
#ifndef __LINUX
	setsockopt(act_socket , SOL_SOCKET , SO_SNDTIMEO , (char *)&in_timeOutVal_miliSecond , sizeof(in_timeOutVal_miliSecond));
	setsockopt(act_socket , SOL_SOCKET , SO_RCVTIMEO , (char *)&in_timeOutVal_miliSecond , sizeof(in_timeOutVal_miliSecond));
#else
	struct timeval th_timeoutVal;

	th_timeoutVal.tv_sec = (int)(in_timeOutVal_miliSecond / 1000);
	th_timeoutVal.tv_usec = (int)(in_timeOutVal_miliSecond % 1000) * 1000;

	setsockopt(act_socket , SOL_SOCKET , SO_SNDTIMEO , (char *)&th_timeoutVal , sizeof(th_timeoutVal));
	setsockopt(act_socket , SOL_SOCKET , SO_RCVTIMEO , (char *)&th_timeoutVal , sizeof(th_timeoutVal));
#endif

	int th_flag = 1;
	setsockopt(act_socket , IPPROTO_TCP , TCP_NODELAY , (char *)&th_flag , sizeof(th_flag));
}

