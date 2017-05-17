// ALIB_DLG.cpp : 实现文件
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "ALIB_DLG.h"
#include "afxdialogex.h"


// CALIB_DLG 对话框

IMPLEMENT_DYNAMIC(CALIB_DLG, CDialog)

CALIB_DLG::CALIB_DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CALIB_DLG::IDD, pParent)
{

}

CALIB_DLG::~CALIB_DLG()
{
}

void CALIB_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CALIB_DLG, CDialog)
END_MESSAGE_MAP()


// CALIB_DLG 消息处理程序
