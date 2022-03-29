
// demoDlg.cpp: 实现文件

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"
#include "digital.h"
#include "disNums.h"
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string> 
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
	m_imageGray.CreateByPixelFormat(w, h, PixelFormat_Mono8); // 灰度
	m_imageDid.CreateByPixelFormat(w, h, PixelFormat_Mono8); // 灰度
	m_imageDid1.CreateByPixelFormat(w, h, PixelFormat_Mono8); // 灰度
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

//void CdemoDlg::DrawImageGray()
//{
//	CRect rct;
//	GetDlgItem(picGray)->GetClientRect(&rct);
//	int dstW = rct.Width();
//	int dstH = rct.Height();
//	CDC* pDC = GetDlgItem(picGray)->GetDC();
//	{
//		pDC->SetStretchBltMode(BLACKONWHITE);
//		m_imageGray.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
//	}
//	ReleaseDC(pDC);
//}

int CdemoDlg::OnStreamCB(MV_IMAGE_INFO* pInfo)
{

	MVInfo2Image(m_hCam, pInfo, &m_image);
	if (Recgon == 1)
	{
		Change_Image();
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
	ifstream img("../photo/photo/4.bmp", ifstream::in | ios::binary); // 默认打开的是1.bmp
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
	m_imageDid1.CreateByPixelFormat(wid, hgt, PixelFormat_Mono8); // 灰度
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
	//vector<struct Pool1> index = NULL;//单一工件图链表起始节点，读取工件图从index->next开始读起

	/*vector<struct Pool1> index;*/
	//readBMP();
	//DrawImage();
	//Image_Gray();
	////DrawImageGray();
	//Watershed(index);//获取单一工件图链表
	////simplar_susan(index);//获取工件边缘
	////Dispool(index);//在原图中标记工件
	//Disedge(index);//在原图上标记边界
	//JudgePiece(index);//判断工件类型
	//freepool(index);
	////Corrode();
	////Corrode();
	////Expand();
	////Expand();

	////////Susan();

	////DrawImage();
	////DrawImageGray();
	Recgon = 1;
}

void CdemoDlg::reset() {
	for (int i = 0; i < 20; i++)
	{
		GetDlgItem(1018 + i)->SetWindowTextW(L"");
		GetDlgItem(1038 + i)->SetWindowTextW(L"");
	}
}

void CdemoDlg::Change_Image()
{
	vector<struct Pool1> index;

	Image_Gray();
	//DrawImageGray();
	if (Watershed(index))//获取单一工件图链表
	{
		//simplar_susan(index);//获取工件边缘
		//Dispool(index);//在原图中标记工件
		Disedge(index);//在原图上标记边界
		JudgePiece(index);//判断工件类型
		freepool(index);
	}
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
	unsigned char* pDstd = (unsigned char*)m_imageDid.GetBits();
	double sum = 0;
	double sum1 = 0;
	double count = 0;
	int i, j;

	w = m_image.GetWidth();
	h = m_image.GetHeight();
	p = (unsigned char*)m_image.GetBits();

	ofstream out("output.txt");

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*pDst = 0.299 * *p + 0.587 * *(p + 1) + 0.114 * *(p + 2);
			*pDstd = *pDst;
			pDst++;
			pDstd++;
			p += 3;
		}
	}
	pDst = (unsigned char*)m_imageGray.GetBits();
	pDstd = (unsigned char*)m_imageDid.GetBits();
	for (i = 0; i < w; i++)
	{
		*(pDst + i) = 255;
		*(pDst + (h - 1)*w + i) = 255;
		*(pDstd + i) = 255;
		*(pDstd + (h - 1) * w + i) = 255;
	}
	for (i = 0; i < h; i++)
	{
		*(pDst + i * w) = 255;
		*(pDst + i * w + w - 1) = 255;
		*(pDstd + i * w) = 255;
		*(pDstd + i * w + w - 1) = 255;
	}
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
void CdemoDlg::Corrode(int num, int th)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	unsigned char* pcur = NULL;
	unsigned char* pool = (unsigned char*)m_imageDid1.GetBits();;
	int i, j, n;
	int w, h;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*(pool + i * w + j) = *(p + i * w + j);
		}
	}

	/*for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			if (*(pool + i * w + j) > th + 1)
				*(pool + i * w + j) = 255;
		}
	}*/

	if (num > 0)
	{
		for (n = 0; n < num; n++)
		{
			for (i = 2; i < h - 2; i++)
			{
				for (j = 2; j < w - 2; j++)
				{
					pcur = pool + i * w + j;
					/*if ((*(pcur - 1) > th0 || *(pcur + 1) > th0 ||
						*(pcur - w) > th || *(pcur + w) > th0 ||
						*(pcur - w - 1) > th0 || *(pcur - w + 1) > th0 ||
						*(pcur + w - 1) > th0 || *(pcur + w + 1) > th0 ||
						*(pcur - 2) > th0 || *(pcur + 2) > th0 ||
						*(pcur - 2 * w) > th0 || *(pcur + 2 * w) > th0 ||
						*(pcur - w - 2) > th0 || *(pcur - w + 2) > th0 ||
						*(pcur + w - 2) > th0 || *(pcur + w + 2) > th0) && *pcur != 255)*/
					if (*(pcur - 1) > th || *(pcur + 1) > th ||
						*(pcur - w) > th || *(pcur + w) > th ||
						*(pcur - w - 1) > th || *(pcur - w + 1) > th ||
						*(pcur + w - 1) > th || *(pcur + w + 1) > th)
					{
						*(pDst + i * w + j) = 255;
					}
				}
			}
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					*(pool + i * w + j) = *(pDst + i * w + j);
				}
			}
		}
	}
	/*p = (unsigned char*)m_imageGray.GetBits();
	pDst = (unsigned char*)m_imageDid.GetBits();
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*p = *pDst;
			pDst++;
			p++;
		}
	}*/
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

