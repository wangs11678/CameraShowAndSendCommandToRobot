#include <stdio.h>
#include <Winsock2.h>
#include <opencv/cv.h>  
#include <opencv/cxcore.h>  
#include <opencv/highgui.h>  

#pragma comment(lib, "ws2_32.lib")   

using namespace cv;

void cameraShow(PVOID param);