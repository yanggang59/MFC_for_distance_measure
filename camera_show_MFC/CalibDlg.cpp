// CalibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "CalibDlg.h"
#include "afxdialogex.h"


// CCalibDlg 对话框

IMPLEMENT_DYNAMIC(CCalibDlg, CDialog)

CCalibDlg::CCalibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibDlg::IDD, pParent)
{

}

CCalibDlg::~CCalibDlg()
{
}

void CCalibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCalibDlg, CDialog)
END_MESSAGE_MAP()


// CCalibDlg 消息处理程序
