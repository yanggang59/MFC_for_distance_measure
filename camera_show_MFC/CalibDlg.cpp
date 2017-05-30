// CalibDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "CalibDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <iostream>
#include <fstream>  
#include "CvvImage.h"
// CCalibDlg 对话框
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


// CCalibDlg 消息处理程序


void CCalibDlg::OnBnClickedStartcali()
{
	 UpdateData(TRUE);	//将控件中的值传到对应的变量中

	 /*下面判断是否填写信息完毕*/
	if(nCornerNumX==0)
	{
		//AfxMessageBox("Please input num X");
		AfxMessageBox("请先输入X方向角点个数");
		return;
	}

	if(nCornerNumY==0)
	{
		//AfxMessageBox("Please input num Y");
		AfxMessageBox("请先输入Y方向角点个数");
		return;
	}
	if(d_Square_Size==0.0)
	{
		//AfxMessageBox("Please input square size");
		AfxMessageBox("请先输入棋盘格边长");
		return;
	}
	if(nImageCount==0)
	{
		//AfxMessageBox("Please input number of pictures");
		AfxMessageBox("请先输入照片张数");
		return;
	}

	if(strBasename=="")
	{
		//AfxMessageBox("Please input picture basename");
		AfxMessageBox("请先输入照片的basename");
		return;
	}

	if(strPath=="")
	{
		AfxMessageBox("请先输入照片的路径");
		return;
	}
	/*将自己和保存标定数据按钮置为禁用状态，直至标定结束*/
	CButton *b_start=(CButton*)GetDlgItem(IDC_STARTCALI);//IDC_STARTCALI为开始标定按钮的ID
	CButton *b_saveData=(CButton*)GetDlgItem(IDC_SAVERESULT);//IDC_SAVERESULT为保存数据按钮
	CButton *b_showRectify=(CButton*)GetDlgItem(IDC_SHOWRECTIFY);//IDC_SHOWRECTIFY为显示按钮
	b_start->EnableWindow(FALSE);                     //均置为禁用状态
	b_saveData->EnableWindow(FALSE);
	b_showRectify->EnableWindow(FALSE);
	
	/*以下开始提取角点，角点提取函数已经被到包成一个cornerDetection函数，方便调用*/
	GetDlgItem(IDC_STATUS) ->SetWindowText( "正在检测角点" );//最下方的状态条的显示
	vector<vector<Point2f>> allCorners;
	cornerDetection(strPath,nImageCount,allCorners);//获得所有的角点

	/*以下开始标定*/
	GetDlgItem(IDC_STATUS) ->SetWindowText( "正在标定" );//最下方的状态条的显示开始标定
	 Size square_size = Size(d_Square_Size,d_Square_Size);  /* 实际测量得到的标定板上每个棋盘格的大小，重要，单位mm */  
    vector<vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */  
	Size board_size =Size(nCornerNumX,nCornerNumY); //这里输入你的标定板的内角点的横向和纵向个数

	getRealCoor(object_points,nImageCount,board_size,square_size);
	vector<int> point_counts;  // 每幅图像中角点的数量  
	/* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */  
    for (int i=0;i<nImageCount;i++)  
    {  
        point_counts.push_back(board_size.width*board_size.height);  
    }     
	/*内外参数,已经定义为全局变量*/  
    //Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* 摄像机内参数矩阵 */  
    //Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */  
    //vector<Mat> tvecsMat;  /* 每幅图像的旋转向量 */  
    //vector<Mat> rvecsMat; /* 每幅图像的平移向量 */ 


	 calibrateCamera(object_points,allCorners,imageSize,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0); 
	 GetDlgItem(IDC_STATUS) ->SetWindowText( "标定完成" );//最下方的状态条的显示
	 AfxMessageBox("恭喜，标定完成",MB_OK);
	
	// /*设置保存标定结果与误差估计结果的文件路径*/
	// CFileDialog saveFileDlg(FALSE,NULL,"E:\\cali_result\\");
	// saveFileDlg.m_ofn.lpstrTitle="请选择保存数据的路径与文件名";
	//INT_PTR result = saveFileDlg.DoModal();  
	//CString filePath ;
 //   if(result==IDOK)
	//{   
	//	filePath= saveFileDlg.GetPathName();  
 //   }  
	//filePath.Replace("\\","\\\\");
	//fout=ofstream(filePath.GetBuffer(0));  /* 保存标定结果的文件 */  

	//double total_err = 0.0; /* 所有图像的平均误差的总和 */  
 //   double err = 0.0; /* 每幅图像的平均误差 */  
 //   vector<Point2f> image_points2; /* 保存重新计算得到的投影点 */  
 //   fout<<"每幅图像的标定误差：\n";  
 //   for (int i=0;i<nImageCount;i++)  
 //   {  
 //       vector<Point3f> tempPointSet=object_points[i];  
 //       /* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */  
 //       projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],cameraMatrix,distCoeffs,image_points2);  
 //       /* 计算新的投影点和旧的投影点之间的误差*/  
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
 //       fout<<"第"<<i+1<<"幅图像的平均误差："<<err<<"像素"<<endl;     
 //   }     
 //   fout<<"总体平均误差："<<total_err/nImageCount<<"像素"<<endl<<endl;     
 //  GetDlgItem(IDC_STATUS) ->SetWindowText( "error evaluation done,written done" );//误差估计完成
   b_saveData->EnableWindow(TRUE);//将保存数据按钮重新启用
   b_start->EnableWindow(TRUE);                     //均置为启用状态
   b_showRectify->EnableWindow(TRUE);

   //AfxMessageBox("请点击保存标定文件按钮",MB_OK);
}


