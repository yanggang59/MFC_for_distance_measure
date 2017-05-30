// CalibDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "CalibDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <iostream>
#include <fstream>  
#include "CvvImage.h"
// CCalibDlg �Ի���
/**/
  


IMPLEMENT_DYNAMIC(CCalibDlg, CDialog)

CCalibDlg::CCalibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalibDlg::IDD, pParent)
	, nCornerNumX(0)
	, nCornerNumY(0)
	, nImageCount(0)
	, strBasename(_T(""))
	, d_Square_Size(0)
{

}

CCalibDlg::~CCalibDlg()
{
}

void CCalibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUM_X, nCornerNumX);
	DDX_Text(pDX, IDC_NUM_Y, nCornerNumY);
	DDX_Text(pDX, IDC_IMAGECOUNT, nImageCount);
	DDX_Text(pDX, IDC_PIC_NAME, strBasename);
	DDX_Text(pDX, IDC_SQUARESIZE, d_Square_Size);
}


BEGIN_MESSAGE_MAP(CCalibDlg, CDialog)
	ON_BN_CLICKED(IDC_STARTCALI, &CCalibDlg::OnBnClickedStartcali)
	ON_BN_CLICKED(IDC_SAVERESULT, &CCalibDlg::OnBnClickedSaveresult)
	ON_BN_CLICKED(IDC_SHOWRECTIFY, &CCalibDlg::OnBnClickedShowrectify)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPENFILE, &CCalibDlg::OnBnClickedOpenfile)
END_MESSAGE_MAP()


// CCalibDlg ��Ϣ�������


void CCalibDlg::OnBnClickedStartcali()
{
	 UpdateData(TRUE);	//���ؼ��е�ֵ������Ӧ�ı�����

	 /*�����ж��Ƿ���д��Ϣ���*/
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

	if(strBasename=="")
	{
		//AfxMessageBox("Please input picture basename");
		AfxMessageBox("����������Ƭ��basename");
		return;
	}

	if(strPath=="")
	{
		AfxMessageBox("����������Ƭ��·��");
		return;
	}
	/*���Լ��ͱ���궨���ݰ�ť��Ϊ����״̬��ֱ���궨����*/
	CButton *b_start=(CButton*)GetDlgItem(IDC_STARTCALI);//IDC_STARTCALIΪ��ʼ�궨��ť��ID
	CButton *b_saveData=(CButton*)GetDlgItem(IDC_SAVERESULT);//IDC_SAVERESULTΪ�������ݰ�ť
	CButton *b_showRectify=(CButton*)GetDlgItem(IDC_SHOWRECTIFY);//IDC_SHOWRECTIFYΪ��ʾ��ť
	b_start->EnableWindow(FALSE);                     //����Ϊ����״̬
	b_saveData->EnableWindow(FALSE);
	b_showRectify->EnableWindow(FALSE);
	
	/*���¿�ʼ��ȡ�ǵ㣬�ǵ���ȡ�����Ѿ���������һ��cornerDetection�������������*/
	GetDlgItem(IDC_STATUS) ->SetWindowText( "���ڼ��ǵ�" );//���·���״̬������ʾ
	vector<vector<Point2f>> allCorners;
	cornerDetection(strPath,nImageCount,allCorners);//������еĽǵ�

	/*���¿�ʼ�궨*/
	GetDlgItem(IDC_STATUS) ->SetWindowText( "���ڱ궨" );//���·���״̬������ʾ��ʼ�궨
	 Size square_size = Size(d_Square_Size,d_Square_Size);  /* ʵ�ʲ����õ��ı궨����ÿ�����̸�Ĵ�С����Ҫ����λmm */  
    vector<vector<Point3f>> object_points; /* ����궨���Ͻǵ����ά���� */  
	Size board_size =Size(nCornerNumX,nCornerNumY); //����������ı궨����ڽǵ�ĺ�����������

	getRealCoor(object_points,nImageCount,board_size,square_size);
	vector<int> point_counts;  // ÿ��ͼ���нǵ������  
	/* ��ʼ��ÿ��ͼ���еĽǵ��������ٶ�ÿ��ͼ���ж����Կ��������ı궨�� */  
    for (int i=0;i<nImageCount;i++)  
    {  
        point_counts.push_back(board_size.width*board_size.height);  
    }     
	/*�������,�Ѿ�����Ϊȫ�ֱ���*/  
    //Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* ������ڲ������� */  
    //Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* �������5������ϵ����k1,k2,p1,p2,k3 */  
    //vector<Mat> tvecsMat;  /* ÿ��ͼ�����ת���� */  
    //vector<Mat> rvecsMat; /* ÿ��ͼ���ƽ������ */ 


	 calibrateCamera(object_points,allCorners,imageSize,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0); 
	 GetDlgItem(IDC_STATUS) ->SetWindowText( "�궨���" );//���·���״̬������ʾ
	 AfxMessageBox("��ϲ���궨���",MB_OK);
	
	// /*���ñ���궨����������ƽ�����ļ�·��*/
	// CFileDialog saveFileDlg(FALSE,NULL,"E:\\cali_result\\");
	// saveFileDlg.m_ofn.lpstrTitle="��ѡ�񱣴����ݵ�·�����ļ���";
	//INT_PTR result = saveFileDlg.DoModal();  
	//CString filePath ;
 //   if(result==IDOK)
	//{   
	//	filePath= saveFileDlg.GetPathName();  
 //   }  
	//filePath.Replace("\\","\\\\");
	//fout=ofstream(filePath.GetBuffer(0));  /* ����궨������ļ� */  

	//double total_err = 0.0; /* ����ͼ���ƽ�������ܺ� */  
 //   double err = 0.0; /* ÿ��ͼ���ƽ����� */  
 //   vector<Point2f> image_points2; /* �������¼���õ���ͶӰ�� */  
 //   fout<<"ÿ��ͼ��ı궨��\n";  
 //   for (int i=0;i<nImageCount;i++)  
 //   {  
 //       vector<Point3f> tempPointSet=object_points[i];  
 //       /* ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ�� */  
 //       projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],cameraMatrix,distCoeffs,image_points2);  
 //       /* �����µ�ͶӰ��;ɵ�ͶӰ��֮������*/  
 //       vector<Point2f> tempImagePoint = allCorners[i];  
 //       Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);  
 //       Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);  
 //       for (int j = 0 ; j < tempImagePoint.size(); j++)  
 //       {  
 //           image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);  
 //           tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);  
 //       }  
 //       err = norm(image_points2Mat, tempImagePointMat, NORM_L2);  
 //       total_err += err/=  point_counts[i];        
 //       fout<<"��"<<i+1<<"��ͼ���ƽ����"<<err<<"����"<<endl;     
 //   }     
 //   fout<<"����ƽ����"<<total_err/nImageCount<<"����"<<endl<<endl;     
 //  GetDlgItem(IDC_STATUS) ->SetWindowText( "error evaluation done,written done" );//���������
   b_saveData->EnableWindow(TRUE);//���������ݰ�ť��������
   b_start->EnableWindow(TRUE);                     //����Ϊ����״̬
   b_showRectify->EnableWindow(TRUE);

   //AfxMessageBox("��������궨�ļ���ť",MB_OK);
}


