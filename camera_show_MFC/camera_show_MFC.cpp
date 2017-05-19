
// camera_show_MFC.cpp : ����Ӧ�ó��������Ϊ��
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


// Ccamera_show_MFCApp ����

Ccamera_show_MFCApp::Ccamera_show_MFCApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Ccamera_show_MFCApp ����
//ȫ�ֱ���
Ccamera_show_MFCApp theApp;
Mat frame1,frame2;			//���������ͼ��
int nIndx;							//�������
Mat m_buffer1,m_buffer2;//����ʱ�����洢��ʱ��Ƭ��
bool isOpen=false; //�ж�����Ƿ��
Size imageSize;//ͼƬ�Ĵ�С�������ҵ��޼������˵��640*480
ofstream fout;   //�������浥������ͷ�궨�ļ���ofstream����

/*������������*/
Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* ������ڲ������� */  
Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* �������5������ϵ����k1,k2,p1,p2,k3 */  
vector<Mat> tvecsMat;  /* ÿ��ͼ�����ת���� */  
vector<Mat> rvecsMat; /* ÿ��ͼ���ƽ������ */ 



/*˫Ŀ��������ڲ�*/
Mat cameraMatrixL;
Mat distCoeffL;

Mat cameraMatrixR;
Mat distCoeffR;

 vector<Mat> tvecsMatL;  /* ÿ�������ͼ�����ת���� */  
 vector<Mat> rvecsMatL; /* ÿ�������ͼ���ƽ������ */ 
 vector<Mat> tvecsMatR;  /* ÿ�������ͼ�����ת���� */  
 vector<Mat> rvecsMatR; /* ÿ�������ͼ�����ת���� */ 


 Mat R, T, E, F;    //R ��תʸ�� Tƽ��ʸ�� E�������� F��������

ofstream fout_stereo; //����������������ͷ�궨�ļ���ofstream����

// Ccamera_show_MFCApp ��ʼ��



BOOL Ccamera_show_MFCApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	Ccamera_show_MFCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



//void Ccamera_show_MFCApp::showImg(Mat src, HWND hWnd,UINT ID)
//{
//	CDC *pDC = GetDlgItem(hWnd,ID)->GetDC();//����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��
//	HDC hdc= pDC->GetSafeHdc();           // ��ȡ�豸�����ľ��
//	CRect rect;
//   GetDlgItem(hWnd,ID)->GetClientRect(&rect); //��ȡbox1�ͻ���
//   CvvImage cimg;
//   IplImage img(src); // ����IplImage����img     
//   cimg.CopyOf(&img,img.nChannels);
//   cimg.DrawToHDC(hdc,&rect);
//   ReleaseDC( pDC );
//   cimg.Destroy();
//}
