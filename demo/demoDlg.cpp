
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
	ON_BN_CLICKED(IDC_OpenCam, &CdemoDlg::OnBnClickedOpencam)
	ON_BN_CLICKED(IDC_StartGrab, &CdemoDlg::OnBnClickedStartgrab)
	ON_BN_CLICKED(IDC_CloseCam, &CdemoDlg::OnBnClickedClosecam)
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
	// 实例化图像
	m_image.CreateByPixelFormat(w, h, m_PixelFormat);
	m_imageGray.CreateByPixelFormat(w, h, PixelFormat_Mono8); // 灰度
	m_imageDid.CreateByPixelFormat(w, h, PixelFormat_Mono8); // 灰度
	m_imageDid1.CreateByPixelFormat(w, h, PixelFormat_Mono8); // 灰度

	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(true);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
}

/*!
*  \brief 在pic控件中显示图像m_image
*  \param [in]	m_image
*  \retval  	void
*/
void CdemoDlg::DrawImage()
{
	CRect rct;
	// 获得pic窗口长宽
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
	if (Recgon == 1)
	{
		Change_Image();
	}
	if (Wrong == 0)
	{
		DrawImage();
	}
	return 0;
}

int __stdcall StreamCB(MV_IMAGE_INFO* pInfo, ULONG_PTR nUserVal)
{
	CdemoDlg* pDlg = (CdemoDlg*)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}

/*!
*  \brief “开始采集”点击事件
*/
void CdemoDlg::OnBnClickedStartgrab()
{
	// TODO: 在此添加控件通知处理程序代码
	TriggerModeEnums enumMode;
	Recgon = 0;
	Wrong = 0;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}
	MVStartGrab(m_hCam, (MVStreamCB)StreamCB, (ULONG_PTR)this); // 采集图像
	m_bRun = true;
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(true);

}

/*!
*  \brief “关闭相机”点击事件
*/
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

void CdemoDlg::OnClose()
{
	if (m_bRun != false)
	{
		MVStopGrab(m_hCam);
	}
	MVTerminateLib();
	CDialog::OnClose();
}


/*!
*  \brief “开始识别”点击事件
*/
void CdemoDlg::OnBnClickedstartrecg()
{
	Recgon = 1;
}


/**
* @brief 清空计数、类别、面积显示
*/
void CdemoDlg::reset() {
	for (int i = 0; i < 7; i++)
	{
		if (i == 4)
			i++;
		GetDlgItem(1011 + i)->SetWindowTextW(L"");
	}
	for (int i = 0; i < 20; i++)
	{
		GetDlgItem(1018 + i)->SetWindowTextW(L"");
		GetDlgItem(1038 + i)->SetWindowTextW(L"");
	}
}

/**
* @brief 
*/
void CdemoDlg::Change_Image()
{
	vector<struct Pool1> index;

	Wrong = 0;
	Image_Gray();
	if (Watershed(index)) // 获取单一工件图链表
	{
		Disedge(index); // 在原图上标记边界
		JudgePiece(index); // 判断工件类型
		freepool(index);
		Wrong = 0;
	}
}

void CdemoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

/**
* @brief 将原图像转换成灰度图
*/
void CdemoDlg::Image_Gray()
{
	int w;
	int h;
	unsigned char* p = (unsigned char*)m_image.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDstd = (unsigned char*)m_imageDid.GetBits();

	w = m_image.GetWidth();
	h = m_image.GetHeight();

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			*pDst = (unsigned char)(0.299 * * p + 0.587 * *(p + 1) + 0.114 * *(p + 2));
			*pDstd = *pDst;
			pDst++;
			pDstd++;
			p += 3;
		}
	}

	pDst = (unsigned char*)m_imageGray.GetBits();
	pDstd = (unsigned char*)m_imageDid.GetBits();

	for (int i = 0; i < w; i++)
	{
		*(pDst + i) = 255;
		*(pDst + (h - 1) * w + i) = 255;
		*(pDstd + i) = 255;
		*(pDstd + (h - 1) * w + i) = 255;
	}
	for (int i = 0; i < h; i++)
	{
		*(pDst + i * w) = 255;
		*(pDst + i * w + w - 1) = 255;
		*(pDstd + i * w) = 255;
		*(pDstd + i * w + w - 1) = 255;
	}
}


