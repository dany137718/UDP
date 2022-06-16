#ifndef _SOCKET_H_
#define _SOCKET_H_

extern bool is_WSAStartup_RunSucceed;

#include "Constant.h"
#include "UtilityFunc.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#ifndef __LINUX

#include <windows.h>

#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

#define SOCKET int
#define INVALID_SOCKET (~0)
#define closesocket close

#endif

using namespace std;

class Socket
{
	SOCKET base_socket , act_socket;
	sockaddr_in service;
	char ipAddr[30];
	unsigned short portNum;

public :

	Socket();
	~Socket();

	bool create(unsigned short portNum);
	bool startServer(bool isExitProcessIfFail = true);
	bool startClient(const char * in_ipAddr);
	void waitForClientToConnect();
	int sendBuf(char *sendbuf, int bufsize);
	int receiveBuf(char *recvbuf, int bufsize);
	void setSocket_send_recv_timeOut(int in_timeOutVal_miliSecond);

	bool closeSocket();
};

///////////////////////////
///////// end of file /////
#endif
