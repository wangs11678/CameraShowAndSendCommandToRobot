#include "sendCmdToRobot.h"

#define  PORT 8000    
#define IP_ADDRESS "192.168.1.210"

void send_help() {
	printf("0 请求socket连接\n1 启动串口控制\n2 前\n3 后\n4 左\n5 右\n6 停\n7 关闭串口控制\n8 关闭socket连接\n");
	printf("please enter your choice: \n");
}


void sendCmdToRobot(PVOID param)  
{  	
	unsigned char SendBuffer[] = {0xf0/*请求socket连接*/, 0xc0/*启动串口控制*/, 0xc8/*前*/, 0xc9/*后*/, 0xca/*左*/,0xcb/*右*/,
								  0xcc/*停*/, 0x80/*关闭串口控制*/, 0xff/*关闭socket连接*/};
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
