
// demoDlg.h: 头文件
//

#pragma once
#include "MVGigE.h"
#include "MVImage.h"
#include<vector>
#define Sth 15000
using namespace std;


// CdemoDlg 对话框
class CdemoDlg : public CDialogEx
{
// 构造
public:
	CdemoDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();

public:
	afx_msg void OnBnClickedOk();
	//定义相机句柄对象
	HANDLE m_hCam;
	//定义图像处理对象
	MVImage m_image;
	//定义图像处理后的对象
	MVImage m_imageDid;
	MVImage m_imageDid1;
	MVImage m_imageGray;
	//定义像素格式对象
	MV_PixelFormatEnums m_PixelFormat;
	//采集标识
	bool m_bRun;
	enum Piece {
		Coin = 1, Block, Nut, Bolt, Screw, Wrench
	};//硬币，积木，螺母，螺栓，螺丝，扳手
	struct Pool1 {
		int symbel;//工件序号
		long int num;//工件像素数
		int** water;//工件图片
		//int* point;//记录所有点
		int edge[5000];//记录边界
		int edgenum;//记录边界点数
		int h0;//记录几何中心纵坐标
		int w0;//记录几何中心横坐标
		enum Piece type;//记录工件类型
		int wmin;
		int wmax;
		int hmax;
		int hmin;
	};
	//通过回调函数获取图像数据信息
	int OnStreamCB(MV_IMAGE_INFO* pInfo);
	//画图
	void DrawImage();
	//void DrawImageGray();
	//关闭相机、释放资源
	void OnClose();
	// 读取bmp图片
	BOOL readBMP();
	// 转成灰度图
	void getGay();
	//判断是否需要开始识别
	void Image_Gray();
	unsigned char Median(unsigned char n1, unsigned char n2, unsigned char n3,
		unsigned char n4, unsigned char n5, unsigned char n6, unsigned char n7,
		unsigned char n8, unsigned char n9);
	void Corrode(int num, int th);
	void Expand();
	void Susan();
	int Watershed(vector<struct Pool1> &index);
	void grow(int h, int w, vector<struct Pool1> &index, int h0, int w0, int th);
	void growagain1(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, struct Pool1* pool, int Th, int wh);
	void grow_susan(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, struct Pool1* pool, int Th, int wh);
	void Dispool(vector<struct Pool1> index);
	void freepool(vector<struct Pool1> index);
	void Disedge(vector<struct Pool1> &index);
	void JudgePiece(vector<struct Pool1> &index);
	void Change_Image();
	void reset();
	int Recgon;

	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedStartgrab();
	afx_msg void OnBnClickedClosecam();
	afx_msg void OnStnClickedpic();
	afx_msg void OnBnClickedstartrecg();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedendrecg();
};
