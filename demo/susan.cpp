/****************************************************
* @file susan.cpp
* @brief Susan算子
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

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
void CdemoDlg::grow_susan(int h1, int w1, vector<struct Pool1>& index, int h0, int w0, struct Pool1* pool, int Th, int wh)
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
