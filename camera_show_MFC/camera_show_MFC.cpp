
// camera_show_MFC.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "camera_show_MFCDlg.h"
#include "CvvImage.h"
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