/**
* @brief 腐蚀
* @param [in] num 腐蚀次数
* @param [in] th  阈值
* @return		  处理后的图像信息在m_imageDid和m_imageDid1
* @remarks		  在灰度图上进行
* @todo			  第一个循环是否可以删掉？
*/
void CdemoDlg::Corrode(int num, int th)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	unsigned char* pool = (unsigned char*)m_imageDid1.GetBits();
	unsigned char* pcur = NULL;
	int i, j, n;
	int w, h;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	// @todo 这个循环是否可以删掉，后面也赋值了的
	// 给m_imageDid1赋初值
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*(pool + i * w + j) = *(p + i * w + j);
		}
	}
	
	if (num > 0)
	{
		// 进行num次腐蚀
		for (n = 0; n < num; n++)
		{
			for (i = 2; i < h - 2; i++)
			{
				for (j = 2; j < w - 2; j++)
				{
					pcur = pool + i * w + j;
					// 如果某点的8邻域内有任何一个像素的灰度值大于th，则将该点置成背景色(白色)
					// 处理后的图像为m_imageDid
					if (*(pcur - 1) > th || *(pcur + 1) > th ||
						*(pcur - w) > th || *(pcur + w) > th ||
						*(pcur - w - 1) > th || *(pcur - w + 1) > th ||
						*(pcur + w - 1) > th || *(pcur + w + 1) > th)
					{
						*(pDst + i * w + j) = 255;
					}
				}
			}
			// 修改m_imageDid1
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					*(pool + i * w + j) = *(pDst + i * w + j);
				}
			}
		}
	}
}

/**
* @brief 膨胀
* @return		  处理后的图像信息在m_imageDid
* @remarks		  在灰度图m_imageGray上进行
* @todo			  待完善给定处理次数和可变阈值
*/
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
			// 如果某点的16邻域内有任何一个像素的灰度值为前景色，则将该点置成前景色(黑色)
			// 处理后的图像为m_imageDid
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

	// 同时修改m_imageGray
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

/**
* @brief 基于区域生长的分水岭算法
* @param [in] &index
* @todo		注释没写完呢
*/
int CdemoDlg::Watershed(vector<struct Pool1> &index)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	int w, h;
	int i, j, a, count, count1, c;
	double sum = 0;
	double sum1 = 0;
	struct Pool1 pool;
	struct Pool1* pcur = NULL;
	struct Pool1* belong = NULL;
	int state = 0;
	int typenum = 1;
	int found = 0;
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
							Wrong = 1;
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

	poolnum = (int)(index.size());
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

/**
* @brief 显示分割后的水池
* @param [in] index
* @return		不同水池绘制不同的颜色
*/
void CdemoDlg::Dispool(vector<struct Pool1> index)
{
	unsigned char* p = (unsigned char*)m_image.GetBits();
	int h, w, i, j, a;
	int color = 60;
	struct Pool1* pcur = NULL;
	int poolnum = 0;
	struct Pool1* poolall = NULL;

	poolnum = (int)(index.size());
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

/**
* @brief 删库跑路
* @param [in] index
* @todo	注释没写完呢
*/
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

/**
* @brief 一次增长获得水洼
* @param [in] h1
* @param [in] h2
* @param [in] &index
* @param [in] h0
* @param [in] wo
* @param [in] th		阈值
* @todo	注释没写完呢
*/
void CdemoDlg::grow(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, int th)
{
	unsigned char* p = (unsigned char*)m_imageDid.GetBits();
	struct Pool1 pool;
	int h, w, a, b;
	int num = (int)(index.size() - 1);
	
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

/**
* @brief 循环增长水平面稳定获得水池
* @param [in] h1
* @param [in] h2
* @param [in] &index
* @param [in] h0
* @param [in] wo
* @param [in] *pool
* @param [in] Th		阈值
* @param [in] wh
* @todo	注释没写完呢
*/
void CdemoDlg::growagain1(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, struct Pool1* pool, int Th, int wh)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	struct Pool1* pcur = NULL;
	int h, w, i ;
	int state;
	int num, bar, poolnum;
	auto it = index.begin();
	struct Pool1* poolall = NULL;
	poolnum = (int)(index.size()) - 1;
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
		if (obj.empty())
			break;
	}
	pool->num;
	free(poolall);
}

