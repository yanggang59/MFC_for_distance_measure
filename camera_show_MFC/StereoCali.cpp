// StereoCali.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "StereoCali.h"
#include "afxdialogex.h"
#include "resource.h"
#include <iostream>
#include <fstream>  

using namespace std;
// CStereoCali �Ի���

IMPLEMENT_DYNAMIC(CStereoCali, CDialog)

CStereoCali::CStereoCali(CWnd* pParent /*=NULL*/)
	: CDialog(CStereoCali::IDD, pParent)
	, nCornerNumX(0)
	, nCornerNumY(0)
	, d_Square_Size(0)
	, nImageCount(0)
	, strLBasename(_T(""))
	, strRBasename(_T(""))
{

}

CStereoCali::~CStereoCali()
{
}

void CStereoCali::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUM_X, nCornerNumX);
	DDX_Text(pDX, IDC_NUM_Y, nCornerNumY);
	DDX_Text(pDX, IDC_SQUARESIZE, d_Square_Size);
	DDX_Text(pDX, IDC_IMGCOUNT, nImageCount);
	DDX_Text(pDX, IDC_PIC_NAME_L, strLBasename);
	DDX_Text(pDX, IDC_PIC_NAME_R, strRBasename);
}


BEGIN_MESSAGE_MAP(CStereoCali, CDialog)
	ON_BN_CLICKED(IDC_STARTCALI, &CStereoCali::OnBnClickedStartcali)
	ON_BN_CLICKED(IDC_SAVEFILE, &CStereoCali::OnBnClickedSavefile)
	ON_BN_CLICKED(IDC_OPENFILE, &CStereoCali::OnBnClickedOpenfile)
END_MESSAGE_MAP()


// CStereoCali ��Ϣ�������


void CStereoCali::OnBnClickedStartcali()
{
	UpdateData(TRUE);

	if(nCornerNumX==0)
	{
		//AfxMessageBox("Please input num X");
		AfxMessageBox("��������X����ǵ����");
		return;
	}

	if(nCornerNumY==0)
	{
		//AfxMessageBox("Please input num Y");
		AfxMessageBox("��������Y����ǵ����");
		return;
	}
	if(d_Square_Size==0.0)
	{
		//AfxMessageBox("Please input square size");
		AfxMessageBox("�����������̸�߳�");
		return;
	}
	if(nImageCount==0)
	{
		//AfxMessageBox("Please input number of pictures");
		AfxMessageBox("����������Ƭ����");
		return;
	}
	if(strPath=="")
	{
		AfxMessageBox("����ָ��ͼƬ����·��");
		return;
	}


	/*��ʼ�ǵ���*/
	GetDlgItem(IDC_STATUSBAR)->SetWindowText("���ڼ��ǵ�");
	vector<vector<Point2f>> allCornersL,allCornersR;
	cornerDetection(strPath,strLBasename,nImageCount,allCornersL);
	cornerDetection(strPath,strRBasename,nImageCount,allCornersR);


	/*���¿�ʼ�궨*/
	GetDlgItem(IDC_STATUSBAR)->SetWindowText("���ڵ�Ŀ�궨");
	Size square_size = Size(d_Square_Size,d_Square_Size);  /* ʵ�ʲ����õ��ı궨����ÿ�����̸�Ĵ�С����Ҫ����λmm */  
    vector<vector<Point3f>> object_points; /* ����궨���Ͻǵ����ά���� */  
	Size board_size =Size(nCornerNumX,nCornerNumY); //����������ı궨����ڽǵ�ĺ�����������
	getRealCoor(object_points,nImageCount,board_size,square_size);//������̸����ʵ��������
	vector<int> point_counts;  // ÿ��ͼ���нǵ������  
	/* ��ʼ��ÿ��ͼ���еĽǵ��������ٶ�ÿ��ͼ���ж����Կ��������ı궨�� */  
    for (int i=0;i<nImageCount;i++)  
    {  
        point_counts.push_back(board_size.width*board_size.height);  
    }     

	calibrateCamera(object_points,allCornersL,imageSize,cameraMatrixL,distCoeffL,rvecsMatL,tvecsMatL,0); 
	calibrateCamera(object_points,allCornersR,imageSize,cameraMatrixR,distCoeffR,rvecsMatR,tvecsMatR,0); 


	GetDlgItem(IDC_STATUSBAR)->SetWindowText("���ڽ�������궨");
	//Mat R, T, E, F;                                         //R ��תʸ�� Tƽ��ʸ�� E�������� F�������� ����Ϊȫ�ֱ���
	double rms = stereoCalibrate(object_points, allCornersL, allCornersR,  
        cameraMatrixL, distCoeffL,  
        cameraMatrixR, distCoeffR,  
        imageSize, R, T, E, F,  
        TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));  

	GetDlgItem(IDC_STATUSBAR)->SetWindowText("����궨���");
}


