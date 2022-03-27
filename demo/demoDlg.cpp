
// demoDlg.cpp: 实现文件

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"
#include "digital.h"
#include<malloc.h>
#include<iostream>
#include <fstream>
#include<vector>
//#define th 10
#define th1 20
using namespace std;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CdemoDlg 对话框



CdemoDlg::CdemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CdemoDlg::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BUTTON1, &CdemoDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &CdemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_OpenCam, &CdemoDlg::OnBnClickedOpencam)
	ON_BN_CLICKED(IDC_StartGrab, &CdemoDlg::OnBnClickedStartgrab)
	ON_BN_CLICKED(IDC_CloseCam, &CdemoDlg::OnBnClickedClosecam)
	ON_STN_CLICKED(pic, &CdemoDlg::OnStnClickedpic)
	ON_BN_CLICKED(startRecg, &CdemoDlg::OnBnClickedstartrecg)
	ON_BN_CLICKED(IDCANCEL, &CdemoDlg::OnBnClickedCancel)
	ON_STN_CLICKED(picGray, &CdemoDlg::OnStnClickedpicgray)
END_MESSAGE_MAP()


// CdemoDlg 消息处理程序

BOOL CdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	MVSTATUS_CODES r;
	r = MVInitLib();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("函数库初始化失败！"), _T("提示"), MB_ICONWARNING);
		return TRUE;
	}
	r = MVUpdateCameraList();
	if (r != MVST_SUCCESS)
	{
		MessageBox(_T("查找连接计算机的相机失败！"), _T("提示"), MB_ICONWARNING);
		return TRUE;
	}
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdemoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CdemoDlg::OnBnClickedOpencam()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		MessageBox(_T("没有找到相机, 请确认连接和相机IP设置"), _T("提示"),MB_ICONWARNING);
			return;
	}
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox(_T("无法打开相机, 可能正被别的软件控制"), _T("提示"),MB_ICONWARNING);
		else
			MessageBox(_T("无法打开相机"), _T("提示"), MB_ICONWARNING);
		return;
	}
	int w, h;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &m_PixelFormat);
	m_image.CreateByPixelFormat(w, h, m_PixelFormat);
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(true);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
}

void CdemoDlg::DrawImage()
{
	CRect rct;
	GetDlgItem(pic)->GetClientRect(&rct);
	int dstW = rct.Width();
	int dstH = rct.Height();
	CDC* pDC = GetDlgItem(pic)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		m_image.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
	}
	ReleaseDC(pDC);
}

void CdemoDlg::DrawImageGray()
{
	CRect rct;
	GetDlgItem(picGray)->GetClientRect(&rct);
	int dstW = rct.Width();
	int dstH = rct.Height();
	CDC* pDC = GetDlgItem(picGray)->GetDC();
	{
		pDC->SetStretchBltMode(BLACKONWHITE);
		m_imageGray.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
	}
	ReleaseDC(pDC);
}

int CdemoDlg::OnStreamCB(MV_IMAGE_INFO* pInfo)
{

	MVInfo2Image(m_hCam, pInfo, &m_image);
	DrawImage();
	return 0;
}

int __stdcall StreamCB(MV_IMAGE_INFO* pInfo, ULONG_PTR nUserVal)
{
	CdemoDlg* pDlg = (CdemoDlg*)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}

void CdemoDlg::OnBnClickedStartgrab()
{
	// TODO: 在此添加控件通知处理程序代码
	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}
	MVStartGrab(m_hCam, (MVStreamCB)StreamCB, (ULONG_PTR)this);//采集图像
	m_bRun = true;
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(true);

}

// “关闭相机”点击事件
void CdemoDlg::OnBnClickedClosecam()
{
	// TODO: 在此添加控件通知处理程序代码
	MVStopGrab(m_hCam);
	MVCloseCam(m_hCam);
	m_bRun = false;
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
}


void CdemoDlg::OnStnClickedpic()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CdemoDlg::OnClose()
{
	if (m_bRun != false)
	{
		MVStopGrab(m_hCam);
	}
	MVTerminateLib();
	CDialog::OnClose();
}

