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


	//CString m_cstrSavePath;//��ñ궨��Ƭ��·����û�зŵ�App���ȫ�ֱ�������Ϊ����ֻ�������õ�
	


// �Ի�������
	enum { IDD = IDD_CALIBDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartcali();
	virtual BOOL OnInitDialog();
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	int nCornerNumX;	//X����ǵ����
	int nCornerNumY;
	int nImageCount;			//��Ƭ����
	CString strBasename; //��Ƭ��basename
	void cornerDetection(CString& picPath,int imageCount,vector<vector<Point2f>>&image_points_seq);//Ѱ�ҽǵ㺯�����Զ���
	double d_Square_Size;
	void getPicPath(CString& m_cstrSavePath);
	void getRealCoor(vector<vector<Point3f>> &object_points,int image_count,Size board_size,Size square_size);
	afx_msg void OnBnClickedSaveresult();
	afx_msg void OnBnClickedShowrectify();
	void showImage(Mat src, UINT ID);
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOpenfile();
	CString strPath;//��ñ궨��Ƭ��·����û�зŵ�App�����ȫ�ֱ�������Ϊ����ֻ�������õ�
	void setVoidPath(CString & strPath);//��ʼ��strPath��ʹ��Ϊ�գ������ж���û������ͼƬ·��
};
