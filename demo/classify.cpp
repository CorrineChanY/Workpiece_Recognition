/****************************************************
* @file classify.cpp
* @brief ��������
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

/**
* @brief �жϹ������
* @param [in] &index	����ͼ������ָ��
* @remarks				�ֱ���ԭͼ�Ͻ���
*/
void CdemoDlg::JudgePiece(vector<struct Pool1>& index)
{
	int i, a, b;
	// th: ��ֵ
	// distance: ����
	// dmax: �������ֵ
	// dmin: ������Сֵ
	double th, distance, dmax, dmin;
	int state;
	int poolnum = 0;
	struct Pool1* poolall = NULL;// ���飺������й����ṹ��
	int counts[6] = { 0 }; // �������� 0:Ӳ�� 1:��ľ 2:��ĸ 3:��˨ 4:�ݶ� 5:����
	wchar_t classes[6][10] = {
		L"Ӳ��", L"��ľ", L"��ĸ", L"��˨", L"�ݶ�", L"����"
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
	reset(); // �б�֮ǰ��������ʾ
	for (auto it = index.begin(); it != index.end(); it++)
	{
		// �˵����ȷָ��С����
		if (poolall[b].num > 1000)
		{
			if (poolall[b].num < Sth) // ���С����ֵ���÷�ֻ֧����ĸ���ݶ�����˨
			{
				// �������Ĳ��ڹ����ϣ�Ϊ��ĸ
				if (poolall[b].water[poolall[b].h0][poolall[b].w0] < 1)
				{
					(*it).type = Nut;
					counts[2]++;
				}
				// ���������ڹ����ϣ�Ϊ�ݶ�����˨
				else {
					dmin = dmax = sqrt((poolall[b].edge[0] - poolall[b].h0) * (poolall[b].edge[0] - poolall[b].h0) + (poolall[b].edge[1] - poolall[b].w0) * (poolall[b].edge[1] - poolall[b].w0));
					// ���� �������ĵ���Ե��� ���������С����
					for (i = 1; i < poolall[b].edgenum; i++)
					{
						distance = sqrt((poolall[b].edge[i * 2] - poolall[b].h0) * (poolall[b].edge[i * 2] - poolall[b].h0) + (poolall[b].edge[i * 2 + 1] - poolall[b].w0) * (poolall[b].edge[i * 2 + 1] - poolall[b].w0));
						if (distance < dmin) dmin = distance;
						if (distance > dmax) dmax = distance;
					}
					// ����ȴ�����ֵΪ��˨
					if (dmax / dmin > 6) {
						(*it).type = Bolt; // ��˨
						counts[3]++;
					}
					else {
						(*it).type = Screw; // �ݶ�
						counts[4]++;
					}
				}
			}
			else { // ���������ֵ���÷�ֻ֧��L�ˡ�Ӳ�ҡ���ľ

				// �������Ĳ��ڹ����ϣ�ΪL��
				if (poolall[b].water[poolall[b].h0][poolall[b].w0] < 1)
				{
					(*it).type = Wrench;
					counts[5]++;
				}
				else { // ���������ڹ����ϣ�ΪӲ�һ��ľ
					state = 0;
					th = 0;
					// ���б�Ե�㵽�������ľ����ƽ��ֵ
					for (i = 0; i < poolall[b].edgenum; i++)
					{
						th += pow(poolall[b].edge[i * 2] - poolall[b].h0, 2) + pow(poolall[b].edge[i * 2 + 1] - poolall[b].w0, 2);
					}
					th /= poolall[b].edgenum;
					// ��Ե�㵽�������ľ��� �� ��ֵ ���ϴ�ĵ����
					for (i = 0; i < poolall[b].edgenum; i++)
					{
						distance = pow(poolall[b].edge[i * 2] - poolall[b].h0, 2) + pow(poolall[b].edge[i * 2 + 1] - poolall[b].w0, 2);
						if (distance < 0.9 * th || distance > 1.1 * th)
						{
							state++;
						}
					}
					// ���б�Ե�㵽�������ľ��������ȣ�ΪӲ��
					if (state < 150) {
						(*it).type = Coin;
						counts[0]++;
					}
					else { // ����Ϊ��ľ
						(*it).type = Block;
						counts[1]++;
					}
				}
			}

			// �����ǰ���������
			GetDlgItem(1018 + a)->SetWindowTextW(classes[(*it).type - 1]);
			// �����ǰ���������
			string str = to_string((*it).num);
			CString cstr;
			cstr = str.c_str();
			GetDlgItem(1038 + a)->SetWindowTextW(cstr);
			a++;
		}
		b++;
	}

	//�����������
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