BOOL CdemoDlg::readBMP()
{
	//img = m_image.Load((LPCTSTR)"../photo/photo/1.bmp"); // 这个自带的方法不行
	int wid = 0, hgt = 0;
	int len = 0, offset = 0;
	char temp[4] = { 0 };

	// 1. 打开图片文件
	ifstream img("../photo/photo/3.bmp", ifstream::in | ios::binary); // 默认打开的是1.bmp
	if (!img) {
		cout << "Can not open this image!" << endl;
		return false;
	}
	// 2. 计算图片长度
	img.seekg(2, ios::beg);
	img.read(temp, 4); // bfSize
	memcpy(&len, temp, 4);
	img.seekg(10, ios::beg);
	img.read(temp, 4); // bfOffBits
	memcpy(&offset, temp, 4);
	img.seekg(18, ios::beg);
	img.read(temp, 4); // biWidth
	memcpy(&wid, temp, 4);
	img.seekg(22, ios::beg);
	img.read(temp, 4); // biHeight
	memcpy(&hgt, temp, 4);
	// 3. 读取RGB数值
	img.seekg(offset, ios::beg);
	m_image.CreateByPixelFormat(wid, hgt, PixelFormat_BayerRG8); // RGB
	m_imageGray.CreateByPixelFormat(wid, hgt, PixelFormat_Mono8); // 灰度
	m_imageDid.CreateByPixelFormat(wid, hgt, PixelFormat_Mono8); // 灰度
	char* p = (char*)m_image.GetBits();
	img.read(p, wid * hgt * 3); // 这样读出来是上下颠倒的
	img.close();

	return true;
}

void CdemoDlg::getGay()
{
	//MVBGRToGray(HANDLE hCam, unsigned char* psrc, unsigned char* pdst, unsigned int width, unsigned int height); // 没连相机，应该不行

}

// “开始识别”点击事件
void CdemoDlg::OnBnClickedstartrecg()
{
	readBMP();
	DrawImage();
	Image_Gray();
	DrawImageGray();
	Watershed();
	//Corrode();
	//Corrode();
	//Expand();
	//Expand();

	//Susan();

	DrawImage();
	DrawImageGray();

}


void CdemoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CdemoDlg::Image_Gray()
{
	int w;
	int h;
	unsigned char* p = (unsigned char*)m_image.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageGray.GetBits();
	double sum = 0;
	double sum1 = 0;
	double count = 0;
	int i, j;

	w = m_image.GetWidth();
	h = m_image.GetHeight();

	//Kittle 二值分割
	/*count = w * h * 3;
	for (i = 0; i < count; i++)
	{
		sum = sum + *p;
		p++;
	}
	sum = sum / count;

	count = 0;
	p = (unsigned char*)m_image.GetBits();
	for (i = 0; i < count; i++)
	{
		if (*p < sum)
		{
			sum1 = sum1 + *p;
			count++;
		}
		p++;
	}*/
	
	
	p = (unsigned char*)m_image.GetBits();
	/*for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			if ((*p + *(p + 1) + *(p + 2)) / 3 > sum)
			{
				*pDst = 255;
				pDst++;
				p++;
				p++;
				p++;
			}
			else {
				*pDst = 0;
				pDst++;
				p++;
				p++;
				p++;
			}
		}
	}*/

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*pDst = 0.299 * *p + 0.587 * *(p + 1) + 0.114 * *(p + 2);
			pDst++;
			p += 3;
		}
	}
	pDst = (unsigned char*)m_imageGray.GetBits();
	for (i = 0; i < w; i++)
	{
		*(pDst + i) = 255;
		*(pDst + (h - 1)*w + i) = 255;
	}
	for (i = 0; i < h; i++)
	{
		*(pDst + i * w) = 255;
		*(pDst + i * w + w - 1) = 255;
	}

	//滤波
	pDst = (unsigned char*)m_imageGray.GetBits();
	pDst = pDst + w + 1;
	for (j = 1; j < h - 1; j++)
	{
		for (i = 1; i < w - 1; i++)
		{
				//*pDst = Median(*pDst, *(pDst-3), *(pDst+3),*(pDst-3*w), *(pDst+3*w), *(pDst-3*w-3),*(pDst-3*w+3), *(pDst+3*w-3), *(pDst+3*w+3));
				*pDst = (*pDst + *(pDst - 1) + *(pDst + 1) + *(pDst - w) + *(pDst + w) + *(pDst - w - 1) + *(pDst - w + 1) + *(pDst + w - 1) + *(pDst + w + 1)) / 9; // 均值滤波
				pDst++;
		}
	}
	//DrawImage2();
	//p = (unsigned char*)m_imageGray.GetBits();
	//pDst = (unsigned char*)m_imageDid.GetBits();
	//for (i = 0; i < h; i++)
	//{
	//	for (j = 1; j < w; j++)
	//	{
	//			*pDst = *p;
	//			pDst++;
	//			p++;
	//	}
	//
}


