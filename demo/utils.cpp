/****************************************************
* @file uitls.cpp
* @brief 工具类函数放在此文件内
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

/**
* @brief 删库跑路
* @param [in] index 保存每个工件信息的向量
*/
void CdemoDlg::freepool(vector<struct Pool1> index)
{
	int i, h;
	int** pool;
	h = m_imageGray.GetHeight();
	for (auto it = index.begin(); it != index.end(); it++)
	{
		pool = (*it).water;
		//释放用于保存仅包含一个工件的图像的二维数组
		for (i = 0; i < h; i++)
		{
			free(pool[i]);
		}
		free(pool);
	}
}