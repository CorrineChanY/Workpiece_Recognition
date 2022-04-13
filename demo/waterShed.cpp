/****************************************************
* @file waterShed.cpp
* @brief 分水岭算法
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

int CdemoDlg::Watershed(vector<struct Pool1>& index)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();//原灰度图
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();//已经腐蚀的灰度图
	int w, h;//灰度图的长和宽
	int i, j, a, b, wh, count, count1, c, d;//循环计数变量
	double sum = 0;//像素平均值
	double sum1 = 0;
	struct Pool1 pool;//临时保存新发现的工件
	struct Pool1* pcur = NULL;
	struct Pool1* belong = NULL;
	int state = 0;
	int typenum = 1;//记录发现的工件序号
	int found = 0;//记录寻找种子时是否发现种子
	int poolnum = 0;//记录找到的工件个数
	struct Pool1* poolall = NULL;//用于保存从向量中取出的记录工件的结构体
	int allnum = 0;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	p = (unsigned char*)m_imageGray.GetBits();

	//求灰度图的像素平均值
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
	//求平均值以下的像素点的平均值
	for (i = 0; i < count; i++)
	{
		if (*p < (unsigned char)sum)
		{
			sum1 = sum1 + *p;
			count1++;
		}
		p++;
	}
	//对图片进行腐蚀处理
	Corrode(4, 40);
	sum1 = sum1 / count1;
	pDst = (unsigned char*)m_imageDid.GetBits();
	//遍历灰度图，寻找阈值以下未生长的像素点作为种子，对满足阈值的邻接像素点生长存储起来
	while (1)
	{
		found = 0;//标志未找到种子，循环中未找到种子则退出循环
		for (i = 100; i < h - 100; i++)//将边缘像素点过滤掉，防止图片的黑色边框影响判断
		{
			for (j = 100; j < w - 100; j++)
			{
				if (*(pDst + i * w + j) < 25)//若找到满足阈值条件的像素点
				{
					state = 0;
					//遍历每一个工件图像，判断像素点是否已经被生长
					for (auto it = index.begin(); it != index.end(); ++it)
					{
						if ((*it).water[i][j] == 1)
						{
							state++;
						}
					}
					if (state == 0)//若像素点未生长
					{
						allnum++;
						if (allnum > 25)//若找到的工件数超过25则跳出算法，认为图像存在问题
						{
							freepool(index);
							index.swap(vector<struct Pool1>());
							Wrong = 1;
							return 0;//算法出错
						}
						pool.symbel = typenum;//初始化工件存储结构体
						typenum++;
						index.push_back(pool);//将新的工件推入向量中存储
						grow(i, j, index, h, w, 30);//对找到的种子进行生长
						found = 1;//标志已找到种子
					}
				}
			}
		}
		if (found == 0)//若未找到种子，退出循环
			break;
	}

	//若发现有工件面积大于阈值，则算法报错，图像存在问题
	for (auto it = index.begin(); it != index.end(); it++)
	{
		if ((*it).num > 100000)
		{
			freepool(index);
			index.swap(vector<struct Pool1>());
			return 0;
		}
	}

	poolnum = (int)(index.size());//发现的工件数量
	poolall = (struct Pool1*)malloc(poolnum * sizeof(struct Pool1));//开辟空间用于保存工件结构体

	//循环生长模拟水位上涨，每次循环根据新的阈值对工件区域重新生长，从而扩大工件范围，当两个水池相遇溢出时则筑防水坝，从而分割工件
	for (a = 31; a < 45; a++)
	{
		c = 0;
		for (auto it1 = index.begin(); it1 != index.end(); it1++)//取出向量中的结构体
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
						growagain1(i, j, index, h, w, &(*(it)), a, a);//对每个工件重新生长，阈值抬升
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

	//最后一次生长，在生长过程中利用susan判断边界，提取特征
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
	free(poolall);//释放内存
	return 1;
}