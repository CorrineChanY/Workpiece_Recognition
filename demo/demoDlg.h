
// demoDlg.h: 头文件
//

#pragma once
#include "MVGigE.h"
#include "MVImage.h"
#include<vector>
#define Sth 10000
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
		int num;//工件像素数
		int** water;//工件图片
		struct Pool1* next;//下一个工件节点
		struct Pool1* end;//仅index中保存链表队尾
		/*vector<int>point;
		vector<int>edge;*/
		int* point;//记录所有点
		int* edge;//记录边界
		int edgenum;//记录边界点数
		int h0;//记录几何中心纵坐标
		int w0;//记录几何中心横坐标
		enum Piece type;//记录工件类型
	};
	//通过回调函数获取图像数据信息
	int OnStreamCB(MV_IMAGE_INFO* pInfo);
	//画图
	void DrawImage();
	void DrawImageGray();
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
	void Corrode();
	void Expand();
	void Susan();
	void Watershed(struct Pool1* index);
	/*int Waterinpool(struct Pool pool, int h, int w);
	void replacepool(struct Pool* pool);
	void Displayedge(struct Pool* fpool);*/
	void grow(int h, int w, struct Pool1 index, int h0, int w0, int th);
	/*void growagain(int h1, int w1, struct Pool1 index, int h0, int w0, struct Pool1* pool);*/
	void growagain1(int h1, int w1, struct Pool1 index, int h0, int w0, struct Pool1* pool, int Th, int wh);
	void growdone(int h1, int w1, struct Pool1 index, int h0, int w0, struct Pool1* pool, int Th, int wh);
	void Dispool(struct Pool1* index);
	void freepool(struct Pool1* index);
	void simplar_susan(struct Pool1* index);
	void Disedge(struct Pool1* index);
	void JudgePiece(struct Pool1* index);
	int Recgon;

	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedStartgrab();
	afx_msg void OnBnClickedClosecam();
	afx_msg void OnStnClickedpic();
	afx_msg void OnBnClickedstartrecg();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnStnClickedpicgray();
};