void CStereoCali::OnBnClickedSavefile()
	{
		GetDlgItem(IDC_STATUSBAR)->SetWindowText("�����ļ�");

		CFileDialog saveFileDlg(FALSE,NULL,"E:\\cali_result\\");
		saveFileDlg.m_ofn.lpstrTitle="��ѡ�񱣴����ݵ�·�����ļ���";
		INT_PTR result = saveFileDlg.DoModal();  
		CString filePath ;
		if(result==IDOK)  filePath= saveFileDlg.GetPathName();  		
	    filePath.Replace("\\","\\\\");
		fout_stereo = ofstream(filePath.GetBuffer(0));  /* ����궨������ļ� */  

	   fout_stereo<<"--------------------������Ĳ���-------------------"<<endl;     
	   fout_stereo<<"����ڲ�������\n"<<endl;     
	   fout_stereo<<cameraMatrixL<<endl<<endl;     
       fout_stereo<<"����ϵ����\n";     
       fout_stereo<<distCoeffL<<endl<<endl<<endl;  

		
	   fout_stereo<<"--------------------������Ĳ���-------------------"<<endl;     
	   fout_stereo<<"����ڲ�������\n"<<endl;     
	   fout_stereo<<cameraMatrixR<<endl<<endl;     
       fout_stereo<<"����ϵ����\n";     
       fout_stereo<<distCoeffR<<endl<<endl<<endl;  


	   fout_stereo<<"--------------------�������λ�˹�ϵ-------------------"<<endl;     
	   fout_stereo<<"��ת����"<<endl; 
	   fout_stereo<<R<<endl<<endl;
	   fout_stereo<<"ƽ��ʸ��"<<endl; 
	   fout_stereo<<T<<endl<<endl;
	   fout_stereo<<"���ʾ���"<<endl; 
	   fout_stereo<<E<<endl<<endl;
	   fout_stereo<<"��������"<<endl; 
	   fout_stereo<< F<<endl<<endl;


	   GetDlgItem(IDC_STATUSBAR)->SetWindowText("�����ļ��ɹ�");
}


BOOL CStereoCali::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_NUM_X)->SetWindowText("0");				//x����ǵ������ʼ��Ϊ0
	GetDlgItem(IDC_NUM_Y)->SetWindowText("0");				//y����Ľǵ������ʼ��Ϊ0
	GetDlgItem(IDC_SQUARESIZE)->SetWindowText("0.0");		//���̸�߳���ʼ��Ϊ0
	GetDlgItem(IDC_IMGCOUNT)->SetWindowText("0");		//ͼ�������ʼ��Ϊ0
	GetDlgItem(IDC_PIC_NAME_L)->SetWindowText("left");			//�������Ƭbasename��ʼ��Ϊ��left��
	GetDlgItem(IDC_PIC_NAME_R)->SetWindowText("right");			//�������Ƭbasename��ʼ��Ϊ��right��


	setVoidPath(strPath);//��ʼ��strPath����������Ϊ���������գ������������ж��Ƿ������˵�ַ

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CStereoCali::OnBnClickedOpenfile()
{
	getPath(strPath);

}


