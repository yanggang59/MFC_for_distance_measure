1.如何在MFC中声明全局变量：
每个mfc工程都会有个theApp的全局变量，双击它 找到它的位置，然后把你要定义的全局变量跟他写在一起
接下来这个文件的.h文件中用extern声明该变量就可以了，注意声明的时候带上类型。
2.showImage函数在三个类里面存在，分别是Ccamera_show_MFCDlg，CShotDlg和CCalibDlg类，这是一个不理想的设计，应该使用继承，
或者将它设计成一个静态函数(败笔之一)
3.在标定的对话框里
记录X方向和Y方向的内角点个数的文本框的关联变量分别是int类型的nCornerNumX和nCornerNumY
图片的张数关联变量是int类型的nImageCount，图片的basename是CString类型的strBasename。
棋盘格的边长的关联变量是double类型的d_Square_Size
在标定对话框初始化的时候，应该将上面的四个关联变量初始化，方便接下来的判断，但貌似MFC已经帮我们做好了，
nCornerNumX和nCornerNumY，nImageCount分别初始化为了0，（至少打开标定对话框以后，显示0）为了保险，
还是初始化一下。
GetDlgItem(IDC_NUM_X)->SetWindowText("0");
GetDlgItem(IDC_NUM_Y)->SetWindowText("0");
GetDlgItem(IDC_IMAGECOUNT)->SetWindowText("0");
GetDlgItem(IDC_SQUARESIZE)->SetWindowText("0");

4.MFC程序中的编辑框有时候需要限制只能输入数字，将其number属性设置为true即可

5.获得图片所在路径,角点检测,获得真实的三维世界坐标都已经封装成了函数，方便调用

6.拍照对话框中，保存照片之前要填写basename，两个文本框分别是IDC_BASENAME1和IDC_BASENAME2，为它们建立了CString类型的
关联变量，分别是str_Basename1和str_Basename2,在窗口初始化的时候先对其初始化为“”。

7.在标定对话框，当点击开始标定的按钮之后，将保存数据的按钮和自己禁用，直到标定结束
先添加两个按钮的关联变量，或者使用控件指针实现也可以，然后使用EnableWindow函数实现

8.在App类中的全局变量，一般的全局变量都放在了这里，有几个例外，就是 m_cstrSavePath，用来获得待标定图片路径的CString类型
变量，放在了CCalibDlg类的Public声明里面，以后修改也方便。（另外是int类型的delay和i
delay=SetTimer(5,100,NULL),i是用在循环里计数的,使用失败，改用Sleep（）函数）


9.待加入的功能：每当开启另外一个窗口的时候，前一个窗口隐藏，当前窗口关闭的时候，前一个窗口再显示。

10.在立体标定的对话框里，设置了一个全局变量strPath，作为标定的时候指定图片的路径，在StereoCalib.cpp里面，没有放在App类里面，简单起见，但是在初始化的时候会出现问题
因为我的想法是，在开始的时候将strPath置为“”,但是不能直接赋值，因为不能直接对类的成员直接操作，所以编写了一个setVoidPath函数来进行
初始化的操作。
void CStereoCali::setVoidPath(CString & strPath)
{
	strPath="";
}


11.如果使用ofstream报错，但是代码没有问题，很有可能是因为没有包含必要的头文件导致的，应该要包含上
#include <iostream>
#include <fstream>  
还要使用 using namespace std;来说明一下命名空间。