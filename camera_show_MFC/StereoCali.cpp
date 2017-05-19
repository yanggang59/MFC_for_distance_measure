// StereoCali.cpp : 实现文件
//

#include "stdafx.h"
#include "camera_show_MFC.h"
#include "StereoCali.h"
#include "afxdialogex.h"
#include "resource.h"
#include <iostream>
#include <fstream>  

using namespace std;
// CStereoCali 对话框

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


// CStereoCali 消息处理程序


void CStereoCali::OnBnClickedStartcali()
{
	UpdateData(TRUE);

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
	if(strPath=="")
	{
		AfxMessageBox("请先指定图片所在路径");
		return;
	}


	/*开始角点检测*/
	GetDlgItem(IDC_STATUSBAR)->SetWindowText("正在检测角点");
	vector<vector<Point2f>> allCornersL,allCornersR;
	cornerDetection(strPath,strLBasename,nImageCount,allCornersL);
	cornerDetection(strPath,strRBasename,nImageCount,allCornersR);


	/*以下开始标定*/
	GetDlgItem(IDC_STATUSBAR)->SetWindowText("正在单目标定");
	Size square_size = Size(d_Square_Size,d_Square_Size);  /* 实际测量得到的标定板上每个棋盘格的大小，重要，单位mm */  
    vector<vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */  
	Size board_size =Size(nCornerNumX,nCornerNumY); //这里输入你的标定板的内角点的横向和纵向个数
	getRealCoor(object_points,nImageCount,board_size,square_size);//获得棋盘格的真实世界坐标
	vector<int> point_counts;  // 每幅图像中角点的数量  
	/* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */  
    for (int i=0;i<nImageCount;i++)  
    {  
        point_counts.push_back(board_size.width*board_size.height);  
    }     

	calibrateCamera(object_points,allCornersL,imageSize,cameraMatrixL,distCoeffL,rvecsMatL,tvecsMatL,0); 
	calibrateCamera(object_points,allCornersR,imageSize,cameraMatrixR,distCoeffR,rvecsMatR,tvecsMatR,0); 


	GetDlgItem(IDC_STATUSBAR)->SetWindowText("正在进行立体标定");
	//Mat R, T, E, F;                                         //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵 ，改为全局变量
	double rms = stereoCalibrate(object_points, allCornersL, allCornersR,  
        cameraMatrixL, distCoeffL,  
        cameraMatrixR, distCoeffR,  
        imageSize, R, T, E, F,  
        TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));  

	GetDlgItem(IDC_STATUSBAR)->SetWindowText("立体标定完成");
}


void CStereoCali::OnBnClickedSavefile()
	{
		GetDlgItem(IDC_STATUSBAR)->SetWindowText("保存文件");

		CFileDialog saveFileDlg(FALSE,NULL,"E:\\cali_result\\");
		saveFileDlg.m_ofn.lpstrTitle="请选择保存数据的路径与文件名";
		INT_PTR result = saveFileDlg.DoModal();  
		CString filePath ;
		if(result==IDOK)  filePath= saveFileDlg.GetPathName();  		
	    filePath.Replace("\\","\\\\");
		fout_stereo = ofstream(filePath.GetBuffer(0));  /* 保存标定结果的文件 */  

	   fout_stereo<<"--------------------左相机的参数-------------------"<<endl;     
	   fout_stereo<<"相机内参数矩阵：\n"<<endl;     
	   fout_stereo<<cameraMatrixL<<endl<<endl;     
       fout_stereo<<"畸变系数：\n";     
       fout_stereo<<distCoeffL<<endl<<endl<<endl;  

		
	   fout_stereo<<"--------------------右相机的参数-------------------"<<endl;     
	   fout_stereo<<"相机内参数矩阵：\n"<<endl;     
	   fout_stereo<<cameraMatrixR<<endl<<endl;     
       fout_stereo<<"畸变系数：\n";     
       fout_stereo<<distCoeffR<<endl<<endl<<endl;  


	   fout_stereo<<"--------------------左右相机位姿关系-------------------"<<endl;     
	   fout_stereo<<"旋转矩阵"<<endl; 
	   fout_stereo<<R<<endl<<endl;
	   fout_stereo<<"平移矢量"<<endl; 
	   fout_stereo<<T<<endl<<endl;
	   fout_stereo<<"本质矩阵"<<endl; 
	   fout_stereo<<E<<endl<<endl;
	   fout_stereo<<"基本矩阵"<<endl; 
	   fout_stereo<< F<<endl<<endl;


	   GetDlgItem(IDC_STATUSBAR)->SetWindowText("保存文件成功");
}


BOOL CStereoCali::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_NUM_X)->SetWindowText("0");				//x方向角点个数初始化为0
	GetDlgItem(IDC_NUM_Y)->SetWindowText("0");				//y方向的角点个数初始化为0
	GetDlgItem(IDC_SQUARESIZE)->SetWindowText("0.0");		//棋盘格边长初始化为0
	GetDlgItem(IDC_IMGCOUNT)->SetWindowText("0");		//图像个数初始化为0
	GetDlgItem(IDC_PIC_NAME_L)->SetWindowText("left");			//左相机照片basename初始化为“left”
	GetDlgItem(IDC_PIC_NAME_R)->SetWindowText("right");			//左相机照片basename初始化为“right”


	setVoidPath(strPath);//初始化strPath，将其设置为“”，（空），这样可以判断是否设置了地址

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
	  strPath.Format("%s", szDir);
	}
		strPath.Replace("\\","\\\\"); //转义字符/的转换

		//return m_cstrSavePath;
}


int CALLBACK CStereoCali::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
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


void CStereoCali::setVoidPath(CString & strPath)
{
	strPath="";
}


void CStereoCali::cornerDetection(CString& picPath,CString strBasename,int imageCount,vector<vector<Point2f>> & image_points_seq)
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
			Mat m_pic=imread(fileName);//临时的Mat变量，用来读入每张照片
			if(m_pic.empty())
			{
				AfxMessageBox("Error reading picture");
				return;
			}
			if(i==0)									//为简单起见，只输入一次图片的大小，假设所有的图片大小一样
			 {
				 imageSize.width=m_pic.cols;  //imageSize是定义在App对象旁边的全局变量
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