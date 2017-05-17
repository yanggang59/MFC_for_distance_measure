#pragma once


// CStereoCali 对话框

class CStereoCali : public CDialog
{
	DECLARE_DYNAMIC(CStereoCali)

public:
	CStereoCali(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStereoCali();

// 对话框数据
	enum { IDD = IDD_STEREO_CALI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