//Watershed基于区域生长的分水岭算法
int CdemoDlg::Watershed(vector<struct Pool1> &index)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	int w, h;
	int i, j, a, b, wh, count, count1, c, d;
	double sum = 0;
	double sum1 = 0;
	//vector<struct Pool1> index;//各工件图片队列的首节点，自身不保存工件信息
	struct Pool1 pool;
	struct Pool1* pcur = NULL;
	struct Pool1* belong = NULL;
	int state = 0;
	int typenum = 1;
	int found = 0;
	int color;
	int poolnum = 0;
	struct Pool1* poolall = NULL;
	int allnum = 0;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	p = (unsigned char*)m_imageGray.GetBits();

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
	Corrode(4, 40);
	sum1 = sum1 / count1;
	pDst = (unsigned char*)m_imageDid.GetBits();
	while (1)
	{
		found = 0;
		for (i = 100; i < h - 100; i++)
		{
			for (j = 100; j < w - 100; j++)
			{
				if (*(pDst + i * w + j) < 25)
				{
					state = 0;
					for (auto it = index.begin(); it != index.end(); ++it)
					{
						if ((*it).water[i][j] == 1)
						{
							state++;
						}
					}
					if (state == 0)//未生长
					{
						allnum++;
						if (allnum > 25)
						{
							freepool(index);
							index.swap(vector<struct Pool1>());
							return 0;
						}
						pool.symbel = typenum;
						typenum++;
						index.push_back(pool);
						grow(i, j, index, h, w, 30);
						found = 1;
					}
				}
			}
		}
		if (found == 0)
			break;
	}

	for (auto it = index.begin(); it != index.end(); it++)
	{
		if ((*it).num > 100000)
		{
			freepool(index);
			index.swap(vector<struct Pool1>());
			return 0;
		}
	}

	poolnum = index.size();
	poolall = (struct Pool1*)malloc(poolnum * sizeof(struct Pool1));

	for (a = 31; a < 45; a++)
	{
		c = 0;
		for (auto it1 = index.begin(); it1 != index.end(); it1++)
		{
			poolall[c] = (*it1);
			c++;
		}
		c = 0;
		for(auto it = index.begin(); it != index.end(); it++)
		{
			state = 0;
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					if (poolall[c].water[i][j] > 0)
					{
						growagain1(i, j, index, h, w, &(*(it)), a, a);
						state = 1;
						break;
					}
				}
				if (state == 1)
					break;
			}
			c++;
		}
	}

	c = 0;
	for (auto it1 = index.begin(); it1 != index.end(); it1++)
	{
		poolall[c] = (*it1);
		c++;
	}

	c = 0;
	for (auto it = index.begin(); it != index.end(); it++)
	{
		state = 0;
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				if (poolall[c].water[i][j] >= 1)
				{
					if (poolall[c].water[i + 2][j] >= 1)
					{
						grow_susan(i + 2, j, index, h, w, &(*(it)), a - 1, a);
						state = 1;
						break;
					}
				}
			}
			if (state == 1)
				break;
		}
		c++;
	}
	free(poolall);
	return 1;
}

