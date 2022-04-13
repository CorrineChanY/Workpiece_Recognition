/****************************************************
* @file classify.cpp
* @brief 工件分类
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

/**
* @brief 判断工件类别
* @param [in] &index	工件图像链表指针
* @remarks				分别在原图上进行
*/
void CdemoDlg::JudgePiece(vector<struct Pool1>& index)
{
	int i, a, b;
	// th: 阈值
	// distance: 距离
	// dmax: 距离最大值
	// dmin: 距离最小值
	double th, distance, dmax, dmin;
	int state;
	int poolnum = 0;
	struct Pool1* poolall = NULL;// 数组：存放所有工件结构体
	int counts[6] = { 0 }; // 各类别计数 0:硬币 1:积木 2:螺母 3:螺栓 4:螺钉 5:扳手
	wchar_t classes[6][10] = {
		L"硬币", L"积木", L"螺母", L"螺栓", L"螺钉", L"扳手"
	};

	poolnum = (int)(index.size());
	poolall = (struct Pool1*)malloc(poolnum * sizeof(struct Pool1));

	i = 0;
	for (auto it = index.begin(); it != index.end(); it++)
	{
		poolall[i] = (*it);
		i++;
	}

	a = 0;
	b = 0;
	reset(); // 判别之前清空输出显示
	for (auto it = index.begin(); it != index.end(); it++)
	{
		// 滤掉过度分割的小区域
		if (poolall[b].num > 1000)
		{
			if (poolall[b].num < Sth) // 面积小于阈值，该分支只有螺母、螺钉、螺栓
			{
				// 几何中心不在工件上，为螺母
				if (poolall[b].water[poolall[b].h0][poolall[b].w0] < 1)
				{
					(*it).type = Nut;
					counts[2]++;
				}
				// 几何中心在工件上，为螺钉或螺栓
				else {
					dmin = dmax = sqrt((poolall[b].edge[0] - poolall[b].h0) * (poolall[b].edge[0] - poolall[b].h0) + (poolall[b].edge[1] - poolall[b].w0) * (poolall[b].edge[1] - poolall[b].w0));
					// 计算 几何中心到边缘点的 最大距离和最小距离
					for (i = 1; i < poolall[b].edgenum; i++)
					{
						distance = sqrt((poolall[b].edge[i * 2] - poolall[b].h0) * (poolall[b].edge[i * 2] - poolall[b].h0) + (poolall[b].edge[i * 2 + 1] - poolall[b].w0) * (poolall[b].edge[i * 2 + 1] - poolall[b].w0));
						if (distance < dmin) dmin = distance;
						if (distance > dmax) dmax = distance;
					}
					// 长宽比大于阈值为螺栓
					if (dmax / dmin > 6) {
						(*it).type = Bolt; // 螺栓
						counts[3]++;
					}
					else {
						(*it).type = Screw; // 螺钉
						counts[4]++;
					}
				}
			}
			else { // 面积大于阈值，该分支只有L杆、硬币、积木

				// 几何中心不在工件上，为L杆
				if (poolall[b].water[poolall[b].h0][poolall[b].w0] < 1)
				{
					(*it).type = Wrench;
					counts[5]++;
				}
				else { // 几何中心在工件上，为硬币或积木
					state = 0;
					th = 0;
					// 所有边缘点到几何中心距离的平均值
					for (i = 0; i < poolall[b].edgenum; i++)
					{
						th += pow(poolall[b].edge[i * 2] - poolall[b].h0, 2) + pow(poolall[b].edge[i * 2 + 1] - poolall[b].w0, 2);
					}
					th /= poolall[b].edgenum;
					// 边缘点到几何中心距离 与 均值 相差较大的点个数
					for (i = 0; i < poolall[b].edgenum; i++)
					{
						distance = pow(poolall[b].edge[i * 2] - poolall[b].h0, 2) + pow(poolall[b].edge[i * 2 + 1] - poolall[b].w0, 2);
						if (distance < 0.9 * th || distance > 1.1 * th)
						{
							state++;
						}
					}
					// 所有边缘点到几何中心距离近似相等，为硬币
					if (state < 150) {
						(*it).type = Coin;
						counts[0]++;
					}
					else { // 否则为积木
						(*it).type = Block;
						counts[1]++;
					}
				}
			}

			// 输出当前工件的类别
			GetDlgItem(1018 + a)->SetWindowTextW(classes[(*it).type - 1]);
			// 输出当前工件的面积
			string str = to_string((*it).num);
			CString cstr;
			cstr = str.c_str();
			GetDlgItem(1038 + a)->SetWindowTextW(cstr);
			a++;
		}
		b++;
	}

	//输出各类别计数
	string strings[6];
	CString cstrings[6];
	a = 0;
	for (int i = 0; i < 7; i++)
	{
		if (i == 4)
			i++;
		strings[a] = to_string(counts[a]);
		cstrings[a] = strings[a].c_str();
		GetDlgItem(1011 + i)->SetWindowTextW(cstrings[a]);
		a++;
	}

	free(poolall);
}