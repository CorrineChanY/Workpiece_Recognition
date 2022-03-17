
// demoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <iostream>
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
	GetDlgItem(IDC_OpenCam)->EnableWindow(false); // 暂时变成不可点击
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

// “打开相机”点击事件
void CdemoDlg::OnBnClickedOpencam()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		MessageBox(_T("没有找到相机, 请确认连接和相机IP设置 "), _T("提示"),MB_ICONWARNING);
			return;
	}
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox(_T("无法打开相机，可能正被别的软件控制 "), _T("提示 "),MB_ICONWARNING);
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

// “开始采集”点击事件
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
	MVStartGrab(m_hCam, (MVStreamCB)StreamCB, (ULONG_PTR)this);
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

// “开始识别”点击事件
void CdemoDlg::OnBnClickedstartrecg()
{
	//img = m_image.Load((LPCTSTR)"../photo/photo/1.bmp"); // 这个自带的方法不行
	int wid = 0, hgt = 0;
	int len = 0, offset = 0;
	char temp[4] = { 0 };
	
	// 1. 打开图片文件
	ifstream img("../photo/photo/1.bmp", ifstream::in | ios::binary);
	if (!img) {
		cout << "Can not open this image!" << endl;
		return;
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
	m_image.CreateByPixelFormat(wid, hgt, PixelFormat_BayerRG8);
	char* p = (char*)m_image.GetBits();
	img.read(p, wid * hgt * 3); // 这样读出来是上下颠倒的
	img.close();

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


void CdemoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
