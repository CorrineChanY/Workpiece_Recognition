﻿
// demoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"
#include "digital.h"



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
	//ON_BN_CLICKED(IDC_BUTTON1, &CdemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(startRecg, &CdemoDlg::OnBnClickedstartrecg)
	//ON_BN_CLICKED(IDC_BUTTON3, &CdemoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(endRecg, &CdemoDlg::OnBnClickedendrecg)
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
		MessageBox(_T(" 没 有 找 到 相 机 , 请确认连接和相机 IP 设 置 "), _T(" 提 示"),MB_ICONWARNING);
			return;
	}
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox(_T(" 无 法 打 开 相 机 ， 可 能 正 被 别 的 软 件 控 制 "), _T(" 提 示 "),MB_ICONWARNING);
		else
			MessageBox(_T("无法打开相机"), _T("提示"), MB_ICONWARNING);
		return;
	}
	int w, h;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &m_PixelFormat);
	m_image.CreateByPixelFormat(w, h, m_PixelFormat);
	m_imageGray.CreateByPixelFormat(w, h, PixelFormat_Mono8);
	m_imageDid.CreateByPixelFormat(w, h, PixelFormat_Mono8);
	m_imageLUV.CreateByPixelFormat(w, h, m_PixelFormat);
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(true);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);

}
void CdemoDlg::DrawImage()
{
	CRect rct;//长方形窗口数据
	GetDlgItem(pic)->GetClientRect(&rct); // rec是ID为pic的显示框
	//GetClientRect 用于取得指定窗口的客户区域大小
	//GetDlgItem 假设一个父窗口中有多个子窗口。那么本函数是返回一个子窗口句柄。
	int dstW = rct.Width();//获得长方体窗口的宽度
	int dstH = rct.Height();//获得长方体窗口的高度
	CDC* pDC = GetDlgItem(pic)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		m_imageGray.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
	}
	ReleaseDC(pDC);
	//用GetDC()得到的DC, 必须调用ReleaseDC()
}

void CdemoDlg::DrawImage2()
{
	CRect rct;//长方形窗口数据
	GetDlgItem(picd)->GetClientRect(&rct);
	//GetClientRect 用于取得指定窗口的客户区域大小
	//GetDlgItem 假设一个父窗口中有多个子窗口。那么本函数是返回一个子窗口句柄。
	int dstW = rct.Width();//获得长方体窗口的宽度
	int dstH = rct.Height();//获得长方体窗口的高度
	CDC* pDC = GetDlgItem(picd)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		m_imageGray.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
	}
	ReleaseDC(pDC);
	//用GetDC()得到的DC, 必须调用ReleaseDC()
}

int CdemoDlg::OnStreamCB(MV_IMAGE_INFO* pInfo)
{
	Digital digi;
	unsigned char* p = (unsigned char*)m_image.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageGray.GetBits();
	int w, h;
	MVInfo2Image(m_hCam, pInfo, &m_image);
	w = m_image.GetWidth();
	h = m_image.GetHeight();
	MVBGRToGray(m_hCam, p, pDst, w, h);
	if(Recgon == 1)
	{
		//digi.Image_(m_image, m_imageDid);
		Image_Gray();
		ImageBRGToLUV();
	}
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
	Recgon = 0;
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


void CdemoDlg::OnBnClickedstartrecg()//打开识别按钮
{
	// TODO: 在此添加控件通知处理程序代码
	Recgon = 1;
}


void CdemoDlg::OnBnClickedendrecg()//关闭识别按钮
{
	// TODO: 在此添加控件通知处理程序代码
	Recgon = 0;
}

void CdemoDlg::Image_Gray()
{
	int w;
	int h;
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageGray.GetBits();
	double sum = 0;
	double count = 0;
	int i, j;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();
	//Kittle 二值分割
	count = w * h;
	for (i = 0; i < count; i++)
	{
		sum = sum + *p;
		p++;
		
	}
	
	sum = sum / count;
	
	
	p = (unsigned char*)m_imageGray.GetBits();
	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			//if (*p > sum)
			if (*p > 100)
			{
				*pDst = 255;
				pDst++;
				p++;
			}
			else {
				*pDst = 0;
				pDst++;
				p++;
			}
		}
	}
	//滤波
	/*
	pDst = (unsigned char*)m_imageGray.GetBits();
	pDst = pDst + w + 1;
	for (j = 1; j < h - 1; j++)
	{
		for (i = 1; i < w - 1; i++)
		{
				//*pDst = Median(*pDst, *(pDst-3), *(pDst+3),*(pDst-3*w), *(pDst+3*w), *(pDst-3*w-3),*(pDst-3*w+3), *(pDst+3*w-3), *(pDst+3*w+3));
				*pDst = (*pDst + *(pDst - 1) + *(pDst + 1) + *(pDst - w) + *(pDst + w) + *(pDst - w - 1) + *(pDst - w + 1) + *(pDst + w - 1) + *(pDst + w + 1)) / 9;
				pDst++;
			}
		}
	}*/
	DrawImage2();
	p = (unsigned char*)m_imageGray.GetBits();
	pDst = (unsigned char*)m_imageDid.GetBits();
	for (i = 0; i < h; i++)
	{
		for (j = 1; j < w; j++)
		{
				*pDst = *p;
				pDst++;
				p++;
		}
	}
	Corrode();
	Corrode();
	Expand();
	Expand();
	

}

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

