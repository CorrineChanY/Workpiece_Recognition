// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
#include "digital.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
void Digital::Image_Gray(MVImage m_image, MVImage m_imageDid)
{
	int w;
	int h;
	unsigned char* p = (unsigned char*)m_image.GetBits();
	unsigned char* pDst = (unsigned char*)m_image.GetBits();
	int i, j;

	w = m_image.GetWidth();
	h = m_image.GetHeight();

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			if (1)
			{
				if (*p < 255)
				{
					//*pDst = 0;
					*p = 0;
				}
				else
				{
					//*pDst = 255;
					*p = 255;
				}
				if (0)
				{
					*pDst = 255 - *pDst;
				}
			}
			else
			{
				if (1)
				{
					*pDst = 255 - *p;
				}
			}
			pDst++;
			p++;
		}
	}

}