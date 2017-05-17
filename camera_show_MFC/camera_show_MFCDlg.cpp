
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
VideoWriter writer1("E:\\test1.avi",CV_FOURCC('X','V','I','D'),30,Size(640,480),true);
VideoWriter writer2("E:\\test2.avi",CV_FOURCC('X','V','I','D'),30,Size(640,480),true);




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
	
	//cap.open(0);//�򿪵�һ������ͷ
	//cap>>frame;
	//if(frame.empty())
	//	return;
	//showImg(frame,IDC_CAMERA1);

	//SetTimer(1,30,NULL);
	CComboBox *pComb=(CComboBox *)GetDlgItem(IDC_CAMERAINDEX);
	nIndx=pComb->GetCurSel();      //ѡ�е������ע��nIndex��ǰ�涨��Ϊȫ�ֱ�����
	if(-1==nIndx)		//���û��ѡ��
	{
		AfxMessageBox("You haven't chosen any camera");
		return;
	}

	if(0==nIndx)			//ѡ��1�����
	{
	cap1.open(nIndx);
	cap1>>frame1;
	if(frame1.empty())
	{
		return;
	}
	showImg(frame1,IDC_CAMERA1);
	SetTimer(1,30,NULL);
	}

	if(1==nIndx)				 //ѡ��2�����
	{
	cap2.open(nIndx);
	cap2>>frame2;
	if(frame2.empty())
	{
		return;
	}
	showImg(frame2,IDC_CAMERA2);
	SetTimer(2,30,NULL);
	}


	if(nIndx==2)
	{
	cap1.open(0);
	cap1>>frame1;
	if(frame1.empty())
	{
		return;
	}
	showImg(frame1,IDC_CAMERA1);
	SetTimer(1,30,NULL);
	}
	cap2.open(1);
	cap2>>frame2;
	if(frame2.empty())
	{
		return;
	}
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
				//string saveFilePath="E:\\test1.avi";
				writer1<<frame1;
				break;
		   }
		case 4:
			{
				//string saveFilePath="E:\\test2.avi";
				writer2<<frame2;
				break;
			}
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


void Ccamera_show_MFCDlg::OnBnClickedClose()
{
	Mat gray(Size(500,400),CV_32FC3, Scalar(120, 120,120));
	cap1.release();								//�ͷ�VideoCapture��Դ
	showImg(gray,IDC_CAMERA1);     
	KillTimer(1);

	cap2.release();
	showImg(gray,IDC_CAMERA2);
	KillTimer(2);

	KillTimer(3);
	KillTimer(4);
}


void Ccamera_show_MFCDlg::OnBnClickedVideo()
{
	if(-1==nIndx)
		{
			AfxMessageBox("you haven't selected any camera");
		}
	if(0==nIndx)
	{
		SetTimer(3,30,NULL);//����camera1����Ƶ
	}
	if(1==nIndx)
	{
		SetTimer(4,30,NULL);//����camera2����Ƶ
	}
	if(2==nIndx)
	{
	SetTimer(3,30,NULL);//����camera1����Ƶ
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
	
	if(-1==nIndx)
	{
		AfxMessageBox("Please at lease select a camera");
	}
	if(0==nIndx)				//ѡ�����1�����
	{
	frame1.copyTo(m_buffer1);
	}
	if(1==nIndx)				//ѡ�����2�����
	{
	frame2.copyTo(m_buffer2);
	}
	if(2==nIndx)				//���������ѡ�У�������
	{
		frame1.copyTo(m_buffer1);
		frame2.copyTo(m_buffer2);
	}
	CShotDlg shotDlg;
	shotDlg.DoModal();
}


