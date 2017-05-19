
// camera_show_MFC.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "camera_show_MFCDlg.h"
#include "CvvImage.h"
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ccamera_show_MFCApp

BEGIN_MESSAGE_MAP(Ccamera_show_MFCApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Ccamera_show_MFCApp 构造

Ccamera_show_MFCApp::Ccamera_show_MFCApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Ccamera_show_MFCApp 对象
//全局变量
Ccamera_show_MFCApp theApp;
Mat frame1,frame2;			//左右相机的图像
int nIndx;							//相机索引
Mat m_buffer1,m_buffer2;//拍照时用来存储临时照片的
bool isOpen=false; //判断相机是否打开
Size imageSize;//图片的大小，对于我的罗技相机来说是640*480
ofstream fout;   //用来保存单个摄像头标定文件的ofstream对象

/*相机的内外参数*/
Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* 摄像机内参数矩阵 */  
Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */  
vector<Mat> tvecsMat;  /* 每幅图像的旋转向量 */  
vector<Mat> rvecsMat; /* 每幅图像的平移向量 */ 



/*双目中相机的内参*/
Mat cameraMatrixL;
Mat distCoeffL;

Mat cameraMatrixR;
Mat distCoeffR;

 vector<Mat> tvecsMatL;  /* 每幅左相机图像的旋转向量 */  
 vector<Mat> rvecsMatL; /* 每幅左相机图像的平移向量 */ 
 vector<Mat> tvecsMatR;  /* 每幅右相机图像的旋转向量 */  
 vector<Mat> rvecsMatR; /* 每幅右相机图像的旋转向量 */ 


 Mat R, T, E, F;    //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵

ofstream fout_stereo; //用来保存两个摄像头标定文件的ofstream对象

// Ccamera_show_MFCApp 初始化



BOOL Ccamera_show_MFCApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	Ccamera_show_MFCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



//void Ccamera_show_MFCApp::showImg(Mat src, HWND hWnd,UINT ID)
//{
//	CDC *pDC = GetDlgItem(hWnd,ID)->GetDC();//根据ID获得窗口指针再获取与该窗口关联的上下文指针
//	HDC hdc= pDC->GetSafeHdc();           // 获取设备上下文句柄
//	CRect rect;
//   GetDlgItem(hWnd,ID)->GetClientRect(&rect); //获取box1客户区
//   CvvImage cimg;
//   IplImage img(src); // 定义IplImage变量img     
//   cimg.CopyOf(&img,img.nChannels);
//   cimg.DrawToHDC(hdc,&rect);
//   ReleaseDC( pDC );
//   cimg.Destroy();
//}
