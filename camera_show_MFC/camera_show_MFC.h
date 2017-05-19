
// camera_show_MFC.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;



// Ccamera_show_MFCApp:
// 有关此类的实现，请参阅 camera_show_MFC.cpp
//

class Ccamera_show_MFCApp : public CWinApp
{
public:
	Ccamera_show_MFCApp();
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	//void showImg(Mat src, UINT ID);
};

extern Ccamera_show_MFCApp theApp;
extern Mat frame1;	//左相机的图像
extern Mat frame2;  //右相机的图像
extern Mat m_buffer1;//用来存储临时相片的
extern Mat m_buffer2;//用来存储临时相片的
extern int nIndx;		//相机的索引
extern bool isOpen;	//判断相机是否打开
extern Size imageSize;//待标定的照片的大小

extern ofstream fout; //用来保存单个摄像头标定文件的ofstream对象


extern Mat cameraMatrix; /* 摄像机内参数矩阵 */  
extern Mat distCoeffs; /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */  
extern vector<Mat> tvecsMat;  /* 每幅图像的旋转向量 */  
extern vector<Mat> rvecsMat; /* 每幅图像的平移向量 */ 



extern ofstream fout_stereo; //用来保存两个摄像头标定文件的ofstream对象
extern Mat cameraMatrixL;
extern Mat distCoeffL;

extern Mat cameraMatrixR;
extern Mat distCoeffR;

extern vector<Mat> tvecsMatL;  /* 每幅左相机图像的旋转向量 */  
extern vector<Mat> rvecsMatL; /* 每幅左相机图像的平移向量 */ 

extern vector<Mat> tvecsMatR;  /* 每幅右相机图像的旋转向量 */  
extern vector<Mat> rvecsMatR; /* 每幅右相机图像的平移向量 */ 

extern Mat R, T, E, F;    //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