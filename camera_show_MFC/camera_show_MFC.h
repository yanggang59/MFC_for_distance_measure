
// camera_show_MFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
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
extern Mat frame1;	//�������ͼ��
extern Mat frame2;  //�������ͼ��
extern Mat m_buffer1;//�����洢��ʱ��Ƭ��
extern Mat m_buffer2;//�����洢��ʱ��Ƭ��
extern int nIndx;		//���������
extern bool isOpen;	//�ж�����Ƿ��
extern Size imageSize;//���궨����Ƭ�Ĵ�С

extern ofstream fout; //�������浥������ͷ�궨�ļ���ofstream����


extern Mat cameraMatrix; /* ������ڲ������� */  
extern Mat distCoeffs; /* �������5������ϵ����k1,k2,p1,p2,k3 */  
extern vector<Mat> tvecsMat;  /* ÿ��ͼ�����ת���� */  
extern vector<Mat> rvecsMat; /* ÿ��ͼ���ƽ������ */ 



extern ofstream fout_stereo; //����������������ͷ�궨�ļ���ofstream����
extern Mat cameraMatrixL;
extern Mat distCoeffL;

extern Mat cameraMatrixR;
extern Mat distCoeffR;

extern vector<Mat> tvecsMatL;  /* ÿ�������ͼ�����ת���� */  
extern vector<Mat> rvecsMatL; /* ÿ�������ͼ���ƽ������ */ 

extern vector<Mat> tvecsMatR;  /* ÿ�������ͼ�����ת���� */  
extern vector<Mat> rvecsMatR; /* ÿ�������ͼ���ƽ������ */ 

extern Mat R, T, E, F;    //R ��תʸ�� Tƽ��ʸ�� E�������� F��������