#pragma once

#include "resource.h"
// CALIB_DLG 对话框

class CALIB_DLG : public CDialog
{
	DECLARE_DYNAMIC(CALIB_DLG)

public:
	CALIB_DLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CALIB_DLG();

// 对话框数据
	enum { IDD = IDD_CALIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
