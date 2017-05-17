// StereoCali.cpp : 实现文件
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "StereoCali.h"
#include "afxdialogex.h"


// CStereoCali 对话框

IMPLEMENT_DYNAMIC(CStereoCali, CDialog)

CStereoCali::CStereoCali(CWnd* pParent /*=NULL*/)
	: CDialog(CStereoCali::IDD, pParent)
{

}

CStereoCali::~CStereoCali()
{
}

void CStereoCali::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStereoCali, CDialog)
END_MESSAGE_MAP()


// CStereoCali 消息处理程序
