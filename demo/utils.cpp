/****************************************************
* @file uitls.cpp
* @brief �����ຯ�����ڴ��ļ���
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

/**
* @brief ɾ����·
* @param [in] index ����ÿ��������Ϣ������
*/
void CdemoDlg::freepool(vector<struct Pool1> index)
{
	int i, h;
	int** pool;
	h = m_imageGray.GetHeight();
	for (auto it = index.begin(); it != index.end(); it++)
	{
		pool = (*it).water;
		//�ͷ����ڱ��������һ��������ͼ��Ķ�ά����
		for (i = 0; i < h; i++)
		{
			free(pool[i]);
		}
		free(pool);
	}
}