//显示水池
void CdemoDlg::Dispool(vector<struct Pool1> index)
{
	unsigned char* p = (unsigned char*)m_image.GetBits();
	int h, w, i, j, a;
	int color = 60;
	struct Pool1* pcur = NULL;
	int poolnum = 0;
	struct Pool1* poolall = NULL;

	poolnum = index.size();
	poolall = (struct Pool1*)malloc(poolnum * sizeof(struct Pool1));
	i = 0;
	for (auto it = index.begin(); it != index.end(); it++)
	{
		poolall[i] = (*it);
		i++;
	}

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	for (a = 0; a < poolnum; a++)
	{
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				if (poolall[a].water[i][j] >= 1)
				{
					*(p + i * w * 3 + j * 3) = 0;
					*(p + i * w * 3 + j * 3 + 1) = color;
					*(p + i * w * 3 + j * 3 + 2) = 0;
				}
			}
		}
		color += 15;
	}
	free(poolall);
}

//删库跑路
void CdemoDlg::freepool(vector<struct Pool1> index)
{
	int i, h;
	int** pool;
	h = m_imageGray.GetHeight();
	for (auto it = index.begin(); it != index.end(); it++)
	{
		pool = (*it).water;
		for (i = 0; i < h; i++)
		{
			free(pool[i]);
		}
		free(pool);
	}
}

//一次增长获得水洼
void CdemoDlg::grow(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, int th)
{
	unsigned char* p = (unsigned char*)m_imageDid.GetBits();
	struct Pool1 pool;
	int h, w, a, b;
	int num = index.size() - 1;

	pool.symbel = index[num].symbel;
	pool.water = (int**)malloc(h0 * sizeof(int*));
	for (a = 0; a < h0; a++)
	{
		pool.water[a] = (int*)malloc(w0 * sizeof(int));
	}

	for (a = 0; a < h0; a++)
	{
		for (b = 0; b < w0; b++)
		{
			pool.water[a][b] = 0;
		}
	}
	pool.num = 0;
	auto it = index.end() - 1;
	vector<int>obj;
	obj.push_back(h1);
	obj.push_back(w1);
	while (1)
	{
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		if (pool.water[h - 1][w - 1] == 0 && *(p + (h - 1) * w0 + (w - 1)) < th)
		{
			pool.water[h - 1][w - 1] = 1;
			pool.num++;
			obj.push_back(h - 1);
			obj.push_back(w - 1);
		}
		if (pool.water[h - 1][w] == 0 && *(p + (h - 1) * w0 + (w)) < th)
		{
			pool.water[h - 1][w] = 1;
			pool.num++;
			obj.push_back(h - 1);
			obj.push_back(w);
		}
		if (pool.water[h - 1][w + 1] == 0 && *(p + (h - 1) * w0 + (w + 1)) < th)
		{
			pool.water[h - 1][w + 1] = 1;
			pool.num++;
			obj.push_back(h - 1);
			obj.push_back(w + 1);
		}

		if (pool.water[h][w - 1] == 0 && *(p + (h)*w0 + (w - 1)) < th)
		{
			pool.water[h][w - 1] = 1;
			pool.num++;
			obj.push_back(h);
			obj.push_back(w - 1);
		}
		if (pool.water[h][w + 1] == 0 && *(p + (h)*w0 + (w + 1)) < th)
		{
			pool.water[h][w + 1] = 1;
			pool.num++;
			obj.push_back(h);
			obj.push_back(w + 1);
		}

		if (pool.water[h + 1][w - 1] == 0 && *(p + (h + 1) * w0 + (w - 1)) < th)
		{
			pool.water[h + 1][w - 1] = 1;
			pool.num++;
			obj.push_back(h + 1);
			obj.push_back(w - 1);
		}
		if (pool.water[h + 1][w] == 0 && *(p + (h + 1) * w0 + (w)) < th)
		{
			pool.water[h + 1][w] = 1;
			pool.num++;
			obj.push_back(h + 1);
			obj.push_back(w);
		}
		if (pool.water[h + 1][w + 1] == 0 && *(p + (h + 1) * w0 + (w + 1)) < th)
		{
			pool.water[h + 1][w + 1] = 1;
			pool.num++;
			obj.push_back(h + 1);
			obj.push_back(w + 1);
		}
		if (obj.empty())
			break;
	}
	index.pop_back();
	index.push_back(pool);
}