BOOL CCalibDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATUS) ->SetWindowText( " Ready To Calibrate" );//���·���״̬������ʾ
	GetDlgItem(IDC_NUM_X)->SetWindowText("0");				//x����ǵ������ʼ��Ϊ0
	GetDlgItem(IDC_NUM_Y)->SetWindowText("0");				//y����Ľǵ������ʼ��Ϊ0
	GetDlgItem(IDC_SQUARESIZE)->SetWindowText("0.0");		//���̸�߳���ʼ��Ϊ0
	GetDlgItem(IDC_IMAGECOUNT)->SetWindowText("0");		//ͼ�������ʼ��Ϊ0
	GetDlgItem(IDC_PIC_NAME)->SetWindowText("");			//basename��ʼ��Ϊ����
	setVoidPath(strPath);//��ʼ��strPath��ʹ��Ϊ�գ������ж���û������ͼƬ·��
	return TRUE;				// return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


int CALLBACK CCalibDlg::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
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

 void CCalibDlg::cornerDetection(CString& picPath,int imageCount,vector<vector<Point2f>> & image_points_seq)
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
	Mat m_pic=imread(fileName);    //��ʱ��Mat��������������ÿ����Ƭ
	if(m_pic.empty())
	{
		AfxMessageBox("Error reading picture");
		return;
	}
	if(i==0)									//Ϊ�������ֻ����һ��ͼƬ�Ĵ�С���������е�ͼƬ��Сһ��
	 {
		 imageSize.width=m_pic.cols;
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

void CCalibDlg::getPicPath(CString& m_cstrSavePath)
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
	  m_cstrSavePath.Format("%s", szDir);
	}
		m_cstrSavePath.Replace("\\","\\\\"); //ת���ַ�/��ת��

		//return m_cstrSavePath;
}


void CCalibDlg::getRealCoor(vector<vector<Point3f>> &object_points,int image_count,Size board_size,Size square_size)
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