// 获取中值
unsigned char CdemoDlg::Median(unsigned char n1, unsigned char n2, unsigned char n3, 
	unsigned char n4, unsigned char n5, unsigned char n6, unsigned char n7, 
	unsigned char n8, unsigned char n9)
{
	unsigned char arr[9];
	unsigned char temp;
	arr[0] = n1;
	arr[1] = n2;
	arr[2] = n3;
	arr[3] = n4;
	arr[4] = n5;
	arr[5] = n6;
	arr[6] = n7;
	arr[7] = n8;
	arr[8] = n9;
	for (int gap = 9 / 2; gap > 0; gap /= 2)//希尔排序
		for (int i = gap; i < 9; ++i)
			for (int j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap)
			{
				temp = arr[j];
				arr[j] = arr[j + gap];
				arr[j + gap] = temp;
			}
	return arr[4];//返回中值
}


// 腐蚀
void CdemoDlg::Corrode()
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	unsigned char* pcur;
	int i, j;
	int w, h;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	for (i = 2; i < h - 2; i++)
	{
		for (j = 2; j < w - 2; j++)
		{
			pcur = p + i * w + j;

			if (*(pcur - 1) == 255 || *(pcur + 1) == 255 ||	\
				*(pcur - w) == 255 || *(pcur + w) == 255 ||	\
				*(pcur - w - 1) == 255 || *(pcur - w + 1) == 255 ||	\
				*(pcur + w - 1) == 255 || *(pcur + w + 1) == 255 ||	\
				*(pcur - 2) == 255 || *(pcur + 2) == 255 ||	\
				*(pcur - 2 * w) == 255 || *(pcur + 2 * w) == 255 ||	\
				*(pcur - w - 2) == 255 || *(pcur - w + 2) == 255 ||	\
				*(pcur + w -2) == 255 || *(pcur + w + 2) == 255)	\
			{
					*(pDst + i * w + j) = 255;
			}
		}
	}
	p = (unsigned char*)m_imageGray.GetBits();
	pDst = (unsigned char*)m_imageDid.GetBits();
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*p = *pDst;
			pDst++;
			p++;
		}
	}
}


// 膨胀
void CdemoDlg::Expand()
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	unsigned char* pcur;
	int i, j;
	int w, h;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	for (i = 2; i < h - 2; i++)
	{
		for (j = 2; j < w - 2; j++)
		{
			pcur = p + i * w + j;

			if (*(pcur - 1) == 0 || *(pcur + 1) == 0 ||
				*(pcur - w) == 0 || *(pcur + w) == 0 ||
				*(pcur - w - 1) == 0 || *(pcur - w + 1) == 0 ||
				*(pcur + w - 1) == 0 || *(pcur + w + 1) == 0 ||
				*(pcur - 2) == 0 || *(pcur + 2) == 0 ||
				*(pcur - 2 * w) == 0 || *(pcur + 2 * w) == 0 ||
				*(pcur - w - 2) == 0 || *(pcur - w + 2) == 0 ||
				*(pcur + w - 2) == 0 || *(pcur + w + 2) == 0)
			{
				*(pDst + i * w + j) = 0;
			}
		}
	}
	p = (unsigned char*)m_imageGray.GetBits();
	pDst = (unsigned char*)m_imageDid.GetBits();
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*p = *pDst;
			pDst++;
			p++;
		}
	}
}

