#pragma once


// CShotDlg 对话框

class CShotDlg : public CDialog
{
	DECLARE_DYNAMIC(CShotDlg)

public:
	CShotDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShotDlg();

// 对话框数据
	enum { IDD = IDD_SHOT };
	static string m_filePath;		//静态成员变量必须在类外初始化

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSaveimage();
	virtual BOOL OnInitDialog();
	void showImage(Mat src, UINT ID);
	afx_msg void OnBnClickedShowimg();
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM  lpData);
	CString str_Basename1;
	CString str_Basename2;
};
