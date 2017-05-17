
// camera_show_MFC.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <opencv2/opencv.hpp>
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
extern Mat frame1,frame2;  //左右相机的图像
extern Mat m_buffer1,m_buffer2;//用来存储临时相片的
extern int nIndx;//相机的索引