//用于循环增长水平面稳定获得水池
void CdemoDlg::growagain1(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, struct Pool1* pool, int Th, int wh)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	struct Pool1* pcur = NULL;
	int h, w, i ,j;
	int state;
	int num, bar, poolnum;
	auto it = index.begin();
	struct Pool1* poolall = NULL;
	poolnum = index.size() - 1;
	i = 0;
	poolall = (struct Pool1*)malloc((index.size() - 1) * sizeof(struct Pool1));
	for (it = index.begin(); (*it).symbel != pool->symbel; it++)
	{
		poolall[i] = (*it);
		i++;
	}
	it++;
	for (; it != index.end(); it++)
	{
		poolall[i] = (*it);
		i++;
	}
	pool->water[h1][w1] = wh;
	pool->num = 1;
	vector<int>obj;
	obj.push_back(h1);
	obj.push_back(w1);
	while (1)
	{
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		num = 0;
		bar = 0;
		if (pool->water[h - 1][w - 1] < wh && *(p + (h - 1) * w0 + (w - 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h - 1][w - 1] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w - 1);
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h - 1][w] < wh && *(p + (h - 1) * w0 + (w)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h - 1][w] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w);
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h - 1][w + 1] < wh && *(p + (h - 1) * w0 + (w + 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w + 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h - 1][w + 1] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w + 1);
			}
			else {
				bar = 1;
			}
		}

		if (pool->water[h][w - 1] < wh && *(p + (h)*w0 + (w - 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h][w - 1] = wh;
				pool->num++;
				obj.push_back(h);
				obj.push_back(w - 1);
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h][w + 1] < wh && *(p + (h)*w0 + (w + 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h][w + 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h][w + 1] = wh;
				pool->num++;
				obj.push_back(h);
				obj.push_back(w + 1);
			}
			else {
				bar = 1;
			}
		}

		if (pool->water[h + 1][w - 1] < wh && *(p + (h + 1) * w0 + (w - 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h + 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h + 1][w - 1] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w - 1);
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h + 1][w] < wh && *(p + (h + 1) * w0 + (w)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h + 1][w] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h + 1][w] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w);
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h + 1][w + 1] < wh && *(p + (h + 1) * w0 + (w + 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h + 1][w + 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h + 1][w + 1] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w + 1);
			}
			else {
				bar = 1;
			}
		}
		if (bar == 1)
		{
			for (i = 0; i < num; i++)
			{
				//obj.pop_back();
				//obj.pop_back();
			}
		}
		if (obj.empty())
			break;
	}
	pool->num;
	free(poolall);
}

