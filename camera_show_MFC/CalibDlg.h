#pragma once

//#include <opencv2/opencv.hpp>
//// CCalibDlg 对话框
//cv::Mat frame1,frame2;
class CCalibDlg : public CDialog
{
	DECLARE_DYNAMIC(CCalibDlg)

public:
	CCalibDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalibDlg();


	//CString m_cstrSavePath;//获得标定照片的路径，没有放到App类的全局变量，因为可能只有这里用到
	


// 对话框数据
	enum { IDD = IDD_CALIBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartcali();
	virtual BOOL OnInitDialog();
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	int nCornerNumX;	//X方向角点个数
	int nCornerNumY;
	int nImageCount;			//照片张数
	CString strBasename; //照片的basename
	void cornerDetection(CString& picPath,int imageCount,vector<vector<Point2f>>&image_points_seq);//寻找角点函数，自定义
	double d_Square_Size;
	void getPicPath(CString& m_cstrSavePath);
	void getRealCoor(vector<vector<Point3f>> &object_points,int image_count,Size board_size,Size square_size);
	afx_msg void OnBnClickedSaveresult();
	afx_msg void OnBnClickedShowrectify();
	void showImage(Mat src, UINT ID);
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOpenfile();
	CString strPath;//获得标定照片的路径，没有放到App对象的全局变量，因为可能只有这里用到
	void setVoidPath(CString & strPath);//初始化strPath，使其为空，用来判断有没有设置图片路径
};
