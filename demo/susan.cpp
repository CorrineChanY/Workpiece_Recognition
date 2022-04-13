/****************************************************
* @file susan.cpp
* @brief Susan����
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

/**
* @brief SUSAN���ӱ�Ե��ȡ
* @param [in] h1		���ص�������
* @param [in] h2		���ص�������
* @param [in] &index	����ͼ������ָ��
* @param [in] h0		ͼ��
* @param [in] wo		ͼ���
* @param [in] *pool		��ǰ�Ĺ����ṹ��
* @param [in] Th		��ֵ
* @param [in] wh		�ù����ڲ�������ֵ
* @return				��Ե������ȫ��������pool->edge[]
*						ͬʱ������������ġ������С����ֵ
* @remarks				�ֱ��ڷָ���ͼ���Ͻ���
*/
void CdemoDlg::grow_susan(int h1, int w1, vector<struct Pool1>& index, int h0, int w0, struct Pool1* pool, int Th, int wh)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	struct Pool1* pcur = NULL;
	int h, w, i, j;
	// state: ��ʶ�Ƿ��������������
	// susan: susan����ֵ
	int state, susan;
	int poolnum;// ��������
	// ģ���СΪ3*3
	int m = 3, n = 3;
	// ָ���һ������
	auto it = index.begin();
	// ���飺������й����ṹ��
	struct Pool1* poolall = NULL;

	poolnum = (int)(index.size()) - 1;
	poolall = (struct Pool1*)malloc((index.size() - 1) * sizeof(struct Pool1));
	i = 0;

	// ��poolall��ֵ
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

	// vector obj: ���ڴ����Ҫ���жϵĵ�����
	vector<int>obj;
	obj.push_back(h1);
	obj.push_back(w1);

	// num�����edgenum��Ե�������h0�������������꣬w0�������ĺ�����
	pool->num = 1;
	pool->edgenum = 0;
	pool->h0 = 0;
	pool->w0 = 0;

	// ѭ������������obj��������Ҫ���жϵĵ�����
	while (1)
	{
		// ȡ�������Ҫ�жϵĵ�����
		w = obj.back();
		obj.pop_back();
		h = obj.back();
		obj.pop_back();

		// �Ե�ǰ��(w,h)��8��������жϣ�һ���̶��ϱ�֤��Ե������
		// �Ҳ���Ҫ��������ͼ����С������

		// ������ - 1
		// �õ㻹û�б���Ϊ��ǰ�����ڲ����һҶ�ֵС����ֵ(ǰ��)
		if (pool->water[h - 1][w - 1] < wh && *(p + (h - 1) * w0 + (w - 1)) < Th)
		{
			state = 1;
			// �����ǰ���������������ڲ����򲻽��н������ı�Ե���
			for (i = 0; i < poolnum; i++)
			{
				if (poolall[i].water[h - 1][w - 1] >= 1)
				{
					state = 0;
					break;
				}
			}
			// �ڵ�ǰ�����ڲ������б�Ե���
			if (state == 1) {
				pool->water[h - 1][w - 1] = wh;
				pool->num++;
				obj.push_back(h - 1);
				obj.push_back(w - 1);
				susan = 0;
				// susan����
				for (i = 0; i < m; i++)
				{
					for (j = 0; j < n; j++)
					{
						susan += pool->water[h - 2 + i][w - 2 + j] >= 1;
					}
				}
				susan = 9 - susan;
				// ���susanֵ���ڵ���1�����ж�Ϊ��Ե
				if (susan >= 1)
				{
					// ��¼��Ե������
					pool->edge[pool->edgenum * 2] = h - 1;
					pool->edge[pool->edgenum * 2 + 1] = w - 1;
					pool->edgenum++;
				}
				// �޸ļ������ġ������С����ֵ
				pool->h0 += h - 1;
				pool->w0 += w - 1;
				if (pool->wmin > w - 1) pool->wmin = w - 1;
				if (pool->wmax < w - 1) pool->wmax = w - 1;
				if (pool->hmin > h - 1) pool->hmin = h - 1;
				if (pool->hmax < h - 1) pool->hmax = h - 1;
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

		// ȫ���㶼�Ѿ������ɣ��˳�ѭ��
		if (obj.empty())
			break;
	}

	free(poolall);
	// �������������
	pool->h0 /= pool->num;
	pool->w0 /= pool->num;
}