void CdemoDlg::grow_susan(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, struct Pool1* pool, int Th, int wh)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	struct Pool1* pcur = NULL;
	int h, w, i, j;
	int state, susan;
	int num, bar;
	int backnum, poolnum;
	int m = 3, n = 3;
	auto it = index.begin();
	struct Pool1* poolall = NULL;
	poolnum = index.size() - 1;
	i = 0;
	poolall = (struct Pool1*)malloc((index.size() - 1) * sizeof(struct Pool1));
	for (it = index.begin(); (*it).symbel != pool->symbel; it++)
	{
		poolall[i] = (*it);
		i++;
	}
	it++;
	for (; it != index.end(); it++)
	{
		poolall[i] = (*it);
		i++;
	}

	pool->water[h1][w1] = wh;
	pool->wmin = pool->wmax = w1;
	pool->hmin = pool->hmin = h1;
	vector<int>obj;
	obj.push_back(h1);
	obj.push_back(w1);
	pool->num = 1;
	pool->edgenum = 0;
	pool->h0 = 0;
	pool->w0 = 0;
	while (1)
	{
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		num = 0;
		bar = 0;
		backnum = 0;
		if (pool->water[h - 1][w - 1] < wh && *(p + (h - 1) * w0 + (w - 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h - 1][w - 1] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w - 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						//susan += *(p + (h0 - 1 + i) * w + w0 - 1 + n) >= 1;
						susan += pool->water[h - 2 + i][w - 2 + j] >= 1;
					}
				}
				susan = 9 - susan;
				if (susan >= 1)
				{
					pool->edge[pool->edgenum * 2] = h - 1;
					pool->edge[pool->edgenum * 2 + 1] = w - 1;
					pool->edgenum++;
				}
				pool->h0 += h - 1;
				pool->w0 += w - 1;
				if (pool->wmin > w - 1) pool->wmin = w - 1;
				if (pool->wmax < w - 1) pool->wmax = w - 1;
				if (pool->hmin > h - 1) pool->hmin = h - 1;
				if (pool->hmax < h - 1) pool->hmax = h - 1;
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h - 1][w] < wh && *(p + (h - 1) * w0 + (w)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h - 1][w] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						//susan += *(p + (h0 - 1 + i) * w + w0 - 1 + n) >= 1;
						susan += pool->water[h - 2 + i][w - 1 + j] >= 1;
					}
				}
				susan = 9 - susan;
				if (susan >= 1)
				{
					pool->edge[pool->edgenum * 2] = h - 1;
					pool->edge[pool->edgenum * 2 + 1] = w;
					pool->edgenum++;
				}
				pool->h0 += h - 1;
				pool->w0 += w;
				if (pool->wmin > w - 1) pool->wmin = w;
				if (pool->wmax < w - 1) pool->wmax = w;
				if (pool->hmin > h - 1) pool->hmin = h - 1;
				if (pool->hmax < h - 1) pool->hmax = h - 1;
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h - 1][w + 1] < wh && *(p + (h - 1) * w0 + (w + 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w + 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h - 1][w + 1] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w + 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						//susan += *(p + (h0 - 1 + i) * w + w0 - 1 + n) >= 1;
						susan += pool->water[h - 2 + i][w + j] >= 1;
					}
				}
				susan = 9 - susan;
				if (susan >= 1)
				{
					pool->edge[pool->edgenum * 2] = h - 1;
					pool->edge[pool->edgenum * 2 + 1] = w + 1;
					pool->edgenum++;
				}
				pool->h0 += h - 1;
				pool->w0 += w + 1;
				if (pool->wmin > w - 1) pool->wmin = w + 1;
				if (pool->wmax < w - 1) pool->wmax = w + 1;
				if (pool->hmin > h - 1) pool->hmin = h - 1;
				if (pool->hmax < h - 1) pool->hmax = h - 1;
			}
			else {
				bar = 1;
			}
		}

		if (pool->water[h][w - 1] < wh && *(p + (h)*w0 + (w - 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h][w - 1] = wh;
				pool->num++;
				obj.push_back(h);
				obj.push_back(w - 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						//susan += *(p + (h0 - 1 + i) * w + w0 - 1 + n) >= 1;
						susan += pool->water[h - 1 + i][w - 2 + j] >= 1;
					}
				}
				susan = 9 - susan;
				if (susan >= 1)
				{
					pool->edge[pool->edgenum * 2] = h;
					pool->edge[pool->edgenum * 2 + 1] = w - 1;
					pool->edgenum++;
				}
				pool->h0 += h;
				pool->w0 += w - 1;
				if (pool->wmin > w - 1) pool->wmin = w - 1;
				if (pool->wmax < w - 1) pool->wmax = w - 1;
				if (pool->hmin > h - 1) pool->hmin = h;
				if (pool->hmax < h - 1) pool->hmax = h;
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h][w + 1] < wh && *(p + (h)*w0 + (w + 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h][w + 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h][w + 1] = wh;
				pool->num++;
				obj.push_back(h);
				obj.push_back(w + 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						//susan += *(p + (h0 - 1 + i) * w + w0 - 1 + n) >= 1;
						susan += pool->water[h - 1 + i][w + j] >= 1;
					}
				}
				susan = 9 - susan;
				if (susan >= 1)
				{
					pool->edge[pool->edgenum * 2] = h;
					pool->edge[pool->edgenum * 2 + 1] = w + 1;
					pool->edgenum++;
				}
				pool->h0 += h;
				pool->w0 += w + 1;
				if (pool->wmin > w - 1) pool->wmin = w + 1;
				if (pool->wmax < w - 1) pool->wmax = w + 1;
				if (pool->hmin > h - 1) pool->hmin = h;
				if (pool->hmax < h - 1) pool->hmax = h;
			}
			else {
				bar = 1;
			}
		}

		if (pool->water[h + 1][w - 1] < wh && *(p + (h + 1) * w0 + (w - 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h + 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h + 1][w - 1] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w - 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						//susan += *(p + (h0 - 1 + i) * w + w0 - 1 + n) >= 1;
						susan += pool->water[h + i][w - 2 + j] >= 1;
					}
				}
				susan = 9 - susan;
				if (susan >= 1)
				{
					pool->edge[pool->edgenum * 2] = h + 1;
					pool->edge[pool->edgenum * 2 + 1] = w - 1;
					pool->edgenum++;
				}
				pool->h0 += h + 1;
				pool->w0 += w - 1;
				if (pool->wmin > w - 1) pool->wmin = w - 1;
				if (pool->wmax < w - 1) pool->wmax = w - 1;
				if (pool->hmin > h - 1) pool->hmin = h + 1;
				if (pool->hmax < h - 1) pool->hmax = h + 1;
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h + 1][w] < wh && *(p + (h + 1) * w0 + (w)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h + 1][w] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h + 1][w] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						//susan += *(p + (h0 - 1 + i) * w + w0 - 1 + n) >= 1;
						susan += pool->water[h + i][w - 1 + j] >= 1;
					}
				}
				susan = 9 - susan;
				if (susan >= 1)
				{
					pool->edge[pool->edgenum * 2] = h + 1;
					pool->edge[pool->edgenum * 2 + 1] = w;
					pool->edgenum++;
				}
				pool->h0 += h + 1;
				pool->w0 += w;
				if (pool->wmin > w - 1) pool->wmin = w;
				if (pool->wmax < w - 1) pool->wmax = w;
				if (pool->hmin > h - 1) pool->hmin = h + 1;
				if (pool->hmax < h - 1) pool->hmax = h + 1;
			}
			else {
				bar = 1;
			}
		}
		if (pool->water[h + 1][w + 1] < wh && *(p + (h + 1) * w0 + (w + 1)) < Th)
		{
			state = 1;
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h + 1][w + 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			if (state == 1) {
				num++;
				pool->water[h + 1][w + 1] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w + 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						//susan += *(p + (h0 - 1 + i) * w + w0 - 1 + n) >= 1;
						susan += pool->water[h + i][w + j] >= 1;
					}
				}
				susan = 9 - susan;
				if (susan >= 1)
				{
					pool->edge[pool->edgenum * 2] = h + 1;
					pool->edge[pool->edgenum * 2 + 1] = w + 1;
					pool->edgenum++;
				}
				pool->h0 += h + 1;
				pool->w0 += w + 1;
				if (pool->wmin > w - 1) pool->wmin = w + 1;
				if (pool->wmax < w - 1) pool->wmax = w + 1;
				if (pool->hmin > h - 1) pool->hmin = h + 1;
				if (pool->hmax < h - 1) pool->hmax = h + 1;
			}
			else {
				bar = 1;
			}
		}
		if (bar == 1)
		{
			for (i = 0; i < num; i++)
			{
				//obj.pop_back();
				//obj.pop_back();
				//pool->num--;
			}
		}
		if (obj.empty())
			break;
	}
	free(poolall);
	pool->h0 /= pool->num;
	pool->w0 /= pool->num;
}

