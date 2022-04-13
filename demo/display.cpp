/****************************************************
* @file display.cpp
* @brief ͼƬ��ʾ�������ʾ
*****************************************************/

#pragma once
#include "pch.h"
#include "disNums.h"
#include "demoDlg.h"

/**
* @brief ��ʾ�ָ���ˮ��
* @param [in] index ����ÿ��������Ϣ������
* @return		��ͬˮ�ػ��Ʋ�ͬ����ɫ
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
* @brief ��������������������š�������������
* @param [in] &index	����ͼ������ָ��
* @remarks				��ԭͼ�Ͻ���
*/
void CdemoDlg::Disedge(vector<struct Pool1>& index)
{
	unsigned char* p = (unsigned char*)m_image.GetBits();
	int h, w, i, j, a, b; // b: �������
	// �������������Сֵ
	int hmin, wmin, hmax, wmax;
	// pointnum: 
	// poolnum: ˮ������
	int pointnum = 0, poolnum = 0;
	// ���飺������й����ṹ��
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
		// ���˵����ȷָ��С����
		if (poolall[a].num > 1000)
		{
			pointnum = poolall[a].edgenum;
			hmin = hmax = poolall[a].edge[0];
			wmin = wmax = poolall[a].edge[1];

			// �����ο�����Ϊhmin��hmax��wmin��wmax
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

			// ����������
			for (i = 0; i < 2; i++)
			{
				for (j = 0; j < 2; j++)
				{
					*(p + (poolall[a].h0 + i) * w * 3 + (poolall[a].w0 + j) * 3) = 255;
					*(p + (poolall[a].h0 + i) * w * 3 + (poolall[a].w0 + j) * 3 + 1) = 0;
					*(p + (poolall[a].h0 + i) * w * 3 + (poolall[a].w0 + j) * 3 + 2) = 0;
				}
			}

			// �����֣�λ�ھ��ο�����Ͻ�
			int x0 = (poolall[a].hmin - 30) > 0 ? poolall[a].hmin - 30 : 0; // ��׼�����꣬�����
			int y0 = (poolall[a].wmin + 15) < w ? poolall[a].wmin : w - 16; // ��׼�����꣬�����
			// ���С��10
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
			else if (b + 1 < 20)// ���С��20
			{
				int tens = 1; // ʮλ��
				int units = b + 1 - 10; // ��λ��
				y0 = (wmin + 15 + 20) < w ? wmin : w - 36;// �����
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
* @brief ��ռ�������������ʾ
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