BOOL CCalibDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATUS) ->SetWindowText( " Ready To Calibrate" );//最下方的状态条的显示
	GetDlgItem(IDC_NUM_X)->SetWindowText("0");				//x方向角点个数初始化为0
	GetDlgItem(IDC_NUM_Y)->SetWindowText("0");				//y方向的角点个数初始化为0
	GetDlgItem(IDC_SQUARESIZE)->SetWindowText("0.0");		//棋盘格边长初始化为0
	GetDlgItem(IDC_IMAGECOUNT)->SetWindowText("0");		//图像个数初始化为0
	GetDlgItem(IDC_PIC_NAME)->SetWindowText("");			//basename初始化为“”
	setVoidPath(strPath);//初始化strPath，使其为空，用来判断有没有设置图片路径
	return TRUE;				// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


int CALLBACK CCalibDlg::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	char szPath[] = "E:\\pictures\\my_cam";
    switch(uMsg)
    {
         
    case BFFM_INITIALIZED:    //初始化消息BFFM_INITIALIZED
        ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szPath);   //传递默认打开路径 break;
    case BFFM_SELCHANGED:    //选择路径变化，BFFM_SELCHANGED
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
	vector<Point2f> image_points_buf;                                   //每幅图片的角点
	Size board_size =Size(nCornerNumX,nCornerNumY); //这里输入你的标定板的内角点的横向和纵向个数
	string m_picPath=picPath.GetBuffer(0);
	for(int i=0;i<imageCount;i++)
	{
	char buff[200]; 
	sprintf(buff,"%02d", i);
	string m_int2str(buff);	
	string fileName=m_picPath+"\\\\"+strBasename.GetBuffer(0)+m_int2str+".jpg";//完整的每张照片名，绝对地址
	Mat m_pic=imread(fileName);    //临时的Mat变量，用来读入每张照片
	if(m_pic.empty())
	{
		AfxMessageBox("Error reading picture");
		return;
	}
	if(i==0)									//为简单起见，只输入一次图片的大小，假设所有的图片大小一样
	 {
		 imageSize.width=m_pic.cols;
		 imageSize.height=m_pic.rows;
	 }
	if (0 == findChessboardCorners(m_pic,board_size,image_points_buf))  
        {   
			CString str_Num;
			str_Num.Format("%02d",i);
           AfxMessageBox("can not find chessboard any corners on picture"+str_Num+" !"); //找不到角点  
			//return;
        }   
		else
		{
			Mat view_gray;                                                  //灰度图
            cvtColor(m_pic,view_gray,CV_RGB2GRAY);  //转换为灰度图
			 find4QuadCornerSubpix(view_gray,image_points_buf,Size(5,5)); //对粗提取的角点进行精确化  
			 image_points_seq.push_back(image_points_buf); //存入数组
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
	 bi.lpszTitle ="请选择图片存放目录"; 
	 bi.ulFlags =  BIF_STATUSTEXT; 
	 //bi.lpfn = NULL;
	 bi.lpfn=BrowseCallbackProc;		//回调函数
	 bi.lParam=NULL;
	 bi.iImage = 0;

	 pidl = SHBrowseForFolder(&bi); 
	 if(pidl == NULL) 
	  return;
	 if(SHGetPathFromIDList(pidl, szDir))
	 {
	  m_cstrSavePath.Format("%s", szDir);
	}
		m_cstrSavePath.Replace("\\","\\\\"); //转义字符/的转换

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
                /* 假设标定板放在世界坐标系中z=0的平面上 */  
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
	GetDlgItem(IDC_STATUS) ->SetWindowText( "saving calibration result" );//开始保存标定结果

	/*设置保存标定结果与误差估计结果的文件路径*/
	 CFileDialog saveFileDlg(FALSE,NULL,"E:\\cali_result\\");
	 saveFileDlg.m_ofn.lpstrTitle="请选择保存数据的路径与文件名";
	INT_PTR result = saveFileDlg.DoModal();  
	CString filePath ;
    if(result==IDOK)
	{   
		filePath= saveFileDlg.GetPathName();  
    }  
	filePath.Replace("\\","\\\\");
	fout=ofstream(filePath.GetBuffer(0));  /* 保存标定结果的文件 */  
	//FileStorage fs(filePath.GetBuffer(0), FileStorage::WRITE);

	Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */  
    fout<<"相机内参数矩阵："<<endl;     
    fout<<cameraMatrix<<endl<<endl;     
    fout<<"畸变系数：\n";     
    fout<<distCoeffs<<endl<<endl<<endl;  
	for (int i=0; i<nImageCount; i++)   
    {   
        fout<<"第"<<i+1<<"幅图像的旋转向量："<<endl;     
        fout<<tvecsMat[i]<<endl;      
        /* 将旋转向量转换为相对应的旋转矩阵 */     
        Rodrigues(tvecsMat[i],rotation_matrix);     
        fout<<"第"<<i+1<<"幅图像的旋转矩阵："<<endl;     
        fout<<rotation_matrix<<endl;     
        fout<<"第"<<i+1<<"幅图像的平移向量："<<endl;     
        fout<<rvecsMat[i]<<endl<<endl;     
    }     
	AfxMessageBox("保存成功");
    GetDlgItem(IDC_STATUS) ->SetWindowText( "标定结果保存成功" );//保存标定结果完成
    fout<<endl;  
}


void CCalibDlg::OnBnClickedShowrectify()
{
	//i=0;
	CButton *b_start=(CButton*)GetDlgItem(IDC_STARTCALI);//IDC_STARTCALI为开始标定按钮的ID
	CButton *b_saveData=(CButton*)GetDlgItem(IDC_SAVERESULT);//IDC_SAVERESULT为保存数据按钮
	CButton *b_showRectify=(CButton*)GetDlgItem(IDC_SHOWRECTIFY);//IDC_SHOWRECTIFY为显示按钮
	b_start->EnableWindow(FALSE);                     //均置为禁用状态
	b_saveData->EnableWindow(FALSE);
	b_showRectify->EnableWindow(FALSE);


	Mat mapx = Mat(imageSize,CV_32FC1);  //imageSize为全局变量，在App类中已经定义
    Mat mapy = Mat(imageSize,CV_32FC1); 
	Mat R = Mat::eye(3,3,CV_32F);  
	for(int j=0;j<nImageCount;j++)
	{

	initUndistortRectifyMap(cameraMatrix,distCoeffs,R,cameraMatrix,imageSize,CV_32FC1,mapx,mapy);  //主要函数
	char buff[200]; 
	sprintf(buff,"%02d", j);
	string m_int2str(buff);	
	string filePath=strPath.GetBuffer(0);//m_cstrSavePath是CString类型的图片路径
	string fileName=filePath+"\\\\"+strBasename.GetBuffer(0)+m_int2str+".jpg";//完整的每张照片名，绝对地址
	Mat m_pic=imread(fileName);//临时的Mat变量，用来读入每张照片
	Mat newimage = m_pic.clone();  //使得newimage和m_pic的Size，通道相同
    //另一种不需要转换矩阵的方式  
   //undistort(imageSource,newimage,cameraMatrix,distCoeffs);  
    remap(m_pic,newimage,mapx, mapy, INTER_LINEAR);      
	showImage(m_pic,IDC_ORIGINAL);
	showImage(newimage,IDC_RECTIFIED);
	//这里应该加上延时函数，以后再添加
	/*SetTimer(5,10000,NULL);*/
	Sleep(1000);
	}


	b_start->EnableWindow(TRUE);                     //均置为启用状态
	b_saveData->EnableWindow(TRUE);
	b_showRectify->EnableWindow(TRUE);

}


void CCalibDlg::showImage(Mat src, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();//根据ID获得窗口指针再获取与该窗口关联的上下文指针
	HDC hdc= pDC->GetSafeHdc();           // 获取设备上下文句柄
	CRect rect;
   GetDlgItem(ID)->GetClientRect(&rect); //获取box1客户区
   CvvImage cimg;
   IplImage img(src); // 定义IplImage变量img     
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
	getPicPath(strPath);/*指定图片存放的路径*/
}


void CCalibDlg::setVoidPath(CString & strPath)//初始化strPath，使其为空，用来判断有没有设置图片路径
{
	strPath="";
}