void CdemoDlg::Disedge(vector<struct Pool1> &index)
{
	unsigned char* p = (unsigned char*)m_image.GetBits();
	int h, w, i, j, po, a, b;
	int h0, w0;
	int hmin, wmin, hmax, wmax;
	int pointnum = 0, poolnum = 0;
	struct Pool1* poolall = NULL;

	poolnum = index.size();
	poolall = (struct Pool1*)malloc(poolnum * sizeof(struct Pool1));
	i = 0;
	for (auto it = index.begin(); it != index.end(); it++)
	{
		poolall[i] = (*it);
		i++;
	}

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	b = 0;
	for (a = 0; a < poolnum; a++)
	{
		if (poolall[a].num > 1000)
		{
			pointnum = poolall[a].edgenum;
			hmin = hmax = poolall[a].edge[0];
			wmin = wmax = poolall[a].edge[1];
			//画边界
			/*for (po = 0; po < pointnum * 2; po++)
			{
				h0 = poolall[a].edge[po];
				po++;
				w0 = poolall[a].edge[po];
				if (w0 < wmin)wmin = w0;
				if (w0 > wmax)wmax = w0;
				if (h0 < hmin)hmin = h0;
				if (h0 > hmax)hmax = h0;
				*(p + h0 * w * 3 + w0 * 3) = 255;
				*(p + h0 * w * 3 + w0 * 3 + 1) = 255;
				*(p + h0 * w * 3 + w0 * 3 + 2) = 255;
			}
			wmin = poolall[a].wmin;
			wmax = poolall[a].wmax;
			hmax = poolall[a].hmax;
			hmin = poolall[a].hmin;*/
			//画框
			for (j = poolall[a].wmin; j <= poolall[a].wmax; j++)
			{
				for (i = 0; i < 3; i++)
				{
					if (poolall[a].hmin > 10 && poolall[a].hmax < h - 10 && poolall[a].wmax < w - 10 && poolall[a].wmin > 10)
					{
						*(p + (poolall[a].hmin - i) * w * 3 + j * 3) = 255;
						*(p + (poolall[a].hmin - i) * w * 3 + j * 3 + 1) = 255;
						*(p + (poolall[a].hmin - i) * w * 3 + j * 3 + 2) = 255;
						*(p + (poolall[a].hmax + i) * w * 3 + j * 3) = 255;
						*(p + (poolall[a].hmax + i) * w * 3 + j * 3 + 1) = 255;
						*(p + (poolall[a].hmax + i) * w * 3 + j * 3 + 2) = 255;
					}
				}
			}
			for (j = poolall[a].hmin; j <= poolall[a].hmax; j++)
			{
				for (i = 0; i < 3; i++)
				{
					if (poolall[a].hmin > 10 && poolall[a].hmax < h - 10 && poolall[a].wmax < w - 10 && poolall[a].wmin > 10)
					{
						*(p + j * w * 3 + (poolall[a].wmin + i) * 3) = 255;
						*(p + j * w * 3 + (poolall[a].wmin + i) * 3 + 1) = 255;
						*(p + j * w * 3 + (poolall[a].wmin + i) * 3 + 2) = 255;
						*(p + j * w * 3 + (poolall[a].wmax - i) * 3) = 255;
						*(p + j * w * 3 + (poolall[a].wmax - i) * 3 + 1) = 255;
						*(p + j * w * 3 + (poolall[a].wmax - i) * 3 + 2) = 255;
					}
				}
			}
			//画几何中心
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < 2; j++)
				{
					*(p + (poolall[a].h0 + i) * w * 3 + (poolall[a].w0 + j) * 3) = 255;
					*(p + (poolall[a].h0 + i) * w * 3 + (poolall[a].w0 + j) * 3 + 1) = 0;
					*(p + (poolall[a].h0 + i) * w * 3 + (poolall[a].w0 + j) * 3 + 2) = 0;
				}
			}

			int x0 = (poolall[a].hmin - 30) > 0 ? poolall[a].hmin - 30 : 0;
			int y0 = (poolall[a].wmin + 15) < w ? poolall[a].wmin : w - 16;
			// 标数字
			if ((b + 1) < 10) {
				for (i = 0; i < 25; i++)
				{
					for (j = 0; j < 15; j++) {
						if (nums[b + 1][i][j] == 1) {
							*(p + (x0 + i) * w * 3 + (y0 + j) * 3) = 255;
							*(p + (x0 + i) * w * 3 + (y0 + j) * 3 + 1) = 255;
							*(p + (x0 + i) * w * 3 + (y0 + j) * 3 + 2) = 255;
						}
					}
				}
			}
			else if (b + 1 < 20)
			{
				int shiwei = 1; // 十位数
				int gewei = b + 1 - 10; // 个位数
				y0 = (wmin + 15 + 20) < w ? wmin : w - 36;
				for (i = 0; i < 25; i++)
				{
					for (j = 0; j < 15; j++) {
						if (nums[shiwei][i][j] == 1) {
							*(p + (x0 + i) * w * 3 + (y0 + j) * 3) = 255;
							*(p + (x0 + i) * w * 3 + (y0 + j) * 3 + 1) = 255;
							*(p + (x0 + i) * w * 3 + (y0 + j) * 3 + 2) = 255;
						}
					}
				}
				for (i = 0; i < 25; i++)
				{
					for (j = 0; j < 15; j++) {
						if (nums[gewei][i][j] == 1) {
							*(p + (x0 + i) * w * 3 + (y0 + 20 + j) * 3) = 255;
							*(p + (x0 + i) * w * 3 + (y0 + 20 + j) * 3 + 1) = 255;
							*(p + (x0 + i) * w * 3 + (y0 + 20 + j) * 3 + 2) = 255;
						}
					}
				}
			}
			b++;
		}
	}
	free(poolall);
}

