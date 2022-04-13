/****************************************************
* @file preProcess.cpp
* @brief 图像预处理
*****************************************************/

#pragma once
#include "pch.h"
#include "demoDlg.h"

/**
* @brief 将原图像转换成灰度图
* @param  m_image
* @return m_image m_imageDid
*/
void CdemoDlg::Image_Gray()
{
	int w;
	int h;
	unsigned char* p = (unsigned char*)m_image.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDstd = (unsigned char*)m_imageDid.GetBits();

	w = m_image.GetWidth();
	h = m_image.GetHeight();

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			*pDst = (unsigned char)(0.299 * *p + 0.587 * *(p + 1) + 0.114 * *(p + 2));
			*pDstd = *pDst;
			pDst++;
			pDstd++;
			p += 3;
		}
	}

	pDst = (unsigned char*)m_imageGray.GetBits();
	pDstd = (unsigned char*)m_imageDid.GetBits();

	for (int i = 0; i < w; i++)
	{
		*(pDst + i) = 255;
		*(pDst + (h - 1) * w + i) = 255;
		*(pDstd + i) = 255;
		*(pDstd + (h - 1) * w + i) = 255;
	}
	for (int i = 0; i < h; i++)
	{
		*(pDst + i * w) = 255;
		*(pDst + i * w + w - 1) = 255;
		*(pDstd + i * w) = 255;
		*(pDstd + i * w + w - 1) = 255;
	}
}


/**
* @brief 腐蚀
* @param [in] num 腐蚀次数
* @param [in] th  阈值
* @return		  处理后的图像信息在m_imageDid和m_imageDid1
* @remarks		  在灰度图上进行
*/
void CdemoDlg::Corrode(int num, int th)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	unsigned char* pool = (unsigned char*)m_imageDid1.GetBits();
	unsigned char* pcur = NULL;
	int i, j, n;
	int w, h;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	// 给m_imageDid1赋初值
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*(pool + i * w + j) = *(p + i * w + j);
		}
	}

	if (num > 0)
	{
		// 进行num次腐蚀
		for (n = 0; n < num; n++)
		{
			for (i = 2; i < h - 2; i++)
			{
				for (j = 2; j < w - 2; j++)
				{
					pcur = pool + i * w + j;
					// 如果某点的8邻域内有任何一个像素的灰度值大于th，则将该点置成背景色(白色)
					// 处理后的图像为m_imageDid
					if (*(pcur - 1) > th || *(pcur + 1) > th ||
						*(pcur - w) > th || *(pcur + w) > th ||
						*(pcur - w - 1) > th || *(pcur - w + 1) > th ||
						*(pcur + w - 1) > th || *(pcur + w + 1) > th)
					{
						*(pDst + i * w + j) = 255;
					}
				}
			}
			// 修改m_imageDid1
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					*(pool + i * w + j) = *(pDst + i * w + j);
				}
			}
		}
	}
}

/**
* @brief 膨胀
* @param [in] num 膨胀次数
* @param [in] th  阈值
* @return		  处理后的图像信息在m_imageDid和m_imageDid1
* @remarks		  在灰度图m_imageGray上进行
*/
void CdemoDlg::Expand(int num, int th)
{
	unsigned char* p = (unsigned char*)m_imageGray.GetBits();
	unsigned char* pDst = (unsigned char*)m_imageDid.GetBits();
	unsigned char* pool = (unsigned char*)m_imageDid1.GetBits();
	unsigned char* pcur = NULL;
	int i, j, n;
	int w, h;

	w = m_imageGray.GetWidth();
	h = m_imageGray.GetHeight();

	// 给m_imageDid1赋初值
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*(pool + i * w + j) = *(p + i * w + j);
		}
	}

	if (num > 0)
	{
		// 进行num次腐蚀
		for (n = 0; n < num; n++)
		{
			for (i = 2; i < h - 2; i++)
			{
				for (j = 2; j < w - 2; j++)
				{
					pcur = pool + i * w + j;
					// 如果某点的8邻域内有任何一个像素的灰度值大于th，则将该点置成背景色(白色)
					// 处理后的图像为m_imageDid
					if (*(pcur - 1) < th || *(pcur + 1) < th ||
						*(pcur - w) < th || *(pcur + w) < th ||
						*(pcur - w - 1) < th || *(pcur - w + 1) < th ||
						*(pcur + w - 1) < th || *(pcur + w + 1) < th)
					{
						*(pDst + i * w + j) = th;
					}
				}
			}
			// 修改m_imageDid1
			for (i = 0; i < h; i++)
			{
				for (j = 0; j < w; j++)
				{
					*(pool + i * w + j) = *(pDst + i * w + j);
				}
			}
		}
	}
}