// pch.cpp: ��Ԥ�����ͷ��Ӧ��Դ�ļ�

#include "pch.h"
#include "digital.h"

// ��ʹ��Ԥ�����ͷʱ����Ҫʹ�ô�Դ�ļ���������ܳɹ���
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