void CdemoDlg::Corrode()
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	unsigned char* pcur;
	int i, j, k;
	int w, h;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	for (i = 2; i < h - 2; i++)
	{
		for (j = 2; j < w - 2; j++)
		{
			pcur = p + i * w + j;

			if (*(pcur - 1) == 255 || *(pcur + 1) == 255 ||
				*(pcur - w) == 255 || *(pcur + w) == 255 ||
				*(pcur - w - 1) == 255 || *(pcur - w + 1) == 255 ||
				*(pcur + w - 1) == 255 || *(pcur + w + 1) == 255 ||
				*(pcur - 2) == 255 || *(pcur + 2) == 255 ||
				*(pcur - 2 * w) == 255 || *(pcur + 2 * w) == 255 || 
				*(pcur - w - 2) == 255 || *(pcur - w + 2) == 255 ||
				*(pcur + w -2) == 255 || *(pcur + w + 2) == 255)
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

void CdemoDlg::Expand()
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	unsigned char* pcur;
	int i, j, k;
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


void CdemoDlg::ImageBRGToLUV()
{
	unsigned char* p = (unsigned char*)m_image.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageLUV.GetBits();
	//int i, j, k;
	int w, h;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
		{
			unsigned short var_R, var_G, var_B;
			unsigned short var_U, var_V;
			unsigned short X, Y, Z;
			// 把RGB转成XYZ
			var_R = *(p + i * w * 3) / 255;
			var_G = *(p + i * w * 3 + 1) / 255;
			var_B = *(p + i * w * 3 + 2) / 255;
			if (var_R > 0.04045){
				var_R = pow((var_R + 0.055) / 1.055, 2.4);
			} else {
				var_R = var_R / 12.92;
			}
			if (var_G > 0.04045) {
				var_G = pow((var_G + 0.055) / 1.055, 2.4);
			}
			else {
				var_G = var_G / 12.92;
			}
			if (var_B > 0.04045){
				var_B = pow((var_B + 0.055) / 1.055, 2.4);
			} else {
				var_B = var_B / 12.92;
			}
			var_R = var_R * 100;
			var_G = var_G * 100;
			var_B = var_B * 100;
			X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
			Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
			Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

			// XYZ转成LUV
			var_U = (4 * X) / (X + (15 * Y) + (3 * Z));
			var_V = (9 * Y) / (X + (15 * Y) + (3 * Z));
			Y = Y / 100;
			if (Y > 0.008856) Y = pow(Y, (double)(1 / 3));
			else Y = 7.787 * Y + (double)(16 / 116);

			unsigned short ref_X = 95.047, ref_Y = 100.000, ref_Z = 108.883;
			unsigned short ref_U = (4 * ref_X) / (ref_X + (15 * ref_Y) + (3 * ref_Z));
			unsigned short ref_V = (9 * ref_Y) / (ref_X + (15 * ref_Y) + (3 * ref_Z));

			unsigned short L = (116 * Y) - 16;
			unsigned short U = 13 * L * (var_U - ref_U);
			unsigned short V = 13 * L * (var_V - ref_V);

			*(pDst + i * w * 3) = L;
			*(pDst + i * w * 3 + 1) = U;
			*(pDst + i * w * 3 + 2) = V;
		}
	}
}