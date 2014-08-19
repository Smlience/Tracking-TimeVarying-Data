#include "Functions.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <gl/GL.h>

double calcImageEntropy( const unsigned char* pixels, int w, int h, int bpp )
{
	double entropy = 0.0;
	// ������ľ���Ϊͼ��
	int histogram[256];
	double p[256];
	double pixel_count = w * h;
	int size = w * h * bpp;
	int i;

	// ����
	memset(histogram, 0, 256 * sizeof(int));
	memset(p, 0, 256 * sizeof(double));

	// ͳ��ֱ��ͼ
	unsigned char gray;
	for(i = 0; i < size; i += bpp)
	{
		gray = rgbToGray(pixels[i], pixels[i + 1], pixels[i + 2]);
		histogram[gray] ++;
	}

	// ���ݶ������ͼ����
	for(i = 0; i < 256; i++)
	{
		// ���ֱ��ͼͳ�ƽ����ÿ��10��
// 		printf("%d ", histogram[i]);
// 		if (i % 10 == 9)
// 		{
// 			printf("\n");
// 		}
		p[i] = histogram[i] / pixel_count;
		if(p[i] != 0.0)
			entropy -= p[i] * (log(p[i])/log(2.0));
	}
	return entropy;
}
