#pragma once


// CShotDlg �Ի���

class CShotDlg : public CDialog
{
	DECLARE_DYNAMIC(CShotDlg)

public:
	CShotDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShotDlg();

// �Ի�������
	enum { IDD = IDD_SHOT };
	static string m_filePath;		//��̬��Ա���������������ʼ��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
