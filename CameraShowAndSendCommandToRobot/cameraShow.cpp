#include "cameraShow.h"

#define  PORT 8885 

void cameraShow(PVOID param)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	const int blocksize = 28880;

	//������ṹ  
	struct recvbuf //����ʽ  
	{  
		char buf[blocksize]; //������ݵı���  
		int flag; //��־  
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
		printf("Socket ����ʧ�ܣ�Exit!");
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

	//����IplImageָ��  
	IplImage* pFrame = NULL;  
	//��ȡ����ͷ  
	//CvCapture* pCapture = cvCreateCameraCapture(0);  

	//��������  
	cvNamedWindow("video", 1);  

	int nRecvBuf = 1024*1024*10; //���ջ���10M  
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

			if(data.flag == 2)  //data.flag==2��һ֡�е����һ�����ݿ�  
			{   
				if(COUNT == 33)  
				{
					pFrame = cvCreateImageHeader(cvSize(640, 480), IPL_DEPTH_8U, 3);  

					cvSetData(pFrame, img, 640*3); //���յ������ݽ���һ֡ͼ��  
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