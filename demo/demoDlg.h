
// demoDlg.h: 头文件
//

#pragma once
#include "MVGigE.h"
#include "MVImage.h"


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
	//定义像素格式对象
	MV_PixelFormatEnums m_PixelFormat;
	//采集标识
	bool m_bRun;
	//通过回调函数获取图像数据信息
	int OnStreamCB(MV_IMAGE_INFO* pInfo);
	//画图
	void DrawImage();
	//关闭相机、释放资源
	void OnClose();
	//判断是否需要开始识别
	void Image_Gray();
	unsigned char Median(unsigned char n1, unsigned char n2, unsigned char n3,
		unsigned char n4, unsigned char n5, unsigned char n6, unsigned char n7,
		unsigned char n8, unsigned char n9);
	int Recgon;

	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedStartgrab();
	afx_msg void OnBnClickedClosecam();
	afx_msg void OnStnClickedpic();


	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedstartrecg();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedendrecg();
};
