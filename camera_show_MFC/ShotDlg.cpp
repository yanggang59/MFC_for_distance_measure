// ShotDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "ShotDlg.h"
#include "afxdialogex.h"
#include "globalVar.h"
#include <string>
#include  "resource.h"

using namespace std;
// CShotDlg 对话框
//全局变量
int imageCount;//用来记录拍照的张数


IMPLEMENT_DYNAMIC(CShotDlg, CDialog)

CShotDlg::CShotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShotDlg::IDD, pParent)
	, str_Basename1(_T(""))
	, str_Basename2(_T(""))
{

}

CShotDlg::~CShotDlg()
{
}

void CShotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BASENAME1, str_Basename1);
	DDX_Text(pDX, IDC_BASENAME2, str_Basename2);
}


BEGIN_MESSAGE_MAP(CShotDlg, CDialog)
	ON_BN_CLICKED(IDC_SAVEIMAGE, &CShotDlg::OnBnClickedSaveimage)
	ON_BN_CLICKED(IDC_SHOWIMG, &CShotDlg::OnBnClickedShowimg)
END_MESSAGE_MAP()


// CShotDlg 消息处理程序

 
void CShotDlg::OnBnClickedSaveimage()
{
	UpdateData(TRUE);
	//if(str_Basename1==""&&str_Basename2=="")
	if(!m_buffer1.empty())
	{
		if(str_Basename1=="")
		{
			AfxMessageBox("Please Input basename for Image 1");
			return;
		}
	}
	if(!m_buffer1.empty())
	{
		if(str_Basename2=="")
		{
			AfxMessageBox("Please Input basename for Image 2");
			return;
		}
	}

 char szDir[MAX_PATH]; 
 BROWSEINFO bi; 
 ITEMIDLIST *pidl;
 bi.hwndOwner = this->m_hWnd; 
 bi.pidlRoot = NULL; 
 bi.pszDisplayName = szDir; 
 bi.lpszTitle ="请选择目录"; 
 bi.ulFlags =  BIF_STATUSTEXT; 
 //bi.lpfn = NULL;
 bi.lpfn=BrowseCallbackProc;		//回调函数
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
	m_cstrSavePath.Replace("\\","\\\\"); //转义字符/的转换
    char buff[200]; 
	sprintf(buff,"%02d", imageCount);
	string m_int2str(buff);		
	string savePath =m_cstrSavePath.GetBuffer(0);		//将CString转化为string,保存的地址，也就是文件夹

	if(m_buffer1.empty()&&m_buffer2.empty())
	{
	AfxMessageBox("No Picture Taken");
	return;
	}
	if(!m_buffer1.empty())    //
	{
		string saveFileName=savePath+"\\"+str_Basename1.GetBuffer(0)+m_int2str+".jpg";
	imwrite(saveFileName,m_buffer1);
	}
	if(!m_buffer2.empty())
	{
		string saveFileName=savePath+"\\"+str_Basename2.GetBuffer(0)+m_int2str+".jpg";
		imwrite(saveFileName,m_buffer2);
	}
	//if(2==nIndx) //左右相机都被选中
	//{
	//string saveFileName=savePath+"\\left"+m_int2str+".jpg";
	//imwrite(saveFileName,m_buffer1);

	//saveFileName=savePath+"\\right"+m_int2str+".jpg";
	//imwrite(saveFileName,m_buffer2);
	imageCount++;//记录拍照的张数
}


BOOL CShotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	/*if(!m_buffer1.empty())
	{
		showImage(m_buffer1,IDC_CAMERA1);
	}
	if(!m_buffer2.empty())
	{
		showImage(m_buffer2,IDC_CAMERA2);
	}*/
	/*if(2==nIndx)
	{
		showImage(m_buffer1,IDC_CAMERA1);
		showImage(m_buffer2,IDC_CAMERA2);
	}*/
	GetDlgItem(IDC_BASENAME1)->SetWindowText("left");//将basename初始化为“”（空）
	GetDlgItem(IDC_BASENAME2)->SetWindowText("right");//将basename初始化为“”（空）
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CShotDlg::showImage(Mat src, UINT ID)
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


void CShotDlg::OnBnClickedShowimg()
{
	if(!m_buffer1.empty())
	{
		showImage(m_buffer1,IDC_CAMERA1);
	}

	if(!m_buffer2.empty())
	{
		showImage(m_buffer2,IDC_CAMERA2);
	}

	if(m_buffer1.empty()&&m_buffer2.empty())
	{
		AfxMessageBox("No photo was taken");
	}
}


int CALLBACK CShotDlg::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM  lpData)
{
	
	char szPath[] = "E:\\pictures";
    switch(uMsg)
    {
         
    case BFFM_INITIALIZED:    //初始化消息BFFM_INITIALIZED
        ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szPath);   //传递默认打开路径 break;
    case BFFM_SELCHANGED:    //选择路径变化，BFFM_SELCHANGED
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
