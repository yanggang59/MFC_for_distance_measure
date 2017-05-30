
// camera_show_MFCDlg.cpp : 实现文件
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
#include "CvvImage.h"   //""表示在当前工程文件夹下寻找该文件，没有则在include的文件夹下寻找
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


using namespace cv;
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//我建立的全局变量
//全局变量


//Mat frame1,frame2;
// Ccamera_show_MFCDlg 对话框
VideoCapture cap1,cap2;   //用来打开相机的
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


// Ccamera_show_MFCDlg 消息处理程序

BOOL Ccamera_show_MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//CRect rect;
	//GetDlgItem(IDC_CAMERA1)->GetWindowRect(&rect);           //IDC_WAVE_DRAW为Picture Control的ID  
	//ScreenToClient(&rect);  
	//GetDlgItem(IDC_CAMERA1)->MoveWindow(rect.left, rect.top, 640, 480, true);    //固定Picture Control控件的大小

	CComboBox* pComb=(CComboBox *)GetDlgItem(IDC_CAMERAINDEX);
	pComb->AddString("1");
	pComb->AddString("2");
	pComb->AddString("Both");
	pComb->SetCurSel(2);			//默认设置为Both模式，即两个相机都选中
	nIndx=pComb->GetCurSel();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Ccamera_show_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Ccamera_show_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Ccamera_show_MFCDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void Ccamera_show_MFCDlg::OnBnClickedOpen()
{
	CComboBox* pComb=(CComboBox *)GetDlgItem(IDC_CAMERAINDEX);
	nIndx=pComb->GetCurSel();
	if(-1==nIndx)		//如果没有选中
	{
		//AfxMessageBox("You haven't chosen any camera");
		AfxMessageBox("请先选择相机！！！");
		return;
	}

	if(0==nIndx)			//选中1号相机
	{
	cap1.open(0);
	if(!cap1.isOpened())
	{
		AfxMessageBox("Error：无法打开相机1，请检查相机1是否存在");
		return;
	}
	isOpen=true;
	isLeftOpen=true;
	cap1>>frame1;
	showImg(frame1,IDC_CAMERA1);
	SetTimer(1,30,NULL);
	}

	if(1==nIndx)				 //选中2号相机
	{
	cap2.open(1);
	if(!cap2.isOpened())
	{
		AfxMessageBox("Error:无法打开相机2，请检查相机2是否存在");
		return;
	}
	isOpen=true;		//相机处于打开状态
	isRightOpen=true;//右相机处于打开状态
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
		AfxMessageBox("Error：无法打开相机1,请检查相机1是否存在");
		return;
	}
		isLeftOpen=true;			//左相机处于打开状态
		cap1>>frame1;
		showImg(frame1,IDC_CAMERA1);
		SetTimer(1,30,NULL);
		isOpen=true;				//有一个相机处于打开状态
		if(!cap2.isOpened())
	{
		AfxMessageBox("Error:无法打开相机2，请检查相机2是否存在");
		return;
	}
	isRightOpen=true;
	}
	cap2>>frame2;
	showImg(frame2,IDC_CAMERA2);
	SetTimer(2,30,NULL);
}


 void Ccamera_show_MFCDlg::showImg(Mat src, UINT ID)//自定义函数，将Mat显示在控件上
{
	CDC *pDC = GetDlgItem(ID)->GetDC();//根据ID获得窗口指针再获取与该窗口关联的上下文指针
	HDC hdc= pDC->GetSafeHdc();           // 获取设备上下文句柄
	CRect rect;
   GetDlgItem(ID)->GetClientRect(&rect); //获取box1客户区
   CvvImage cimg;
   IplImage img(src); // 定义IplImage变量img     
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
				disp.convertTo(disp8, CV_8U, 255 / ((numberOfDisparities * 16 + 16)*16.));//计算出的视差是CV_16S格式
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
	cap1.release();								//释放VideoCapture资源
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
		AfxMessageBox("摄像头已经处于关闭状态");
		return;
	}
}


