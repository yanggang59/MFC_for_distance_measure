
// camera_show_MFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <opencv2/opencv.hpp>
using namespace cv;



// Ccamera_show_MFCApp:
// �йش����ʵ�֣������ camera_show_MFC.cpp
//

class Ccamera_show_MFCApp : public CWinApp
{
public:
	Ccamera_show_MFCApp();
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	//void showImg(Mat src, UINT ID);
};

extern Ccamera_show_MFCApp theApp;
extern Mat frame1,frame2;  //���������ͼ��
extern Mat m_buffer1,m_buffer2;//�����洢��ʱ��Ƭ��
extern int nIndx;//���������