// Susan算子检测边缘和角点
void CdemoDlg::Susan()
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pOrg = (unsigned char*)m_image.GetBits();
	int i, j, m, n;
	int w, h;
	unsigned char* Susan = NULL; // Susan矩阵
	int c;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	Susan = (unsigned char*)malloc(static_cast<unsigned long long>(h) * w * sizeof(unsigned char));


	// 形成Susan矩阵
	for (i = 3; i < h - 3; i++) {
		for (j = 3; j < w - 3; j++) {
			c = 0;
			for (m = 0; m < 3; m++) {
				for ( n = 0; n < 3; n++)
				{
					c += exp(-1 * pow((*(p + i * w + j) - *(p + j + i * w - w - 1 + w * m + n)) / 180, 2)); // T=130
					//c += abs(*(p + i * w + j) - *(p + j + i * w - w - 1 + w * m + n)) > 200 ? 1 : 0;
				}
			}
			if (c < 6) *(Susan + i * w + j) = 6 - c;
			else *(Susan + i * w + j) = 0;
			cout << *(Susan + i * w + j);
		}
	}

	// 判断角点和边缘
	for ( i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++) {
			if (*(Susan + i * w + j) > 1) // 角点, 在原图上显示绿色
			{
				*(pOrg + i * w * 3 + j * 3) = 0;
				*(pOrg + i * w * 3 + j * 3 + 1) = 255;
				*(pOrg + i * w * 3 + j*3 + 2) = 0;
			}
			else if(*(Susan + i * w + j) > 0) // 边缘，在原图上显示白色
			{
				*(pOrg + i * w * 3 + j * 3) = 255;
				*(pOrg + i * w * 3 + j*3 + 1) = 255;
				*(pOrg + i * w * 3 + j * 3 + 2) = 255;
			}
		}
	}
}

