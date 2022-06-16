#include "Constant.h"
#include "Socket.h"

bool isSenderActived = false;
unsigned short udpPort = 0;

void udpBroadCastSender();
void udpBroadCastReceiver();
void loadParmas();

////////////////////////////////////////////////////////

ThreadReturn SocketThread(void * arg)
{
    if (isSenderActived)
        udpBroadCastSender();
    else
        udpBroadCastReceiver();
#ifndef __LINUX
	return 1;
#endif
}

////////////////////////////////////////////////////////

void createAllThreads()
{
	HANDLE H_SocketThread;

#ifndef __LINUX
	unsigned int th_Thread_Addr;
	H_SocketThread = (HANDLE)_beginthreadex(NULL , 0 , &SocketThread , NULL , 0 , &th_Thread_Addr);
	Sleep(300);
	SetThreadAffinityMask((HANDLE)H_SocketThread , 32768);
#else
	pthread_create(&H_SocketThread[i] , NULL , SocketThread , (void *)(&i));
	Sleep(300);
#endif
}

////////////////////////////////////////////////////////

int main(int argc , char ** argv)
{
    loadParmas();
    
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    createAllThreads();
	
	Sleep(100);


#ifndef __LINUX
	SetProcessAffinityMask(GetCurrentProcess() , 32768);
#endif

	while (1)
	{
		Sleep(10000);
	}
	return 0;
}


void udpBroadCastSender()
{
    cout << "Start Sending BroadCast ... Port Number : " << udpPort << endl << endl;

    const int th_sendbuff_size = 300;
    char th_sendbuff[300];

    char broadcast = '1';

    char act_base_ip_Str[20], act_mask_ip_Str[20], act_gateway_ip_Str[20];
    long long id = 0;
    //////////////////////////////////////////////////////
    while (1)
    {
        SOCKET sock;

        sock = socket(AF_INET, SOCK_DGRAM, 0);

        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
        {
            cout << "Error in setting Broadcast option";
            closesocket(sock);
            Sleep(4000);
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in Recv_addr;
        int len = sizeof(struct sockaddr_in);
        
        memset(&Recv_addr, 0, sizeof(Recv_addr));

        Recv_addr.sin_family = AF_INET;
        Recv_addr.sin_port = htons(udpPort);

        //TODO : what is this ??? subnet ? https://www.cs.ubbcluj.ro/~dadi/compnet/labs/lab3/udp-broadcast.html
        Recv_addr.sin_addr.s_addr = 0xffffffff;


        ////////////////////////////////////////////////////////////////

        while (2)
        {
            sprintf(act_base_ip_Str, (char *)"%d.%d.%d.%d", 10, 13, 110, 250);

            sprintf(act_mask_ip_Str, (char*)"%d.%d.%d.%d", 255, 255, 0, 0);

            sprintf(act_gateway_ip_Str, (char*)"%d.%d.%d.%d", 0, 0, 0, 0);

            sprintf(th_sendbuff, (char*)"{\"Counter\":\"%i\" , \"Frequence\":\"%.2f\" , \"Base_ip\":\"%s\" , \"Mask_ip\":\"%s\" , \"Gateway_ip\":\"%s\"}",
                id, 10.0,
                act_base_ip_Str, act_mask_ip_Str, act_gateway_ip_Str);

            //////////////////////////////////////////////////
            cout << th_sendbuff << endl;
            int sendto_ret = sendto(sock, th_sendbuff, th_sendbuff_size, 0, (sockaddr*)&Recv_addr, sizeof(Recv_addr));

            if (sendto_ret != th_sendbuff_size)
                break;

            id++;

            Sleep(5000);
        } // end of while 2

        closesocket(sock);
        Sleep(10000);
    } // end of while 1

}

void udpBroadCastReceiver()
{
    cout << "Start Receiving BroadCast ... Port Number : "  << udpPort << endl << endl;

    char recvbuff[300];
    int recvbufflen = 300;

    char broadcast = '1';

    char act_base_ip_Str[20], act_mask_ip_Str[20], act_gateway_ip_Str[20];
    long long id = 0;
    //////////////////////////////////////////////////////
    while (1)
    {
        SOCKET sock;

        sock = socket(AF_INET, SOCK_DGRAM, 0);

        if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
        {
            cout << "Error in setting Broadcast option" << endl;
            closesocket(sock);
            Sleep(4000);
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in Recv_addr;
        struct sockaddr_in Sender_addr;

        int len = sizeof(struct sockaddr_in);

        memset(&Recv_addr, 0, sizeof(Recv_addr));

        Recv_addr.sin_family = AF_INET;
        Recv_addr.sin_port = htons(udpPort);
        Recv_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sock, (const sockaddr*)&Recv_addr, (int)sizeof(Recv_addr)) < 0)
        {
            cout << "Error in BINDING" << WSAGetLastError() << endl;
            closesocket(sock);
            Sleep(4000);
            exit(EXIT_FAILURE);
        }

        ////////////////////////////////////////////////////////////////

        while (2)
        {
            
            int recvfrom_ret = recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr*)&Sender_addr, &len);
            cout << "\n\n\tReceived Message is : " << recvbuff;

            id++;

            Sleep(5000);
        } // end of while 2

        closesocket(sock);
        Sleep(10000);
    } // end of while 1
}

void loadParmas()
{
	try
	{
		FileStorage fs_read("UDP_Params.XML", FileStorage::READ);

		if (fs_read.isOpened())
		{
            string th_type = (string)(fs_read["Type"]);
            udpPort = (unsigned short)((int)fs_read["Port"]);

            if (th_type.compare("SENDER") == 0)
                isSenderActived = true;
            else if(th_type.compare("RECEIEVER") == 0)
                isSenderActived = false;
            else
            {
                cout << "type in xml is wrong ..." << endl;
                Sleep(4000);
                exit(EXIT_FAILURE);
            }

			fs_read.release();
		}
		else
		{
			cout << "Error ..." << endl << "UDP_Params.XML Damaged or not Found !" << endl;
			fs_read.release();
			Sleep(4000);
			exit(EXIT_FAILURE);
		}
	}
	catch (const cv::Exception& e)
	{
		cout << "Error ..." << endl << "Something Wrong : " << endl << "\t" << e.msg;
		Sleep(4000);
		exit(EXIT_FAILURE);
	}
}