/**
* @brief SUSAN算子边缘提取
* @param [in] h1		像素点所在行
* @param [in] h2		像素点所在列
* @param [in] &index	工件图像链表指针
* @param [in] h0		图像长
* @param [in] wo		图像宽
* @param [in] *pool		当前的工件结构体
* @param [in] Th		阈值
* @param [in] wh		该工件内部点的填充值
* @return				边缘点坐标全部存于在pool->edge[]
*						同时计算出几何中心、最大最小坐标值
* @remarks				分别在分割后的图像上进行
*/
void CdemoDlg::grow_susan(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, struct Pool1* pool, int Th, int wh)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	struct Pool1* pcur = NULL;
	int h, w, i, j;
	// state: 标识是否包含于其他工件
	// susan: susan算子值
	int state, susan;
	int poolnum;// 工件总数
	// 模板大小为3*3
	int m = 3, n = 3;
	// 指向第一个工件
	auto it = index.begin();
	// 数组：存放所有工件结构体
	struct Pool1* poolall = NULL;

	poolnum = (int)(index.size()) - 1;
	poolall = (struct Pool1*)malloc((index.size() - 1) * sizeof(struct Pool1));
	i = 0;

	// @todo 两个循环可合并？直接从begin到end
	// 给poolall赋值
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

	// vector obj: 用于存放需要被判断的点坐标
	vector<int>obj;
	obj.push_back(h1);
	obj.push_back(w1);

	// num面积，edgenum边缘点个数，h0几何中心纵坐标，w0几何中心横坐标
	pool->num = 1;
	pool->edgenum = 0;
	pool->h0 = 0;
	pool->w0 = 0;

	// 循环结束条件：obj内已无需要被判断的点坐标
	while (1)
	{
		// 取出这次需要判断的点坐标
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();

		// 对当前点(w,h)的8邻域进行判断，一定程度上保证边缘点连续
		// 且不需要遍历整个图，减小计算量
		
		// 八邻域 - 1
		// 该点还没有被归为当前工件内部，且灰度值小于阈值(前景)
		if (pool->water[h - 1][w - 1] < wh && *(p + (h - 1) * w0 + (w - 1)) < Th)
		{
			state = 1;
			// 如果当前坐标在其他工件内部，则不进行接下来的边缘检测
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			// 在当前工件内部，进行边缘检测
			if (state == 1) {
				pool->water[h - 1][w - 1] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w - 1);
				susan = 0;
				// susan算子
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						susan += pool->water[h - 2 + i][w - 2 + j] >= 1;
					}
				}
				susan = 9 - susan;
				// 如果susan值大于等于1，则判断为边缘
				if (susan >= 1)
				{
					// 记录边缘点坐标
					pool->edge[pool->edgenum * 2] = h - 1;
					pool->edge[pool->edgenum * 2 + 1] = w - 1;
					pool->edgenum++;
				}
				// 修改几何中心、最大最小坐标值
				pool->h0 += h - 1;
				pool->w0 += w - 1;
				if (pool->wmin > w - 1) pool->wmin = w - 1;
				if (pool->wmax < w - 1) pool->wmax = w - 1;
				if (pool->hmin > h - 1) pool->hmin = h - 1;
				if (pool->hmax < h - 1) pool->hmax = h - 1;
			}
		}

		// 八邻域 - 2
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
				pool->water[h - 1][w] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
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
		}

		// 八邻域 - 3
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
				pool->water[h - 1][w + 1] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w + 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
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
		}

		// 八邻域 - 4
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
				pool->water[h][w - 1] = wh;
				pool->num++;
				obj.push_back(h);
				obj.push_back(w - 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
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
		}

		// 八邻域 - 5
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
				pool->water[h][w + 1] = wh;
				pool->num++;
				obj.push_back(h);
				obj.push_back(w + 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
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
		}

		// 八邻域 - 6
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
				pool->water[h + 1][w - 1] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w - 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
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
		}

		// 八邻域 - 7
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
				pool->water[h + 1][w] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
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
		}

		// 八邻域 - 8
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
				pool->water[h + 1][w + 1] = wh;
				pool->num++;
				obj.push_back(h + 1);
				obj.push_back(w + 1);
				susan = 0;
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
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
		}

		// 全部点都已经检测完成，退出循环
		if (obj.empty())
			break;
	}

	free(poolall);
	// 计算出几何中心
	pool->h0 /= pool->num;
	pool->w0 /= pool->num;
}