void CdemoDlg::OnStnClickedpicgray()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CdemoDlg::Watershed()
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	int w, h;
	int i, j, a, b, wh, count, count1;
	double sum = 0;
	double sum1 = 0;
	struct Pool1 index;
	struct Pool1* pool = NULL;
	struct Pool1* pcur = NULL;
	struct Pool1* belong = NULL;
	int state = 0;
	int typenum = 1;
	int found = 0;
	int color;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	index.end = &index;
	index.next = NULL;
	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	p = (unsigned char*)m_imageGray.GetBits();
	//for (i = 100; i < h - 100; i++)
	//{
	//	for (j = 100; j < w - 100; j++)
	//	{
	//		if ((*(p + i * w + j) < 5) && (*(p + i * w + j) >= 0))
	//		{
	//			state = 0;
	//			pcur = index.next;
	//			while (1)
	//			{
	//				if (pcur == NULL)
	//					break;
	//				w = w;
	//				if (pcur->water[i - 1][j - 1] == 1 || pcur->water[i - 1][j] == 1 || pcur->water[i - 1][j + 1] == 1
	//					|| pcur->water[i][j - 1] == 1 || pcur->water[i][j + 1] == 1
	//					|| pcur->water[i + 1][j - 1] == 1 || pcur->water[i + 1][j] == 1 || pcur->water[i + 1][j + 1] == 1)//判断该水滴是否在当前水池
	//				{
	//					state++;
	//					belong = pcur;
	//				}
	//				pcur = pcur->next;
	//			}
	//			//如果water属于某一特定pool
	//			if (state > 1) {

	//			}
	//			else if (state == 1)
	//			{
	//				belong->water[i][j] = 1;
	//			}
	//			//如果water不属于任何特定pool
	//			else {
	//				pool = (struct Pool1*)malloc(sizeof(struct Pool1));
	//				pool->water = (int**)malloc(h * sizeof(int*));
	//				for (a = 0; a < h; a++)
	//				{
	//					pool->water[a] = (int*)malloc(w * sizeof(int));
	//				}

	//				for (a = 0; a < h; a++)
	//				{
	//					for (b = 0; b < w; b++)
	//					{
	//						pool->water[a][b] = 0;
	//					}
	//				}
	//				pool->num = 0;
	//				pool->symbel = typenum;
	//				pool->next = NULL;
	//				typenum++;
	//				pool->water[i][j] = 1;
	//				index.end->next = pool;
	//				index.end = pool;
	//			}
	//		}
	//	}
	//}

	count = w * h;
	for (i = 0; i < count; i++)
	{
		sum = sum + *p;
		p++;
	}
	sum = sum / count;
	count = w * h;
	count1 = 0;
	p = (unsigned char*)m_imageGray.GetBits();
	for (i = 0; i < count; i++)
	{
		if (*p < (unsigned char)sum)
		{
			sum1 = sum1 + *p;
			count1++;
		}
		p++;
	}

	sum1 = sum1 / count1;
	p = (unsigned char*)m_imageGray.GetBits();
	while (1)
	{
		found = 0;
		for (i = 100; i < h - 100; i++)
		{
			for (j = 100; j < w - 100; j++)
			{
				if (*(p + i * w + j) < 5)
				{
					state = 0;
					pcur = index.next;
					while (1)
					{
						if (pcur == NULL)
							break;
						if (pcur->water[i][j] == 1)//判断该种子是否已经生长
						{
							state++;
						}
						pcur = pcur->next;
					}
					if (state == 0)//未生长
					{
						pool = (struct Pool1*)malloc(sizeof(struct Pool1));
						pool->water = (int**)malloc(h * sizeof(int*));
						for (a = 0; a < h; a++)
						{
							pool->water[a] = (int*)malloc(w * sizeof(int));
						}

						for (a = 0; a < h; a++)
						{
							for (b = 0; b < w; b++)
							{
								pool->water[a][b] = 0;
							}
						}
						pool->num = 0;
						pool->symbel = typenum;
						pool->next = NULL;
						typenum++;
						index.end->next = pool;
						index.end = pool;
						grow(i, j, index, h, w, sum1);
						found = 1;
					}
				}
			}
		}
		if (found == 0)
			break;
	}
	//for (wh = 6; wh < 15; wh++)
	//{
	//	for (i = 100; i < h - 100; i++)
	//	{
	//		for (j = 100; j < w - 100; j++)
	//		{
	//			if ((*(p + i * w + j) < wh) && (*(p + i * w + j) >= (wh - 1)))
	//			{
	//				state = 0;
	//				pcur = index.next;
	//				while (1)
	//				{
	//					if (pcur == NULL)
	//						break;
	//					if (pcur->water[i - 1][j - 1] == 1 || pcur->water[i - 1][j] == 1 || pcur->water[i - 1][j + 1] == 1
	//						|| pcur->water[i][j - 1] == 1 || pcur->water[i][j + 1] == 1
	//						|| pcur->water[i + 1][j - 1] == 1 || pcur->water[i + 1][j] == 1 || pcur->water[i + 1][j + 1] == 1)//判断该水滴是否在当前水池
	//					{
	//						state++;
	//						belong = pcur;
	//					}
	//					pcur = pcur->next;
	//				}
	//				//如果water同时属于多个pool
	//				if (state >= 1)
	//				{
	//					belong->water[i][j] = 1;
	//				}
	//				//如果water不属于任何特定pool
	//				else {
	//					pool = (struct Pool1*)malloc(sizeof(struct Pool1));
	//					pool->water = (int**)malloc(h * sizeof(int*));
	//					for (a = 0; a < h; a++)
	//					{
	//						pool->water[a] = (int*)malloc(w * sizeof(int));
	//					}

	//					for (a = 0; a < h; a++)
	//					{
	//						for (b = 0; b < w; b++)
	//						{
	//							pool->water[a][b] = 0;
	//						}
	//					}
	//					pool->num = 0;
	//					pool->symbel = typenum;
	//					pool->next = NULL;
	//					typenum++;
	//					pool->water[i][j] = 1;
	//					index.end->next = pool;
	//					index.end = pool;
	//				}
	//			}
	//		}
	//	}
	//}

	for (a = (int)sum1; a < 20; a++)
	{
		pcur = index.next;
		while (1)
		{
			if (pcur == NULL)
				break;
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					if (pcur->water[i][j] > 0)
					{
						growagain1(i, j, index, h, w, pcur, a, a - (int)sum1 + 2);
						//growagain(i, j, index, h, w, pcur);
						break;
					}
				}
			}
			pcur = pcur->next;
		}
	}

	p = (unsigned char*)m_image.GetBits();
	color = 0;
	pcur = index.next;
	while(1)
	{
		if (pcur == NULL)
			break;
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				if (pcur->water[i][j] >= 1)
				{
					*(p + i * w * 3 + j * 3) = color;
					*(p + i * w * 3 + j * 3 + 1) = color;
					*(p + i * w * 3 + j * 3 + 2) = color;
				}
			}
		}
		color += 30;
		pcur = pcur->next;
	}
	pcur = index.next;
	while(1)
	{
		if (pcur == NULL)
			break;
		belong = pcur->next;
		for (i = 0; i < h; i++)
		{
			free(pcur->water[i]);
		}
		free(pcur->water);
		free(pcur);
		pcur = belong;
	}
}

int CdemoDlg::Waterinpool(struct Pool pool, int h, int w)
{
	struct Water* water;
	double distance;
	//water = pool.edge;
	water = pool.poolwater;

	while (1)
	{
		if (water == NULL)
			break;
		distance = sqrt((water->h - h) * (water->h - h) + (water->w - w) * (water->w - w));
		if (distance < 1.42)
		{
			return 1;
		}
		water = water->next;
	}
	return 0;
}

