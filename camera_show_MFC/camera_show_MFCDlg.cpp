
// camera_show_MFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "camera_show_MFCDlg.h"
#include "afxdialogex.h"
#include  "resource.h"
#include <string>
#include "CalibDlg.h"
#include "StereoCali.h"
#include "ShotDlg.h"
#include "globalVar.h"
#include <opencv2/opencv.hpp>
#include "CvvImage.h"   //""��ʾ�ڵ�ǰ�����ļ�����Ѱ�Ҹ��ļ���û������include���ļ�����Ѱ��
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//�ҽ�����ȫ�ֱ���
//ȫ�ֱ���


//Mat frame1,frame2;
// Ccamera_show_MFCDlg �Ի���
VideoCapture cap1,cap2;   //�����������
VideoWriter writer1;
VideoWriter writer2;
//isOpen=false;



Ccamera_show_MFCDlg::Ccamera_show_MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ccamera_show_MFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ccamera_show_MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ccamera_show_MFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Ccamera_show_MFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OPEN, &Ccamera_show_MFCDlg::OnBnClickedOpen)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CLOSE, &Ccamera_show_MFCDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_VIDEO, &Ccamera_show_MFCDlg::OnBnClickedVideo)
	ON_BN_CLICKED(IDC_CALIB, &Ccamera_show_MFCDlg::OnBnClickedCalib)
	ON_BN_CLICKED(IDC_STEREO_CALI, &Ccamera_show_MFCDlg::OnBnClickedStereoCali)
	ON_BN_CLICKED(IDC_SHOT, &Ccamera_show_MFCDlg::OnBnClickedShot)
	ON_BN_CLICKED(IDC_SHOWDISPARITY, &Ccamera_show_MFCDlg::OnBnClickedShowdisparity)
	ON_BN_CLICKED(IDC_READFILE, &Ccamera_show_MFCDlg::OnBnClickedReadfile)
END_MESSAGE_MAP()


// Ccamera_show_MFCDlg ��Ϣ�������

BOOL Ccamera_show_MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//CRect rect;
	//GetDlgItem(IDC_CAMERA1)->GetWindowRect(&rect);           //IDC_WAVE_DRAWΪPicture Control��ID  
	//ScreenToClient(&rect);  
	//GetDlgItem(IDC_CAMERA1)->MoveWindow(rect.left, rect.top, 640, 480, true);    //�̶�Picture Control�ؼ��Ĵ�С

	CComboBox* pComb=(CComboBox *)GetDlgItem(IDC_CAMERAINDEX);
	pComb->AddString("1");
	pComb->AddString("2");
	pComb->AddString("Both");
	pComb->SetCurSel(2);			//Ĭ������ΪBothģʽ�������������ѡ��
	nIndx=pComb->GetCurSel();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Ccamera_show_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Ccamera_show_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Ccamera_show_MFCDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void Ccamera_show_MFCDlg::OnBnClickedOpen()
{
	CComboBox* pComb=(CComboBox *)GetDlgItem(IDC_CAMERAINDEX);
	nIndx=pComb->GetCurSel();
	if(-1==nIndx)		//���û��ѡ��
	{
		//AfxMessageBox("You haven't chosen any camera");
		AfxMessageBox("����ѡ�����������");
		return;
	}

	if(0==nIndx)			//ѡ��1�����
	{
	cap1.open(0);
	if(!cap1.isOpened())
	{
		AfxMessageBox("Error���޷������1���������1�Ƿ����");
		return;
	}
	isOpen=true;
	isLeftOpen=true;
	cap1>>frame1;
	showImg(frame1,IDC_CAMERA1);
	SetTimer(1,30,NULL);
	}

	if(1==nIndx)				 //ѡ��2�����
	{
	cap2.open(1);
	if(!cap2.isOpened())
	{
		AfxMessageBox("Error:�޷������2���������2�Ƿ����");
		return;
	}
	isOpen=true;		//������ڴ�״̬
	isRightOpen=true;//��������ڴ�״̬
	cap2>>frame2;
	showImg(frame2,IDC_CAMERA2);
	SetTimer(2,30,NULL);
	}
	if(nIndx==2)
	{
		cap1.open(0);
		cap2.open(1);
		if(!cap1.isOpened())
	{	
		AfxMessageBox("Error���޷������1,�������1�Ƿ����");
		return;
	}
		isLeftOpen=true;			//��������ڴ�״̬
		cap1>>frame1;
		showImg(frame1,IDC_CAMERA1);
		SetTimer(1,30,NULL);
		isOpen=true;				//��һ��������ڴ�״̬
		if(!cap2.isOpened())
	{
		AfxMessageBox("Error:�޷������2���������2�Ƿ����");
		return;
	}
	isRightOpen=true;
	}
	cap2>>frame2;
	showImg(frame2,IDC_CAMERA2);
	SetTimer(2,30,NULL);
}


 void Ccamera_show_MFCDlg::showImg(Mat src, UINT ID)//�Զ��庯������Mat��ʾ�ڿؼ���
{
	CDC *pDC = GetDlgItem(ID)->GetDC();//����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��
	HDC hdc= pDC->GetSafeHdc();           // ��ȡ�豸�����ľ��
	CRect rect;
   GetDlgItem(ID)->GetClientRect(&rect); //��ȡbox1�ͻ���
   CvvImage cimg;
   IplImage img(src); // ����IplImage����img     
   cimg.CopyOf(&img,img.nChannels);
   cimg.DrawToHDC(hdc,&rect);
   ReleaseDC( pDC );
   cimg.Destroy();

}


