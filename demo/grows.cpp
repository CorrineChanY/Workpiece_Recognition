/****************************************************
* @file grows.cpp
* @brief ��������
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

/**
* @brief һ���������ˮ��
* @param [in] h1        ����������
* @param [in] w1        ���Ӻ�����
* @param [in] &index    ����ͼ������ָ��
* @param [in] h0        ͼ��߶�
* @param [in] w0        ͼ����
* @param [in] th		��ֵ
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
	auto it = index.end() - 1;//���������洢�����һ���ṹ�壬�����������Ķ���
	vector<int>obj;//��������������ջ ���ڴ洢�����������������ص�
	obj.push_back(h1);
	obj.push_back(w1);//�����Ӵ����������������ջ
	while (1)
	{
		//���������ջ�����һ�����������ص��Ƴ���ջ��������Χ�˸����ص��ж��Ƿ���Ҫ����
		//������ص�������ֵ��δ���������䱣���ڹ���ͼ���ϣ���������������ջ�У�֮���ٶ�������
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		// ������ - 1
		if (pool.water[h - 1][w - 1] == 0 && *(p + (h - 1) * w0 + (w - 1)) < th)
		{
			pool.water[h - 1][w - 1] = 1;
			pool.num++;
			obj.push_back(h - 1);
			obj.push_back(w - 1);
		}
		// ������ - 2
		if (pool.water[h - 1][w] == 0 && *(p + (h - 1) * w0 + (w)) < th)
		{
			pool.water[h - 1][w] = 1;
			pool.num++;
			obj.push_back(h - 1);
			obj.push_back(w);
		}
		// ������ - 3
		if (pool.water[h - 1][w + 1] == 0 && *(p + (h - 1) * w0 + (w + 1)) < th)
		{
			pool.water[h - 1][w + 1] = 1;
			pool.num++;
			obj.push_back(h - 1);
			obj.push_back(w + 1);
		}
		// ������ - 4
		if (pool.water[h][w - 1] == 0 && *(p + (h)*w0 + (w - 1)) < th)
		{
			pool.water[h][w - 1] = 1;
			pool.num++;
			obj.push_back(h);
			obj.push_back(w - 1);
		}
		// ������ - 5
		if (pool.water[h][w + 1] == 0 && *(p + (h)*w0 + (w + 1)) < th)
		{
			pool.water[h][w + 1] = 1;
			pool.num++;
			obj.push_back(h);
			obj.push_back(w + 1);
		}
		// ������ - 6
		if (pool.water[h + 1][w - 1] == 0 && *(p + (h + 1) * w0 + (w - 1)) < th)
		{
			pool.water[h + 1][w - 1] = 1;
			pool.num++;
			obj.push_back(h + 1);
			obj.push_back(w - 1);
		}
		// ������ - 7
		if (pool.water[h + 1][w] == 0 && *(p + (h + 1) * w0 + (w)) < th)
		{
			pool.water[h + 1][w] = 1;
			pool.num++;
			obj.push_back(h + 1);
			obj.push_back(w);
		}
		// ������ - 8
		if (pool.water[h + 1][w + 1] == 0 && *(p + (h + 1) * w0 + (w + 1)) < th)
		{
			pool.water[h + 1][w + 1] = 1;
			pool.num++;
			obj.push_back(h + 1);
			obj.push_back(w + 1);
		}
		if (obj.empty())//���������ջΪ�գ�������ѭ��
			break;
	}
	index.pop_back();
	index.push_back(pool);//��������ɵ�ͼ���ƻ�����
}


/**
* @brief ѭ������ˮƽ���ȶ����ˮ��
* @param [in] h1        ����������
* @param [in] w1        ���Ӻ�����
* @param [in] &index    ����ͼ������ָ��
* @param [in] h0        ͼ��߶�
* @param [in] w0        ͼ����
* @param [in] *pool     ����������Ӧ�ṹ��
* @param [in] Th		��ֵ
* @param [in] wh        ͼ��洢�б������������ֵ
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
	vector<int>obj;//��������������ջ ���ڴ洢�����������������ص�
	obj.push_back(h1);
	obj.push_back(w1);//�����Ӵ����������������ջ
	while (1)
	{
		//���������ջ�����һ�����������ص��Ƴ���ջ��������Χ�˸����ص��ж��Ƿ���Ҫ����
		//������ص�������ֵ��δ���������䱣���ڹ���ͼ���ϣ���������������ջ�У�֮���ٶ�������
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();
		num = 0;
		bar = 0;
		// ������ - 1
		if (pool->water[h - 1][w - 1] < wh && *(p + (h - 1) * w0 + (w - 1)) < Th)
		{
			state = 1;
			//�жϸ������Ƿ��Ѿ�������ˮ�����������ѱ�����������ˮ��
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			//��δ�������������
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
		// ������ - 2
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
		// ������ - 3
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
		// ������ - 4
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
		// ������ - 5
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
		// ������ - 6
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
		// ������ - 7
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
		// ������ - 8
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