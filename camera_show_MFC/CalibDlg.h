#pragma once

//#include <opencv2/opencv.hpp>
//// CCalibDlg �Ի���
//cv::Mat frame1,frame2;
class CCalibDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalibDlg)

public:
	CCalibDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCalibDlg();

// �Ի�������
	enum { IDD = IDD_CALIBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