void CCalibDlg::OnBnClickedSaveresult()
{
	GetDlgItem(IDC_STATUS) ->SetWindowText( "saving calibration result" );//��ʼ����궨���

	/*���ñ���궨����������ƽ�����ļ�·��*/
	 CFileDialog saveFileDlg(FALSE,NULL,"E:\\cali_result\\");
	 saveFileDlg.m_ofn.lpstrTitle="��ѡ�񱣴����ݵ�·�����ļ���";
	INT_PTR result = saveFileDlg.DoModal();  
	CString filePath ;
    if(result==IDOK)
	{   
		filePath= saveFileDlg.GetPathName();  
    }  
	filePath.Replace("\\","\\\\");
	fout=ofstream(filePath.GetBuffer(0));  /* ����궨������ļ� */  
	//FileStorage fs(filePath.GetBuffer(0), FileStorage::WRITE);

	Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0)); /* ����ÿ��ͼ�����ת���� */  
    fout<<"����ڲ�������"<<endl;     
    fout<<cameraMatrix<<endl<<endl;     
    fout<<"����ϵ����\n";     
    fout<<distCoeffs<<endl<<endl<<endl;  
	for (int i=0; i<nImageCount; i++)   
    {   
        fout<<"��"<<i+1<<"��ͼ�����ת������"<<endl;     
        fout<<tvecsMat[i]<<endl;      
        /* ����ת����ת��Ϊ���Ӧ����ת���� */     
        Rodrigues(tvecsMat[i],rotation_matrix);     
        fout<<"��"<<i+1<<"��ͼ�����ת����"<<endl;     
        fout<<rotation_matrix<<endl;     
        fout<<"��"<<i+1<<"��ͼ���ƽ��������"<<endl;     
        fout<<rvecsMat[i]<<endl<<endl;     
    }     
	AfxMessageBox("����ɹ�");
    GetDlgItem(IDC_STATUS) ->SetWindowText( "�궨�������ɹ�" );//����궨������
    fout<<endl;  
}


void CCalibDlg::OnBnClickedShowrectify()
{
	//i=0;
	CButton *b_start=(CButton*)GetDlgItem(IDC_STARTCALI);//IDC_STARTCALIΪ��ʼ�궨��ť��ID
	CButton *b_saveData=(CButton*)GetDlgItem(IDC_SAVERESULT);//IDC_SAVERESULTΪ�������ݰ�ť
	CButton *b_showRectify=(CButton*)GetDlgItem(IDC_SHOWRECTIFY);//IDC_SHOWRECTIFYΪ��ʾ��ť
	b_start->EnableWindow(FALSE);                     //����Ϊ����״̬
	b_saveData->EnableWindow(FALSE);
	b_showRectify->EnableWindow(FALSE);


	Mat mapx = Mat(imageSize,CV_32FC1);  //imageSizeΪȫ�ֱ�������App�����Ѿ�����
    Mat mapy = Mat(imageSize,CV_32FC1); 
	Mat R = Mat::eye(3,3,CV_32F);  
	for(int j=0;j<nImageCount;j++)
	{

	initUndistortRectifyMap(cameraMatrix,distCoeffs,R,cameraMatrix,imageSize,CV_32FC1,mapx,mapy);  //��Ҫ����
	char buff[200]; 
	sprintf(buff,"%02d", j);
	string m_int2str(buff);	
	string filePath=strPath.GetBuffer(0);//m_cstrSavePath��CString���͵�ͼƬ·��
	string fileName=filePath+"\\\\"+strBasename.GetBuffer(0)+m_int2str+".jpg";//������ÿ����Ƭ�������Ե�ַ
	Mat m_pic=imread(fileName);//��ʱ��Mat��������������ÿ����Ƭ
	Mat newimage = m_pic.clone();  //ʹ��newimage��m_pic��Size��ͨ����ͬ
    //��һ�ֲ���Ҫת������ķ�ʽ  
   //undistort(imageSource,newimage,cameraMatrix,distCoeffs);  
    remap(m_pic,newimage,mapx, mapy, INTER_LINEAR);      
	showImage(m_pic,IDC_ORIGINAL);
	showImage(newimage,IDC_RECTIFIED);
	//����Ӧ�ü�����ʱ�������Ժ������
	/*SetTimer(5,10000,NULL);*/
	Sleep(1000);
	}


	b_start->EnableWindow(TRUE);                     //����Ϊ����״̬
	b_saveData->EnableWindow(TRUE);
	b_showRectify->EnableWindow(TRUE);

}


void CCalibDlg::showImage(Mat src, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();//����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��
	HDC hdc= pDC->GetSafeHdc();           // ��ȡ�豸�����ľ��
	CRect rect;
   GetDlgItem(ID)->GetClientRect(&rect); //��ȡbox1�ͻ���
   CvvImage cimg;
   IplImage img(src); // ����IplImage����img     
   cimg.CopyOf(&img,img.nChannels);
   cimg.DrawToHDC(hdc,&rect);
   ReleaseDC( pDC );
   cimg.Destroy();
}


//void CCalibDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	if(5==nIDEvent)
//	{
//		 KillTimer(5);
//	}
//
//	CDialog::OnTimer(nIDEvent);
//}


void CCalibDlg::OnBnClickedOpenfile()
{
	getPicPath(strPath);/*ָ��ͼƬ��ŵ�·��*/
}


void CCalibDlg::setVoidPath(CString & strPath)//��ʼ��strPath��ʹ��Ϊ�գ������ж���û������ͼƬ·��
{
	strPath="";
}