//void CdemoDlg::replacepool(struct Pool* pool)
//{
//	struct Pool* pcur = pool->nextpool;
//	while (1)
//	{
//		if (pcur == NULL)
//		{
//			break;
//		}
//		pcur->endwater->next = pcur->newwater;
//		pcur->newwater = pcur->edge;
//		pcur->endwater = pcur->endnew;
//		pcur->endnew = pcur->edgecur;
//		pcur->edge = pcur->edgecur;
//		pcur->edgecur = pcur->edge;
//		pcur = pcur->nextpool;
//	}
//}

void CdemoDlg::Displayedge(struct Pool* fpool)
{
	struct Pool* pcur = NULL;
	struct Water* wcur = NULL;
	unsigned char* p = (unsigned char*)m_image.GetBits();
	int h, w;

	w = m_image.GetWidth();
	h = m_image.GetHeight();

	if (fpool == NULL)
		exit(1);
	pcur = fpool->nextpool;
	while (1)
	{
		if (pcur == NULL)
			break;
		wcur = pcur->poolwater;
		while (1)
		{
			if (wcur == NULL)
				break;
			*(p + wcur->h * w * 3 + wcur->w * 3) = 0;
			*(p + wcur->h * w * 3 + wcur->w * 3 + 1) = 255;
			*(p + wcur->h * w * 3 + wcur->w * 3 + 2) = 0;
			wcur = wcur->next;
		}
		pcur = pcur->nextpool;
	}
}

void CdemoDlg::grow(int h1, int w1, struct Pool1 index, int h0, int w0, int th)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	int h, w;

	index.end->water[h1][w1] = 1;
	index.end->num++;
	vector<int>obj;
	obj.push_back(h1);
	obj.push_back(w1);
	while (1)
	{
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		if (index.end->water[h - 1][w - 1] == 0 && *(p + (h - 1) * w0 + (w - 1)) < th)
		{
			index.end->water[h - 1][w - 1] = 1;
			index.end->num++;
			obj.push_back(h - 1);
			obj.push_back(w - 1);
		}
		if (index.end->water[h - 1][w] == 0 && *(p + (h - 1) * w0 + (w)) < th)
		{
			index.end->water[h - 1][w] = 1;
			index.end->num++;
			obj.push_back(h - 1);
			obj.push_back(w);
		}
		if (index.end->water[h - 1][w + 1] == 0 && *(p + (h - 1) * w0 + (w + 1)) < th)
		{
			index.end->water[h - 1][w + 1] = 1;
			index.end->num++;
			obj.push_back(h - 1);
			obj.push_back(w + 1);
		}

		if (index.end->water[h][w - 1] == 0 && *(p + (h)*w0 + (w - 1)) < th)
		{
			index.end->water[h][w - 1] = 1;
			index.end->num++;
			obj.push_back(h);
			obj.push_back(w - 1);
		}
		if (index.end->water[h][w + 1] == 0 && *(p + (h)*w0 + (w + 1)) < th)
		{
			index.end->water[h][w + 1] = 1;
			index.end->num++;
			obj.push_back(h);
			obj.push_back(w + 1);
		}

		if (index.end->water[h + 1][w - 1] == 0 && *(p + (h + 1) * w0 + (w - 1)) < th)
		{
			index.end->water[h + 1][w - 1] = 1;
			index.end->num++;
			obj.push_back(h + 1);
			obj.push_back(w - 1);
		}
		if (index.end->water[h + 1][w] == 0 && *(p + (h + 1) * w0 + (w)) < th)
		{
			index.end->water[h + 1][w] = 1;
			index.end->num++;
			obj.push_back(h + 1);
			obj.push_back(w);
		}
		if (index.end->water[h + 1][w + 1] == 0 && *(p + (h + 1) * w0 + (w + 1)) < th)
		{
			index.end->water[h + 1][w + 1] = 1;
			index.end->num++;
			obj.push_back(h + 1);
			obj.push_back(w + 1);
		}
		if (obj.empty())
			break;
	}
	
}