void Ccamera_show_MFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
		{
			cap1>>frame1;
			if(frame1.empty())
				return;
			showImg(frame1,IDC_CAMERA1);
			break;
		}
		case 2:
		{
			cap2>>frame2;
			if(frame2.empty())
				return;
			showImg(frame2,IDC_CAMERA2);
			break;
		}
		case 3:
			{
				writer1<<frame1;
				break;
		   }
		case 4:
			{
				writer2<<frame2;
				break;
			}
		case 5:
			{
				if(frame1.empty()||frame2.empty()) return;
				cvtColor(frame1, grayImageL, CV_BGR2GRAY);
				cvtColor(frame2, grayImageR, CV_BGR2GRAY);
				remap(grayImageL, rectifyImageL, mapLx, mapLy, INTER_LINEAR);
				remap(grayImageR, rectifyImageR, mapRx, mapRy, INTER_LINEAR);
				bm(rectifyImageL, rectifyImageR, disp);
				disp.convertTo(disp8, CV_8U, 255 / ((numberOfDisparities * 16 + 16)*16.));//��������Ӳ���CV_16S��ʽ
				showImg(disp8,IDC_DISPARITY);
				break;
			}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


void Ccamera_show_MFCDlg::OnBnClickedClose()
{
	if(isOpen)
	{
	Mat gray(Size(500,400),CV_32FC3, Scalar(15,15,15));
	cap1.release();								//�ͷ�VideoCapture��Դ
	showImg(gray,IDC_CAMERA1);     
	KillTimer(1);

	cap2.release();
	showImg(gray,IDC_CAMERA2);
	showImg(gray,IDC_DISPARITY);
	KillTimer(2);
	KillTimer(3);
	KillTimer(4);
	KillTimer(5);
	isOpen=false;
	}
	else
	{
		//AfxMessageBox("The camera is closed,no need to close again");
		AfxMessageBox("����ͷ�Ѿ����ڹر�״̬");
		return;
	}
}


void Ccamera_show_MFCDlg::OnBnClickedVideo()
{
	if(!isOpen)
	{
		//AfxMessageBox("Not Videoing,Moron,Open the camera first");
		AfxMessageBox("���ȴ�����ͷ");
		return;
	}
	if(isLeftOpen)
	{
		CFileDialog saveFileDlg(FALSE,".avi","E:\\cali_result\\");
		saveFileDlg.m_ofn.lpstrTitle="��ѡ�񱣴����1��Ƶ��·�����ļ���";
		INT_PTR result = saveFileDlg.DoModal();  
		CString filePath ;
		if(result==IDOK)  filePath= saveFileDlg.GetPathName();  		
	    filePath.Replace("\\","\\\\");
		writer1=VideoWriter(filePath.GetBuffer(0),CV_FOURCC('X','V','I','D'),30,Size(640,480),true);
		SetTimer(3,30,NULL);//����camera1����Ƶ
	}
	if(isRightOpen)
	{
		CFileDialog saveFileDlg(FALSE,".avi","E:\\cali_result\\");
		saveFileDlg.m_ofn.lpstrTitle="��ѡ�񱣴����2��Ƶ��·�����ļ���";
		INT_PTR result = saveFileDlg.DoModal();  
		CString filePath ;
		if(result==IDOK)  filePath= saveFileDlg.GetPathName();  		
	    filePath.Replace("\\","\\\\");
		writer2=VideoWriter(filePath.GetBuffer(0),CV_FOURCC('X','V','I','D'),30,Size(640,480),true);
		SetTimer(4,30,NULL);//����camera2����Ƶ
	}

}


//void Ccamera_show_MFCDlg::saveVideo(int nIndex)
//{
//	//
//	//CFileDialog saveFileDlg(FALSE,"avi","test",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,NULL);  
//
//	//if(saveFileDlg.DoModal()==IDOK)  
//	//{  
//	//	strSaveFilePath=saveFileDlg.GetPathName();//�����·���ˡ�  
//	//}  	
//	string saveFilePath="E:\\test"+to_string((long long)(nIndex+1))+".avi";
//	writer=VideoWriter(saveFilePath,CV_FOURCC('X','V','I','D'),30,Size(640,480),true);
//	if(0==nIndex)
//	{
//		while(!frame1.empty())
//		{
//		writer<<frame1;
//		}
//	}
//	if(1==nIndex)
//	{
//		while(!frame2.empty())
//		{
//			writer<<frame2;
//		}
//	}
//}


void Ccamera_show_MFCDlg::OnBnClickedCalib()
{
	CCalibDlg caliDlg;
	caliDlg.DoModal();
}


