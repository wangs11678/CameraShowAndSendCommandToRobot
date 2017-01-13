#include "sendCmdToRobot.h"

#define  PORT 8000    
#define IP_ADDRESS "192.168.1.210"

void send_help() {
	printf("0 ����socket����\n1 �������ڿ���\n2 ǰ\n3 ��\n4 ��\n5 ��\n6 ͣ\n7 �رմ��ڿ���\n8 �ر�socket����\n");
	printf("please enter your choice: \n");
}


void sendCmdToRobot(PVOID param)  
{  	
	unsigned char SendBuffer[] = {0xf0/*����socket����*/, 0xc0/*�������ڿ���*/, 0xc8/*ǰ*/, 0xc9/*��*/, 0xca/*��*/,0xcb/*��*/,
								  0xcc/*ͣ*/, 0x80/*�رմ��ڿ���*/, 0xff/*�ر�socket����*/};
	send_help();
	WSADATA  Ws;  
	SOCKET ClientSocket;  
	struct sockaddr_in ClientAddr;
	while(1)  
	{  
		int cmd;
		scanf("%d", &cmd);    
		int ret = 0; 
		if(cmd == 0) { 
			/* Init Windows Socket */  
			if(WSAStartup(MAKEWORD(2, 2), &Ws) != 0)  
			{  
				//printf("Init Windows Socket Failed::%d\n", GetLastError());  
				//return -1;  
			}  

			/* Create Socket */  
			ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
			if(ClientSocket == INVALID_SOCKET)  
			{  
				//printf("Create Socket Failed:%d\n", GetLastError());  
				//return -1;  
			}  

			ClientAddr.sin_family = AF_INET;  
			ClientAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);  
			ClientAddr.sin_port = htons(PORT);  
			memset(ClientAddr.sin_zero, 0x00, 8);  

			/* connect socket */  
			//printf("Connecting to Server...\n");
			ret = connect(ClientSocket,(struct sockaddr*)&ClientAddr, sizeof(ClientAddr));  
			if(ret == SOCKET_ERROR)  
			{  
				printf("Connect Server Error!\n"); 
				//printf("Connect Error::%d\n", GetLastError());  
				//return -1;  
			}  
			else  
			{  
				printf("Connect Server succeed!\n");  
			}  
		}
		/* send data to server */  
		if(cmd == 1 || cmd == 2 || cmd == 3 || cmd == 4 || cmd == 5 || cmd == 6 || cmd == 7 || cmd == 8){
			ret = send(ClientSocket, (char*) &SendBuffer[cmd], (int)strlen((char*)&SendBuffer[cmd]), 0);
		}
		//else
		//	printf("command error, please input again: \n");

		if (ret == SOCKET_ERROR){  
			printf("Send Info Error::%d\n", GetLastError());  
			//break;  
		}             
	}  
	/* close socket */  
	closesocket(ClientSocket);  
	WSACleanup(); 
}  
