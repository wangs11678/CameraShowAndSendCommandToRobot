#include "cameraShow.h"

#define  PORT 8885 

void cameraShow(PVOID param)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	const int blocksize = 28880;

	//定义包结构  
	struct recvbuf //包格式  
	{  
		char buf[blocksize]; //存放数据的变量  
		int flag; //标志  
	};  
	struct recvbuf data;  

	/* Init Windows Socket */ 
	if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0){
		printf("Init Windows Socket Failed:%d\n", GetLastError()); 
		return;
	}

	if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0){
		WSACleanup();
		return;
	}

	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	if(INVALID_SOCKET == sockSrv){
		printf("Socket 创建失败，Exit!");
		return;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(PORT);

	if(-1 == bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR))){
		printf("Server bind error!\n");
		return;
	}

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	//声明IplImage指针  
	IplImage* pFrame = NULL;  
	//获取摄像头  
	//CvCapture* pCapture = cvCreateCameraCapture(0);  

	//创建窗口  
	cvNamedWindow("video", 1);  

	int nRecvBuf = 1024*1024*10; //接收缓存10M  
	setsockopt(sockSrv, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
	int COUNT = 0;
	char img[blocksize*32] = {0};
	int n;

	//printf("sadfsadf\n");
	while(1)  
	{     
		//memset(data.buf, 0, sizeof(char)*blocksize);
		for(int i=0; i<32; i++)           
		{  
			//Sleep(1000);
			n = recvfrom(sockSrv, (char *)(&data), blocksize+4, 0, (SOCKADDR*)&addrClient, &len);  
			//Sleep(1000);
			//printf("%d\n", n);
			COUNT=COUNT + data.flag;  
			for(int k=0; k<blocksize; k++)  
			{
				img[i*blocksize+k] = data.buf[k];  
			}  

			if(data.flag == 2)  //data.flag==2是一帧中的最后一个数据块  
			{   
				if(COUNT == 33)  
				{
					pFrame = cvCreateImageHeader(cvSize(640, 480), IPL_DEPTH_8U, 3);  

					cvSetData(pFrame, img, 640*3); //由收到的数据建立一帧图像  
					cvShowImage("video", pFrame);
					char c = cvWaitKey(1000/30);  
					if(c == 27) break; 
					//printf("frame!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				}  
				else   
				{  
					COUNT = 0;  
					i = -1;  
				}   
			}  
		}      
	}  
	cvDestroyWindow("video");  
}