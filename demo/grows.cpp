/****************************************************
* @file grows.cpp
* @brief 区域生长
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

/**
* @brief 一次增长获得水洼
* @param [in] h1        种子纵坐标
* @param [in] w1        种子横坐标
* @param [in] &index    工件图像链表指针
* @param [in] h0        图像高度
* @param [in] w0        图像宽度
* @param [in] th		阈值
*/
void CdemoDlg::grow(int h1, int w1, vector<struct Pool1>& index, int h0, int w0, int th)
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
	auto it = index.end() - 1;//读出向量存储的最后一个结构体，即正在生长的对象
	vector<int>obj;//待处理（生长）堆栈 用于存储待处理（生长）的像素点
	obj.push_back(h1);
	obj.push_back(w1);//将种子存入待处理（生长）堆栈
	while (1)
	{
		//将待处理堆栈的最后一个待处理像素点推出堆栈，对它周围八个像素点判断是否需要生长
		//如果像素点满足阈值且未生长，则将其保存在工件图像上，将其推入待处理堆栈中，之后再对其生长
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		// 八邻域 - 1
		if (pool.water[h - 1][w - 1] == 0 && *(p + (h - 1) * w0 + (w - 1)) < th)
		{
			pool.water[h - 1][w - 1] = 1;
			pool.num++;
			obj.push_back(h - 1);
			obj.push_back(w - 1);
		}
		// 八邻域 - 2
		if (pool.water[h - 1][w] == 0 && *(p + (h - 1) * w0 + (w)) < th)
		{
			pool.water[h - 1][w] = 1;
			pool.num++;
			obj.push_back(h - 1);
			obj.push_back(w);
		}
		// 八邻域 - 3
		if (pool.water[h - 1][w + 1] == 0 && *(p + (h - 1) * w0 + (w + 1)) < th)
		{
			pool.water[h - 1][w + 1] = 1;
			pool.num++;
			obj.push_back(h - 1);
			obj.push_back(w + 1);
		}
		// 八邻域 - 4
		if (pool.water[h][w - 1] == 0 && *(p + (h)*w0 + (w - 1)) < th)
		{
			pool.water[h][w - 1] = 1;
			pool.num++;
			obj.push_back(h);
			obj.push_back(w - 1);
		}
		// 八邻域 - 5
		if (pool.water[h][w + 1] == 0 && *(p + (h)*w0 + (w + 1)) < th)
		{
			pool.water[h][w + 1] = 1;
			pool.num++;
			obj.push_back(h);
			obj.push_back(w + 1);
		}
		// 八邻域 - 6
		if (pool.water[h + 1][w - 1] == 0 && *(p + (h + 1) * w0 + (w - 1)) < th)
		{
			pool.water[h + 1][w - 1] = 1;
			pool.num++;
			obj.push_back(h + 1);
			obj.push_back(w - 1);
		}
		// 八邻域 - 7
		if (pool.water[h + 1][w] == 0 && *(p + (h + 1) * w0 + (w)) < th)
		{
			pool.water[h + 1][w] = 1;
			pool.num++;
			obj.push_back(h + 1);
			obj.push_back(w);
		}
		// 八邻域 - 8
		if (pool.water[h + 1][w + 1] == 0 && *(p + (h + 1) * w0 + (w + 1)) < th)
		{
			pool.water[h + 1][w + 1] = 1;
			pool.num++;
			obj.push_back(h + 1);
			obj.push_back(w + 1);
		}
		if (obj.empty())//若待处理堆栈为空，则跳出循环
			break;
	}
	index.pop_back();
	index.push_back(pool);//将生长完成的图像推回向量
}


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
void CdemoDlg::growagain1(int h1, int w1, vector<struct Pool1>& index, int h0, int w0, struct Pool1* pool, int Th, int wh)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	struct Pool1* pcur = NULL;
	int h, w, i;
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
	vector<int>obj;//待处理（生长）堆栈 用于存储待处理（生长）的像素点
	obj.push_back(h1);
	obj.push_back(w1);//将种子存入待处理（生长）堆栈
	while (1)
	{
		//将待处理堆栈的最后一个待处理像素点推出堆栈，对它周围八个像素点判断是否需要生长
		//如果像素点满足阈值且未生长，则将其保存在工件图像上，将其推入待处理堆栈中，之后再对其生长
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		num = 0;
		bar = 0;
		// 八邻域 - 1
		if (pool->water[h - 1][w - 1] < wh && *(p + (h - 1) * w0 + (w - 1)) < Th)
		{
			state = 1;
			//判断该种子是否已经被其他水池生长，若已被生长则筑防水坝
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			//若未生长则对其生长
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