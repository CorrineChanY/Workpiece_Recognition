/****************************************************
* @file display.cpp
* @brief 图片显示、结果显示
*****************************************************/

#pragma once
#include "pch.h"
#include "disNums.h"
#include "demoDlg.h"

/**
* @brief 显示分割后的水池
* @param [in] index 保存每个工件信息的向量
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
* @brief 框出工件，并输出工件序号、其他几何特征
* @param [in] &index	工件图像链表指针
* @remarks				在原图上进行
*/
void CdemoDlg::Disedge(vector<struct Pool1>& index)
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