void Ccamera_show_MFCDlg::OnBnClickedStereoCali()
{
	CStereoCali stereoCali;
	stereoCali.DoModal();
}


void Ccamera_show_MFCDlg::OnBnClickedShot()
{
	if(!isOpen)
	{
		//AfxMessageBox("Not Videoing ,Fool,Open the camera first");
		AfxMessageBox("���ȴ�����ͷ");
		return;
	}
	
	if(-1==nIndx)
	{
		AfxMessageBox("Please at lease select a camera");
		return;
	}
	if(0==nIndx)				//ѡ�����1�����
	{
	frame1.copyTo(m_buffer1);
	sndPlaySound ("shot.wav",SND_ASYNC);
	
	}
	if(1==nIndx)				//ѡ�����2�����
	{
	frame2.copyTo(m_buffer2);
	sndPlaySound ("shot.wav",SND_ASYNC);
	}
	if(2==nIndx)				//���������ѡ�У�������
	{
		frame1.copyTo(m_buffer1);
		frame2.copyTo(m_buffer2);
		sndPlaySound ("shot.wav",SND_ASYNC);
	}
	CShotDlg shotDlg;
	shotDlg.DoModal();
}




void Ccamera_show_MFCDlg::OnBnClickedShowdisparity()
{
	/*�������������Ǵ򿪵�,������������Ѿ�����궨�ˣ�����ʾ���ͼ,���򷵻�*/
	if(!isOpen)
	{
		AfxMessageBox("���ȴ����");
		return;
	}
	if(!isLeftOpen)
	{
		AfxMessageBox("��������ڹر�״̬���޷���ʾ���ͼ");
		return;
	}
	if(!isRightOpen)
	{
		AfxMessageBox("��������ڹر�״̬���޷���ʾ���ͼ");
		return;
	}
	if((!isStereoRectified)&&(!isFileRead))
	{
		AfxMessageBox("���Ƚ�������궨���߶�����������ļ�");
		return;
	}

	cvtColor(frame1, grayImageL, CV_BGR2GRAY);
	cvtColor(frame2, grayImageR, CV_BGR2GRAY);
	/*
	����remap֮�����������ͼ���Ѿ����沢���ж�׼��
	*/
	remap(grayImageL, rectifyImageL, mapLx, mapLy, INTER_LINEAR);
	remap(grayImageR, rectifyImageR, mapRx, mapRy, INTER_LINEAR);
	//int numberOfDisparities=0,SADWindowSize=0;������ΪApp�����Աߵ�ȫ�ֱ���
	//numberOfDisparities = numberOfDisparities > 0 ? numberOfDisparities : ((imageSize.width/8) + 15) & -16;
	/*����ƥ��*/
	//StereoBM bm;//��ΪPublic ����
	setBM(bm);
    bm(rectifyImageL, rectifyImageR, disp);
	disp.convertTo(disp8, CV_8U, 255 / ((numberOfDisparities * 16 + 16)*16.));//��������Ӳ���CV_16S��ʽ
	showImg(disp8,IDC_DISPARITY);
	SetTimer(5,30,NULL);
}


void Ccamera_show_MFCDlg::setBM(StereoBM & bm)
{
	bm.state->roi1 = validROIL;
    bm.state->roi2 = validROIR;
    bm.state->preFilterCap = 31;
    bm.state->SADWindowSize = SADWindowSize > 0 ? SADWindowSize : 9;
    bm.state->minDisparity = 0;
    bm.state->numberOfDisparities = 16;
    bm.state->textureThreshold = 10;
    bm.state->uniquenessRatio = 15;
    bm.state->speckleWindowSize = 100;
    bm.state->speckleRange = 32;
    bm.state->disp12MaxDiff = 1;
}


void Ccamera_show_MFCDlg::OnBnClickedReadfile()
{
	CFileDialog readFileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"�����ļ�(yml)|*.yml||");
	CString strPath;
	if(IDOK==readFileDlg.DoModal())
	{
		strPath=readFileDlg.GetPathName();	
	}
	strPath.Replace("\\","\\\\");
	FileStorage fs(strPath.GetBuffer(),FileStorage::READ);
	if(!fs.isOpened())  AfxMessageBox("���ļ�ʧ��");
		fs["cameraMatrixL"]>>cameraMatrixL;
		fs["distCoeffL"]>>distCoeffL;
		fs["cameraMatrixR"]>>cameraMatrixR;
		fs["distCoeffR"]>>distCoeffR;
		fs["R"]>>R;
		fs["T"]>>T;
		fs["E"]>>E;
		fs["F"]>>F;
		fs["Rl"]>>Rl;
		fs["Rr"]>>Rr;
		fs["Pl"]>>Pl;
		fs["Pr"]>>Pr;
		fs["Q"]>>Q;
		fs["mapLx"]>>mapLx;
		fs["mapLy"]>>mapLy;
		fs["mapRx"]>>mapRx;
		fs["mapRy"]>>mapRy;
	fs.release();

	isFileRead=true;
}