void CStereoCali::getPath(CString& strPath)
{
	char szDir[MAX_PATH]; 
	 BROWSEINFO bi; 
	 ITEMIDLIST *pidl;
	 bi.hwndOwner = this->m_hWnd; 
	 bi.pidlRoot = NULL; 
	 bi.pszDisplayName = szDir; 
	 bi.lpszTitle ="��ѡ��ͼƬ���Ŀ¼"; 
	 bi.ulFlags =  BIF_STATUSTEXT; 
	 //bi.lpfn = NULL;
	 bi.lpfn=BrowseCallbackProc;		//�ص�����
	 bi.lParam=NULL;
	 bi.iImage = 0;

	 pidl = SHBrowseForFolder(&bi); 
	 if(pidl == NULL) 
	  return;
	 if(SHGetPathFromIDList(pidl, szDir))
	 {
	  strPath.Format("%s", szDir);
	}
		strPath.Replace("\\","\\\\"); //ת���ַ�/��ת��

		//return m_cstrSavePath;
}


int CALLBACK CStereoCali::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	char szPath[] = "E:\\pictures\\my_cam";
    switch(uMsg)
    {
         
    case BFFM_INITIALIZED:    //��ʼ����ϢBFFM_INITIALIZED
        ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szPath);   //����Ĭ�ϴ�·�� break;
    case BFFM_SELCHANGED:    //ѡ��·���仯��BFFM_SELCHANGED
        {
            char curr[MAX_PATH];   
            SHGetPathFromIDList((LPCITEMIDLIST)lParam,curr);   
            ::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)&curr[0]);  
        }
        break;
    default:
        break;
    } 
	return 0;
}


void CStereoCali::setVoidPath(CString & strPath)
{
	strPath="";
}


void CStereoCali::cornerDetection(CString& picPath,CString strBasename,int imageCount,vector<vector<Point2f>> & image_points_seq)
{
	vector<Point2f> image_points_buf;                                   //ÿ��ͼƬ�Ľǵ�
	Size board_size =Size(nCornerNumX,nCornerNumY); //����������ı궨����ڽǵ�ĺ�����������
	string m_picPath=picPath.GetBuffer(0);
	for(int i=0;i<imageCount;i++)
	{
			char buff[200]; 
			sprintf(buff,"%02d", i);
			string m_int2str(buff);	
			string fileName=m_picPath+"\\\\"+strBasename.GetBuffer(0)+m_int2str+".jpg";//������ÿ����Ƭ�������Ե�ַ
			Mat m_pic=imread(fileName);//��ʱ��Mat��������������ÿ����Ƭ
			if(m_pic.empty())
			{
				AfxMessageBox("Error reading picture");
				return;
			}
			if(i==0)									//Ϊ�������ֻ����һ��ͼƬ�Ĵ�С���������е�ͼƬ��Сһ��
			 {
				 imageSize.width=m_pic.cols;  //imageSize�Ƕ�����App�����Աߵ�ȫ�ֱ���
				 imageSize.height=m_pic.rows;
			 }
			if (0 == findChessboardCorners(m_pic,board_size,image_points_buf))  
				{   
					CString str_Num;
					str_Num.Format("%02d",i);
				   AfxMessageBox("can not find chessboard any corners on picture"+str_Num+" !"); //�Ҳ����ǵ�  
					//return;
				}   
				else
				{
					Mat view_gray;                                                  //�Ҷ�ͼ
					cvtColor(m_pic,view_gray,CV_RGB2GRAY);  //ת��Ϊ�Ҷ�ͼ
					 find4QuadCornerSubpix(view_gray,image_points_buf,Size(5,5)); //�Դ���ȡ�Ľǵ���о�ȷ��  
					 image_points_seq.push_back(image_points_buf); //��������
				}
	}
}

	void CStereoCali::getRealCoor(vector<vector<Point3f>> &object_points,int image_count,Size board_size,Size square_size)
	{
		int i,j,t;  
		for (t=0;t<image_count;t++)   
		{  
			vector<Point3f> tempPointSet;  
			for (i=0;i<board_size.height;i++)   
			{  
				for (j=0;j<board_size.width;j++)   
				{  
					Point3f realPoint;  
					/* ����궨�������������ϵ��z=0��ƽ���� */  
					realPoint.x =(float) i*square_size.width;  
					realPoint.y = (float)j*square_size.height;  
					realPoint.z = (float)0;  
					tempPointSet.push_back(realPoint);  
				}  
			}  
			object_points.push_back(tempPointSet);  
		}  
	}