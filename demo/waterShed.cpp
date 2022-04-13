/****************************************************
* @file waterShed.cpp
* @brief ��ˮ���㷨
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

int CdemoDlg::Watershed(vector<struct Pool1>& index)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();//ԭ�Ҷ�ͼ
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();//�Ѿ���ʴ�ĻҶ�ͼ
	int w, h;//�Ҷ�ͼ�ĳ��Ϳ�
	int i, j, a, b, wh, count, count1, c, d;//ѭ����������
	double sum = 0;//����ƽ��ֵ
	double sum1 = 0;
	struct Pool1 pool;//��ʱ�����·��ֵĹ���
	struct Pool1* pcur = NULL;
	struct Pool1* belong = NULL;
	int state = 0;
	int typenum = 1;//��¼���ֵĹ������
	int found = 0;//��¼Ѱ������ʱ�Ƿ�������
	int poolnum = 0;//��¼�ҵ��Ĺ�������
	struct Pool1* poolall = NULL;//���ڱ����������ȡ���ļ�¼�����Ľṹ��
	int allnum = 0;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	p = (unsigned char*)m_imageGray.GetBits();

	//��Ҷ�ͼ������ƽ��ֵ
	count = w * h;
	for (i = 0; i < count; i++)
	{
		sum = sum + *p;
		p++;
	}
	sum = sum / count;
	count = w * h;
	count1 = 0;
	p = (unsigned char*)m_imageGray.GetBits();
	//��ƽ��ֵ���µ����ص��ƽ��ֵ
	for (i = 0; i < count; i++)
	{
		if (*p < (unsigned char)sum)
		{
			sum1 = sum1 + *p;
			count1++;
		}
		p++;
	}
	//��ͼƬ���и�ʴ����
	Corrode(4, 40);
	sum1 = sum1 / count1;
	pDst = (unsigned char*)m_imageDid.GetBits();
	//�����Ҷ�ͼ��Ѱ����ֵ����δ���������ص���Ϊ���ӣ���������ֵ���ڽ����ص������洢����
	while (1)
	{
		found = 0;//��־δ�ҵ����ӣ�ѭ����δ�ҵ��������˳�ѭ��
		for (i = 100; i < h - 100; i++)//����Ե���ص���˵�����ֹͼƬ�ĺ�ɫ�߿�Ӱ���ж�
		{
			for (j = 100; j < w - 100; j++)
			{
				if (*(pDst + i * w + j) < 25)//���ҵ�������ֵ���������ص�
				{
					state = 0;
					//����ÿһ������ͼ���ж����ص��Ƿ��Ѿ�������
					for (auto it = index.begin(); it != index.end(); ++it)
					{
						if ((*it).water[i][j] == 1)
						{
							state++;
						}
					}
					if (state == 0)//�����ص�δ����
					{
						allnum++;
						if (allnum > 25)//���ҵ��Ĺ���������25�������㷨����Ϊͼ���������
						{
							freepool(index);
							index.swap(vector<struct Pool1>());
							Wrong = 1;
							return 0;//�㷨����
						}
						pool.symbel = typenum;//��ʼ�������洢�ṹ��
						typenum++;
						index.push_back(pool);//���µĹ������������д洢
						grow(i, j, index, h, w, 30);//���ҵ������ӽ�������
						found = 1;//��־���ҵ�����
					}
				}
			}
		}
		if (found == 0)//��δ�ҵ����ӣ��˳�ѭ��
			break;
	}

	//�������й������������ֵ�����㷨����ͼ���������
	for (auto it = index.begin(); it != index.end(); it++)
	{
		if ((*it).num > 100000)
		{
			freepool(index);
			index.swap(vector<struct Pool1>());
			return 0;
		}
	}

	poolnum = (int)(index.size());//���ֵĹ�������
	poolall = (struct Pool1*)malloc(poolnum * sizeof(struct Pool1));//���ٿռ����ڱ��湤���ṹ��

	//ѭ������ģ��ˮλ���ǣ�ÿ��ѭ�������µ���ֵ�Թ������������������Ӷ����󹤼���Χ��������ˮ���������ʱ������ˮ�ӣ��Ӷ��ָ��
	for (a = 31; a < 45; a++)
	{
		c = 0;
		for (auto it1 = index.begin(); it1 != index.end(); it1++)//ȡ�������еĽṹ��
		{
			poolall[c] = (*it1);
			c++;
		}
		c = 0;
		for (auto it = index.begin(); it != index.end(); it++)
		{
			state = 0;
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					if (poolall[c].water[i][j] > 0)
					{
						growagain1(i, j, index, h, w, &(*(it)), a, a);//��ÿ������������������ֵ̧��
						state = 1;
						break;
					}
				}
				if (state == 1)
					break;
			}
			c++;
		}
	}

	c = 0;
	for (auto it1 = index.begin(); it1 != index.end(); it1++)
	{
		poolall[c] = (*it1);
		c++;
	}

	//���һ������������������������susan�жϱ߽磬��ȡ����
	c = 0;
	for (auto it = index.begin(); it != index.end(); it++)
	{
		state = 0;
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				if (poolall[c].water[i][j] >= 1)
				{
					if (poolall[c].water[i + 2][j] >= 1)
					{
						grow_susan(i + 2, j, index, h, w, &(*(it)), a - 1, a);
						state = 1;
						break;
					}
				}
			}
			if (state == 1)
				break;
		}
		c++;
	}
	free(poolall);//�ͷ��ڴ�
	return 1;
}