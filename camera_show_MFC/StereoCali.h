#pragma once


// CStereoCali �Ի���

class CStereoCali : public CDialog
{
	DECLARE_DYNAMIC(CStereoCali)

public:
	CStereoCali(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStereoCali();

// �Ի�������
	enum { IDD = IDD_STEREO_CALI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
