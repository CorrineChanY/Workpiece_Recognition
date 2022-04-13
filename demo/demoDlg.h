
// demoDlg.h: 头文件

#pragma once
#include "MVGigE.h"
#include "MVImage.h"
#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "afxdialogex.h"
#include <malloc.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string> 
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

	// 定义相机句柄对象
	HANDLE m_hCam;

	// 定义图像处理对象
	MVImage m_image;

	// 定义图像处理后的对象
	MVImage m_imageDid;
	MVImage m_imageDid1;
	MVImage m_imageGray;

	// 定义像素格式对象
	MV_PixelFormatEnums m_PixelFormat;

	// 采集标识
	bool m_bRun;

	// 1: 正在识别 0: 停止识别
	int Recgon;

	// 算法错误标志位
	// 0: 算法出错 图像未处理 不显示
	// 1: 算法正确
	int Wrong;

	// 类别枚举
	enum Piece {
		Coin = 1, Block, Nut, Bolt, Screw, Wrench
	}; // 硬币，积木，螺母，螺栓，螺丝，扳手

	// 工件结构体
	struct Pool1 {
		int symbel; // 工件序号
		long int num; // 工件像素数(面积)
		int** water; // 工件图片像素信息
		int edge[5000]; // 边界点坐标
		int edgenum; // 边缘点个数
		int h0; // 几何中心纵坐标
		int w0; // 几何中心横坐标
		enum Piece type; // 工件类型
		int wmin; // 横坐标最小值
		int wmax; // 横坐标最大值
		int hmax; // 纵坐标最大值
		int hmin; // 纵坐标最小值
	};

	/**
	* @brief 通过回调函数获取图像数据信息
	*/
	int OnStreamCB(MV_IMAGE_INFO* pInfo);

	/**
	* @brief 画图m_image
	*/
	void DrawImage();

	/**
	* @brief 关闭相机、释放资源
	*/
	void OnClose();

	/**
	* @brief 判断是否需要开始识别
	*/
	void Image_Gray();

	/**
	* @brief 腐蚀
	* @param [in] num 腐蚀次数
	* @param [in] th  阈值
	* @return		  处理后的图像信息在m_imageDid和m_imageDid1
	* @remarks		  在灰度图m_imageGray上进行
	*/
	void Corrode(int num, int th);

	/**
	* @brief 膨胀
	* @return		  处理后的图像信息在m_imageDid
	* @remarks		  在灰度图m_imageGray上进行
	*/
	void Expand(int num, int th);

	/**
	* @brief 基于区域生长的分水岭算法
	* @param [in] &index 用于存储找到的每一个工件的结构体并回传
	* @retval 0 fail 算法出错
	* @retval 1 succeed 算法运行完成
	*/
	int Watershed(vector<struct Pool1> &index);

	/**
	* @brief 一次增长获得水洼
	* @param [in] h1        种子纵坐标
	* @param [in] w1        种子横坐标
	* @param [in] &index    工件图像链表指针
	* @param [in] h0        图像高度
	* @param [in] w0        图像宽度
	* @param [in] th		阈值
	*/
	void grow(int h, int w, vector<struct Pool1> &index, int h0, int w0, int th);

	/**
	* @brief 循环增长水平面稳定获得水池
	* @param [in] h1        种子纵坐标
	* @param [in] w1        种子横坐标
	* @param [in] &index    工件图像链表指针
	* @param [in] h0        图像高度
	* @param [in] w0        图像宽度
	* @param [in] *pool     待处理工件对应结构体
	* @param [in] Th		阈值
	* @param [in] wh        图像存储中表达已生长的阈值
	*/
	void growagain1(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, struct Pool1* pool, int Th, int wh);

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
	void grow_susan(int h1, int w1, vector<struct Pool1> &index, int h0, int w0, struct Pool1* pool, int Th, int wh);

	/**
	* @brief 显示分割后的水池
	* @param [in] index
	* @return		不同水池绘制不同的颜色
	*/
	void Dispool(vector<struct Pool1> index);

	/**
	* @brief 删库跑路
	* @param [in] index     工件图像链表指针
	*/
	void freepool(vector<struct Pool1> index);

	/**
	* @brief 框出工件，并输出工件序号、其他几何特征
	* @param [in] &index	工件图像链表指针
	* @return
	* @remarks				在原图上进行
	*/
	void Disedge(vector<struct Pool1> &index);

	/**
	* @brief 判断工件类别
	* @param [in] &index	工件图像链表指针
	* @return
	* @remarks				分别在原图上进行
	*/
	void JudgePiece(vector<struct Pool1> &index);

	/**
	* @brief 处理主函数
	*/
	void Change_Image();

	/**
	* @brief 清空计数、类别、面积显示
	*/
	void reset();

	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedStartgrab();
	afx_msg void OnBnClickedClosecam();
	afx_msg void OnBnClickedstartrecg();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedendrecg();
};
