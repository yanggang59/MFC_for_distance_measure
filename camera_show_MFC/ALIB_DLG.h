#pragma once

#include "resource.h"
// CALIB_DLG �Ի���

class CALIB_DLG : public CDialog
{
	DECLARE_DYNAMIC(CALIB_DLG)

public:
	CALIB_DLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CALIB_DLG();

// �Ի�������
	enum { IDD = IDD_CALIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