/**
* @brief 框出工件，并输出工件序号、其他几何特征
* @param [in] &index	工件图像链表指针
* @return				
* @remarks				在原图上进行
*/
void CdemoDlg::Disedge(vector<struct Pool1> &index)
{
	unsigned char* p = (unsigned char*)m_image.GetBits();
	int h, w, i, j, a, b; // b: 工件序号
	// 横纵坐标最大最小值
	int hmin, wmin, hmax, wmax;
	// pointnum: 
	// poolnum: 水池总数
	int pointnum = 0, poolnum = 0;
	// 数组：存放所有工件结构体
	struct Pool1* poolall = NULL;

	poolnum = (int)(index.size());
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
		// 过滤掉过度分割的小区域
		if (poolall[a].num > 1000)
		{
			pointnum = poolall[a].edgenum;
			hmin = hmax = poolall[a].edge[0];
			wmin = wmax = poolall[a].edge[1];

			// 画矩形框，坐标为hmin、hmax、wmin、wmax
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
			
			// 画几何中心
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < 2; j++)
				{
					*(p + (poolall[a].h0 + i) * w * 3 + (poolall[a].w0 + j) * 3) = 255;
					*(p + (poolall[a].h0 + i) * w * 3 + (poolall[a].w0 + j) * 3 + 1) = 0;
					*(p + (poolall[a].h0 + i) * w * 3 + (poolall[a].w0 + j) * 3 + 2) = 0;
				}
			}

			// 标数字，位于矩形框的左上角
			int x0 = (poolall[a].hmin - 30) > 0 ? poolall[a].hmin - 30 : 0; // 基准横坐标，防溢出
			int y0 = (poolall[a].wmin + 15) < w ? poolall[a].wmin : w - 16; // 基准纵坐标，防溢出
			// 序号小于10
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
			else if (b + 1 < 20)// 序号小于20
			{
				int tens = 1; // 十位数
				int units = b + 1 - 10; // 个位数
				y0 = (wmin + 15 + 20) < w ? wmin : w - 36;// 防溢出
				for (i = 0; i < 25; i++)
				{
					for (j = 0; j < 15; j++) {
						if (nums[tens][i][j] == 1) {
							*(p + (x0 + i) * w * 3 + (y0 + j) * 3) = 255;
							*(p + (x0 + i) * w * 3 + (y0 + j) * 3 + 1) = 255;
							*(p + (x0 + i) * w * 3 + (y0 + j) * 3 + 2) = 255;
						}
					}
				}
				for (i = 0; i < 25; i++)
				{
					for (j = 0; j < 15; j++) {
						if (nums[units][i][j] == 1) {
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

/**
* @brief 判断工件类别
* @param [in] &index	工件图像链表指针
* @return
* @remarks				分别在原图上进行
*/
void CdemoDlg::JudgePiece(vector<struct Pool1> &index)
{
	int i, a, b;
	// th: 阈值
	// distance: 距离
	// dmax: 距离最大值
	// dmin: 距离最小值
	double th, distance, dmax, dmin;
	int state;
	int poolnum = 0;
	struct Pool1* poolall = NULL;// 数组：存放所有工件结构体
	int counts[6] = {0}; // 各类别计数 0:硬币 1:积木 2:螺母 3:螺栓 4:螺钉 5:扳手
	wchar_t classes[6][10] = {
		L"硬币", L"积木", L"螺母", L"螺栓", L"螺钉", L"扳手"
	};

	poolnum = (int)(index.size());
	poolall = (struct Pool1*)malloc(poolnum * sizeof(struct Pool1));

	i = 0;
	for (auto it = index.begin(); it != index.end(); it++)
	{
		poolall[i] = (*it);
		i++;
	}

	a = 0;
	b = 0;
	reset(); // 判别之前清空输出显示
	for (auto it = index.begin(); it != index.end(); it++)
	{
		// 滤掉过度分割的小区域
		if (poolall[b].num > 1000)
		{
			if (poolall[b].num < Sth) // 面积小于阈值，该分支只有螺母、螺钉、螺栓
			{
				// 几何中心不在工件上，为螺母
				if (poolall[b].water[poolall[b].h0][poolall[b].w0] < 1)
				{
					(*it).type = Nut;
					counts[2]++;
				}
				// 几何中心在工件上，为螺钉或螺栓
				else {
					dmin = dmax = sqrt((poolall[b].edge[0] - poolall[b].h0) * (poolall[b].edge[0] - poolall[b].h0) + (poolall[b].edge[1] - poolall[b].w0) * (poolall[b].edge[1] - poolall[b].w0));
					// 计算 几何中心到边缘点的 最大距离和最小距离
					for (i = 1; i < poolall[b].edgenum; i++)
					{
						distance = sqrt((poolall[b].edge[i * 2] - poolall[b].h0) * (poolall[b].edge[i * 2] - poolall[b].h0) + (poolall[b].edge[i * 2 + 1] - poolall[b].w0) * (poolall[b].edge[i * 2 + 1] - poolall[b].w0));
						if (distance < dmin) dmin = distance;
						if (distance > dmax) dmax = distance;
					}
					// 长宽比(最大与最小距离之比)小于阈值为螺栓
					// 螺栓更粗，dmin大，故dmax/dmin比较小
					// @todo 为什么代码跟逻辑相反？
					if (dmax / dmin > 6) {
						(*it).type = Bolt; // 螺栓
						counts[3]++;
					}
					else {
						(*it).type = Screw; // 螺钉
						counts[4]++;
					}
				}
			}
			else { // 面积大于阈值，该分支只有L杆、硬币、积木

				// 几何中心不在工件上，为L杆
				if (poolall[b].water[poolall[b].h0][poolall[b].w0] < 1)
				{
					(*it).type = Wrench;
					counts[5]++;
				}
				else { // 几何中心在工件上，为硬币或积木
					state = 0;
					th = 0;
					// 所有边缘点到几何中心距离的平均值
					for (i = 0; i < poolall[b].edgenum; i++)
					{
						th += pow(poolall[b].edge[i * 2] - poolall[b].h0, 2) + pow(poolall[b].edge[i * 2 + 1] - poolall[b].w0, 2);
					}
					th /= poolall[b].edgenum;
					// 边缘点到几何中心距离 与 均值 相差较大的点个数
					for (i = 0; i < poolall[b].edgenum; i++)
					{
						distance = pow(poolall[b].edge[i * 2] - poolall[b].h0,2) + pow(poolall[b].edge[i * 2 + 1] - poolall[b].w0,2);
						if (distance < 0.9 * th || distance > 1.1 * th)
						{
							state++;
						}
					}
					// 所有边缘点到几何中心距离近似相等，为硬币
					if (state < 150) {
						(*it).type = Coin;
						counts[0]++;
					}
					else { // 否则为积木
						(*it).type = Block;
						counts[1]++;
					}
				}
			}

			// 输出当前工件的类别
			GetDlgItem(1018 + a)->SetWindowTextW(classes[(*it).type - 1]);
			// 输出当前工件的面积
			string str = to_string((*it).num);
			CString cstr;
			cstr = str.c_str();
			GetDlgItem(1038 + a)->SetWindowTextW(cstr);
			a++;
		}
		b++;
	}

	//输出各类别计数
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

/**
* @brief “停止识别”点击事件
*/
void CdemoDlg::OnBnClickedendrecg()
{
	Recgon = 0;
	reset();
}