void CdemoDlg::JudgePiece(vector<struct Pool1> &index)
{
	int i, j, a, b;
	double th, distence, dmax, dmin;
	int state;
	int poolnum = 0;
	struct Pool1* poolall = NULL;
	int counts[6] = {0}; // 各类别计数 0:硬币 1:积木 2:螺母 3:螺栓 4:螺钉 5:扳手
	wchar_t classes[6][10] = {
		L"硬币", L"积木", L"螺母", L"螺栓", L"螺钉", L"扳手"
	};

	poolnum = index.size();
	poolall = (struct Pool1*)malloc(poolnum * sizeof(struct Pool1));
	i = 0;
	for (auto it = index.begin(); it != index.end(); it++)
	{
		poolall[i] = (*it);
		i++;
	}

	a = 0;
	b = 0;
	reset();
	for (auto it = index.begin(); it != index.end(); it++)
	{
		if (poolall[b].num > 1000)
		{
			if (poolall[b].num < Sth)//面积小于10000
			{
				if (poolall[b].water[poolall[b].h0][poolall[b].w0] < 1)//几何中心不在工件内
				{
					(*it).type = Nut;
					counts[2]++;
				}
				else { //几何中心在工件内
					dmin = dmax = sqrt((poolall[b].edge[0] - poolall[b].h0) * (poolall[b].edge[0] - poolall[b].h0) + (poolall[b].edge[1] - poolall[b].w0) * (poolall[b].edge[1] - poolall[b].w0));
					for (i = 1; i < poolall[b].edgenum; i++)
					{
						distence = sqrt((poolall[b].edge[i * 2] - poolall[b].h0) * (poolall[b].edge[i * 2] - poolall[b].h0) + (poolall[b].edge[i * 2 + 1] - poolall[b].w0) * (poolall[b].edge[i * 2 + 1] - poolall[b].w0));
						if (distence < dmin) dmin = distence;
						if (distence > dmax) dmax = distence;
					}
					if (dmax / dmin > 6) {//长宽比小于阈值为螺栓
						(*it).type = Bolt;
						counts[3]++;
					}
					else {
						(*it).type = Screw;
						counts[4]++;
					}
				}
			}
			else {//面积大于10000
				if (poolall[b].water[poolall[b].h0][poolall[b].w0] < 1)//几何中心不在工件内
				{
					(*it).type = Wrench;
					counts[5]++;
				}
				else {//几何中心在工件内
					state = 0;
					th = 0;
					for (i = 0; i < poolall[b].edgenum; i++)
					{
						th += sqrt((poolall[b].edge[i * 2] - poolall[b].h0) * (poolall[b].edge[i * 2] - poolall[b].h0) + (poolall[b].edge[i * 2 + 1] - poolall[b].w0) * (poolall[b].edge[i * 2 + 1] - poolall[b].w0));
					}
					th /= poolall[b].edgenum;
					for (i = 0; i < poolall[b].edgenum; i++)
					{
						distence = sqrt((poolall[b].edge[i * 2] - poolall[b].h0) * (poolall[b].edge[i * 2] - poolall[b].h0) + (poolall[b].edge[i * 2 + 1] - poolall[b].w0) * (poolall[b].edge[i * 2 + 1] - poolall[b].w0));
						if (distence < 0.9 * th || distence > 1.1 * th)
						{
							state++;
						}
					}
					if (state < 150) {//所有边缘点到Coin中心距离近似相等，抛除部分误差
						(*it).type = Coin;
						counts[0]++;
					}
					else {
						(*it).type = Block;
						counts[1]++;
					}
				}
			}

			//int x0 = a < 10 ? 1800 : 1800 + 200;
			//int y0 = 300;
			//// 输出序号
			//CButton* p_No = new CButton();
			//ASSERT_VALID(p_No);
			//string no = to_string(a + 1);
			//CString cno;
			//cno = no.c_str();
			//p_No->Create(cno, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(x0 - 50, y0 + 20 * a, 10, 10), this, (1030 + a * 3)); //创建按钮

			// 输出分类
			//CButton* p_Class = new CButton();
			//ASSERT_VALID(p_Class);
			//p_Class->Create(classes[(*it).type - 1], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON , CRect(x0, y0 + 20 * a, 10, 10), this, (1030 + a*3 + 1)); //创建按钮
			GetDlgItem(1018 + a)->SetWindowTextW(classes[(*it).type - 1]);

			// 输出像素个数
			//CButton* p_Num = new CButton();
			//ASSERT_VALID(p_Num);
			string str = to_string((*it).num);
			CString cstr;
			cstr = str.c_str();
			//p_Num->Create(cstr, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(x0 + 50, y0 + 20 * a, 10, 10), this, (1030 + a*3 + 2)); //创建按钮
			GetDlgItem(1038 + a)->SetWindowTextW(cstr);
			a++;
		}
		b++;
	}

	//输出计数
	string strings[6];
	CString cstrings[6];
	a = 0;
	for (int i = 0; i < 7; i++)
	{
		if (i == 4)
			i++;
		strings[a] = to_string(counts[a]);
		cstrings[a] = strings[a].c_str();
		GetDlgItem(1011 + i)->SetWindowTextW(cstrings[a]);
		a++;
	}
	
	free(poolall);
}


void CdemoDlg::OnBnClickedendrecg()
{
	// TODO: 在此添加控件通知处理程序代码
	Recgon = 0;
	reset();
}
