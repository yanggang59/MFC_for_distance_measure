#pragma once


// CStereoCali �Ի���

class CStereoCali : public CDialog
{
	DECLARE_DYNAMIC(CStereoCali)

public:
	CStereoCali(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStereoCali();


	CString strPath;//ָ��˫Ŀ�궨��·��

// �Ի�������
	enum { IDD = IDD_STEREO_CALI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartcali();
	afx_msg void OnBnClickedSavefile();
	int nCornerNumX;			//
	int nCornerNumY;			//
	double d_Square_Size;
	int nImageCount;
	CString strLBasename;
	CString strRBasename;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOpenfile();
	void getPath(CString & strPath);
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	//void setPath();
	void setVoidPath(CString & strPath);
	void cornerDetection(CString& picPath,CString strBasename,int imageCount,vector<vector<Point2f>> & image_points_seq);
	void getRealCoor(vector<vector<Point3f>> &object_points,int image_count,Size board_size,Size square_size);
	afx_msg void OnBnClickedRectify();
};
