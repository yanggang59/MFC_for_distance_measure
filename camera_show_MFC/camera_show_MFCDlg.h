

#pragma once

#include "camera_show_MFC.h"
#include <opencv2/opencv.hpp>
using namespace cv;


//�ҽ�����ȫ�ֱ���

 class Ccamera_show_MFCDlg: public CDialogEx
{
// ����
public:
	Ccamera_show_MFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

//

//�Ի�������
	enum { IDD = IDD_CAMERA_SHOW_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOpen();
	void showImg(Mat src, UINT ID);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedVideo();
	//void saveVideo(int nIndex);
	afx_msg void OnBnClickedCalib();
	afx_msg void OnBnClickedStereoCali();
	afx_msg void OnBnClickedShot();
	
	afx_msg void OnBnClickedShowdisparity();
	void setBM(StereoBM & bm);
	StereoBM bm;//StereoBM������������ƥ��
	afx_msg void OnBnClickedReadfile();
 };