void CdemoDlg::growagain(int h1, int w1, struct Pool1 index, int h0, int w0, struct Pool1* pool)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	struct Pool1* pcur = NULL;
	int h, w;
	int state;

	pool->water[h1][w1] = 2;
	vector<int>obj;
	obj.push_back(h1);
	obj.push_back(w1);
	while (1)
	{
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		if (pool->water[h - 1][w - 1] < 2 && *(p + (h - 1) * w0 + (w - 1)) < th1)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h - 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h - 1][w - 1] = 2;
				obj.push_back(h - 1);
				obj.push_back(w - 1);
			}
		}
		if (pool->water[h - 1][w] < 2 && *(p + (h - 1) * w0 + (w)) < th1)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h - 1][w] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h - 1][w] = 2;
				obj.push_back(h - 1);
				obj.push_back(w);
			}
		}
		if (pool->water[h - 1][w + 1] < 2 && *(p + (h - 1) * w0 + (w + 1)) < th1)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h - 1][w + 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h - 1][w + 1] = 2;
				obj.push_back(h - 1);
				obj.push_back(w + 1);
			}
		}

		if (pool->water[h][w - 1] < 2 && *(p + (h)*w0 + (w - 1)) < th1)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h][w - 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h][w - 1] = 2;
				obj.push_back(h);
				obj.push_back(w - 1);
			}
		}
		if (pool->water[h][w + 1] < 2 && *(p + (h)*w0 + (w + 1)) < th1)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h][w + 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h][w + 1] = 2;
				obj.push_back(h);
				obj.push_back(w + 1);
			}
		}

		if (pool->water[h + 1][w - 1] < 2 && *(p + (h + 1) * w0 + (w - 1)) < th1)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h + 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h + 1][w - 1] = 2;
				obj.push_back(h + 1);
				obj.push_back(w - 1);
			}
		}
		if (pool->water[h + 1][w] < 2 && *(p + (h + 1) * w0 + (w)) < th1)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h + 1][w] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h + 1][w] = 2;
				obj.push_back(h + 1);
				obj.push_back(w);
			}
		}
		if (pool->water[h + 1][w + 1] < 2 && *(p + (h + 1) * w0 + (w + 1)) < th1)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h + 1][w + 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h + 1][w + 1] = 2;
				obj.push_back(h + 1);
				obj.push_back(w + 1);
			}
		}
		if (obj.empty())
			break;
	}
}

void CdemoDlg::growagain1(int h1, int w1, struct Pool1 index, int h0, int w0, struct Pool1* pool, int Th, int wh)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	struct Pool1* pcur = NULL;
	int h, w, i ,j;
	int state;

	pool->water[h1][w1] = 2;
	vector<int>obj;
	obj.push_back(h1);
	obj.push_back(w1);
	while (1)
	{
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		if (pool->water[h - 1][w - 1] < wh && *(p + (h - 1) * w0 + (w - 1)) < Th)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h - 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h - 1][w - 1] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w - 1);
			}
		}
		if (pool->water[h - 1][w] < wh && *(p + (h - 1) * w0 + (w)) < Th)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h - 1][w] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h - 1][w] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w);
			}
		}
		if (pool->water[h - 1][w + 1] < wh && *(p + (h - 1) * w0 + (w + 1)) < Th)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h - 1][w + 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h - 1][w + 1] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w + 1);
			}
		}

		if (pool->water[h][w - 1] < wh && *(p + (h)*w0 + (w - 1)) < Th)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h][w - 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h][w - 1] = wh;
				pool->num++;
				obj.push_back(h);
				obj.push_back(w - 1);
			}
		}
		if (pool->water[h][w + 1] < wh && *(p + (h)*w0 + (w + 1)) < Th)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h][w + 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h][w + 1] = wh;
				pool->num++;
				obj.push_back(h);
				obj.push_back(w + 1);
			}
		}

		if (pool->water[h + 1][w - 1] < wh && *(p + (h + 1) * w0 + (w - 1)) < Th)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h + 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h + 1][w - 1] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w - 1);
			}
		}
		if (pool->water[h + 1][w] < wh && *(p + (h + 1) * w0 + (w)) < Th)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h + 1][w] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h + 1][w] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w);
			}
		}
		if (pool->water[h + 1][w + 1] < wh && *(p + (h + 1) * w0 + (w + 1)) < Th)
		{
			pcur = index.next;
			state = 1;
			while (1) {
				if (pcur == NULL)
					break;
				if (pcur->water[h + 1][w + 1] >= 1)
				{
					state = 0;
					break;
				}
				pcur = pcur->next;
			}
			if (state == 1) {
				pool->water[h + 1][w + 1] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w + 1);
			}
		}
		if (obj.empty())
			break;
	}
}