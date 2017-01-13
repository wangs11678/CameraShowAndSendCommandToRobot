#include "cameraShow.h"
#include "sendCmdToRobot.h"
#include <process.h>

int main(int argc, char* argv[]) 
{
	HANDLE h[2]; 
	h[0] = (HANDLE)_beginthreadex(0, 0, (unsigned int (__stdcall *)(void *))cameraShow, 0, 0, 0); 
	h[1] = (HANDLE)_beginthreadex(0, 0, (unsigned int (__stdcall *)(void *))sendCmdToRobot, 0, 0, 0); 
	WaitForMultipleObjects(2, h, TRUE, INFINITE);  
	return 0;
}