void Ccamera_show_MFCDlg::OnBnClickedVideo()
{
	if(!isOpen)
	{
		//AfxMessageBox("Not Videoing,Moron,Open the camera first");
		AfxMessageBox("请先打开摄像头");
		return;
	}
	if(isLeftOpen)
	{
		CFileDialog saveFileDlg(FALSE,".avi","E:\\cali_result\\");
		saveFileDlg.m_ofn.lpstrTitle="请选择保存相机1视频的路径与文件名";
		INT_PTR result = saveFileDlg.DoModal();  
		CString filePath ;
		if(result==IDOK)  filePath= saveFileDlg.GetPathName();  		
	    filePath.Replace("\\","\\\\");
		writer1=VideoWriter(filePath.GetBuffer(0),CV_FOURCC('X','V','I','D'),30,Size(640,480),true);
		SetTimer(3,30,NULL);//保存camera1的视频
	}
	if(isRightOpen)
	{
		CFileDialog saveFileDlg(FALSE,".avi","E:\\cali_result\\");
		saveFileDlg.m_ofn.lpstrTitle="请选择保存相机2视频的路径与文件名";
		INT_PTR result = saveFileDlg.DoModal();  
		CString filePath ;
		if(result==IDOK)  filePath= saveFileDlg.GetPathName();  		
	    filePath.Replace("\\","\\\\");
		writer2=VideoWriter(filePath.GetBuffer(0),CV_FOURCC('X','V','I','D'),30,Size(640,480),true);
		SetTimer(4,30,NULL);//保存camera2的视频
	}

}


//void Ccamera_show_MFCDlg::saveVideo(int nIndex)
//{
//	//
//	//CFileDialog saveFileDlg(FALSE,"avi","test",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,NULL,NULL);  
//
//	//if(saveFileDlg.DoModal()==IDOK)  
//	//{  
//	//	strSaveFilePath=saveFileDlg.GetPathName();//保存的路径了。  
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
		AfxMessageBox("请先打开摄像头");
		return;
	}
	
	if(-1==nIndx)
	{
		AfxMessageBox("Please at lease select a camera");
		return;
	}
	if(0==nIndx)				//选择的是1号相机
	{
	frame1.copyTo(m_buffer1);
	sndPlaySound ("shot.wav",SND_ASYNC);
	
	}
	if(1==nIndx)				//选择的是2号相机
	{
	frame2.copyTo(m_buffer2);
	sndPlaySound ("shot.wav",SND_ASYNC);
	}
	if(2==nIndx)				//两个相机都选中，都拍照
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
	/*如果左右相机都是打开的,而且两个相机已经立体标定了，则显示深度图,否则返回*/
	if(!isOpen)
	{
		AfxMessageBox("请先打开相机");
		return;
	}
	if(!isLeftOpen)
	{
		AfxMessageBox("左相机处于关闭状态，无法显示深度图");
		return;
	}
	if(!isRightOpen)
	{
		AfxMessageBox("右相机处于关闭状态，无法显示深度图");
		return;
	}
	if((!isStereoRectified)&&(!isFileRead))
	{
		AfxMessageBox("请先进行立体标定或者读入相机参数文件");
		return;
	}

	cvtColor(frame1, grayImageL, CV_BGR2GRAY);
	cvtColor(frame2, grayImageR, CV_BGR2GRAY);
	/*
	经过remap之后，左右相机的图像已经共面并且行对准了
	*/
	remap(grayImageL, rectifyImageL, mapLx, mapLy, INTER_LINEAR);
	remap(grayImageR, rectifyImageR, mapRx, mapRy, INTER_LINEAR);
	//int numberOfDisparities=0,SADWindowSize=0;已设置为App对象旁边的全局变量
	//numberOfDisparities = numberOfDisparities > 0 ? numberOfDisparities : ((imageSize.width/8) + 15) & -16;
	/*立体匹配*/
	//StereoBM bm;//作为Public 变量
	setBM(bm);
    bm(rectifyImageL, rectifyImageR, disp);
	disp.convertTo(disp8, CV_8U, 255 / ((numberOfDisparities * 16 + 16)*16.));//计算出的视差是CV_16S格式
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
	CFileDialog readFileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"参数文件(yml)|*.yml||");
	CString strPath;
	if(IDOK==readFileDlg.DoModal())
	{
		strPath=readFileDlg.GetPathName();	
	}
	strPath.Replace("\\","\\\\");
	FileStorage fs(strPath.GetBuffer(),FileStorage::READ);
	if(!fs.isOpened())  AfxMessageBox("打开文件失败");
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
