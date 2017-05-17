// ShotDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "ShotDlg.h"
#include "afxdialogex.h"
#include "globalVar.h"
#include <string>
#include  "resource.h"

using namespace std;
// CShotDlg �Ի���
//ȫ�ֱ���
int imageCount;//������¼���յ�����


IMPLEMENT_DYNAMIC(CShotDlg, CDialog)

CShotDlg::CShotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShotDlg::IDD, pParent)
{

}

CShotDlg::~CShotDlg()
{
}

void CShotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShotDlg, CDialog)
	ON_BN_CLICKED(IDC_SAVEIMAGE, &CShotDlg::OnBnClickedSaveimage)
	ON_BN_CLICKED(IDC_SHOWIMG, &CShotDlg::OnBnClickedShowimg)
END_MESSAGE_MAP()


// CShotDlg ��Ϣ�������

 
void CShotDlg::OnBnClickedSaveimage()
{
 char szDir[MAX_PATH]; 
 BROWSEINFO bi; 
 ITEMIDLIST *pidl;
 bi.hwndOwner = this->m_hWnd; 
 bi.pidlRoot = NULL; 
 bi.pszDisplayName = szDir; 
 bi.lpszTitle ="��ѡ��Ŀ¼"; 
 bi.ulFlags =  BIF_STATUSTEXT; 
 //bi.lpfn = NULL;
 bi.lpfn=BrowseCallbackProc;		//�ص�����
 bi.lParam=NULL;
 bi.iImage = 0;

 pidl = SHBrowseForFolder(&bi); 
 if(pidl == NULL) 
  return;
 CString m_cstrSavePath;
 if(SHGetPathFromIDList(pidl, szDir))
 {
  m_cstrSavePath.Format("%s", szDir);
}
	m_cstrSavePath.Replace("\\","\\\\"); //ת���ַ�/��ת��
    char buff[200]; 
	sprintf(buff,"%02d", imageCount);
	string m_int2str(buff);		
	string savePath =m_cstrSavePath.GetBuffer(0);		//��CStringת��Ϊstring,����ĵ�ַ��Ҳ�����ļ���
	if(-1==nIndx)
	{
	AfxMessageBox("No Picture Taken");
	}
	if(0==nIndx)    //
	{
	string saveFileName=savePath+"\\left"+m_int2str+".jpg";
	imwrite(saveFileName,m_buffer1);
	}
	if(1==nIndx)
	{
		string saveFileName=savePath+"\\right"+m_int2str+".jpg";
		imwrite(saveFileName,m_buffer2);
	}
	if(2==nIndx) //�����������ѡ��
	{
	string saveFileName=savePath+"\\left"+m_int2str+".jpg";
	imwrite(saveFileName,m_buffer1);

	saveFileName=savePath+"\\right"+m_int2str+".jpg";
	imwrite(saveFileName,m_buffer2);

	}



	imageCount++;//��¼���յ�����
}


BOOL CShotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(0==nIndx)
	{
		showImage(m_buffer1,IDC_CAMERA1);
	}
	if(1==nIndx)
	{
		showImage(m_buffer2,IDC_CAMERA2);
	}
	if(2==nIndx)
	{
		showImage(m_buffer1,IDC_CAMERA1);
		showImage(m_buffer2,IDC_CAMERA2);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CShotDlg::showImage(Mat src, UINT ID)
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


void CShotDlg::OnBnClickedShowimg()
{
	if(0==nIndx)
	{
		showImage(m_buffer1,IDC_CAMERA1);
	}
	if(1==nIndx)
	{
		showImage(m_buffer2,IDC_CAMERA2);
	}
	if(2==nIndx)
	{
		showImage(m_buffer1,IDC_CAMERA1);
		showImage(m_buffer2,IDC_CAMERA2);
	}
}


int CALLBACK CShotDlg::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM  lpData)
{
	
	 char szPath[] = "E:\\pictures";
    switch(uMsg)
    {
         
    case BFFM_INITIALIZED:    //��ʼ����ϢBFFM_INITIALIZED
        ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szPath);   //����Ĭ�ϴ�·�� break;
    case BFFM_SELCHANGED:    //ѡ��·���仯��BFFM_SELCHANGED
        {
            char curr[MAX_PATH];   
            SHGetPathFromIDList((LPCITEMIDLIST)lParam,curr);   
            ::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)&curr[0]);  
        }
        break;
    default:
        break;
    } 
    return 0;
}
