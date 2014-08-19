#include "util/Vector4.h"

void transposeMatrix( double* mOut, const double* rhs )
{
	double m[4][4];
	memcpy(m, rhs, 16 * sizeof(double));
	mOut[0]  = m[0][0]; mOut[1]  = m[1][0]; mOut[2]  = m[2][0]; mOut[3]  = m[3][0];
	mOut[4]  = m[0][1]; mOut[5]  = m[1][1]; mOut[6]  = m[2][1]; mOut[7]  = m[3][1];
	mOut[8]  = m[0][2]; mOut[9]  = m[1][2]; mOut[10] = m[2][2]; mOut[11] = m[3][2];
	mOut[12] = m[0][3]; mOut[13] = m[1][3]; mOut[14] = m[2][3]; mOut[15] = m[3][3];
}

float invMatrix(double* mOut, const double* rhs)
{
	double m[4][4];
	memcpy(m, rhs, 16 * sizeof(double));
	int is[4];
	int js[4];
	float fDet = 1.0f;
	int f = 1;
	for (int k = 0; k < 4; k ++)
	{
		// 第一步，全选主元
		float fMax = 0.0f;
		for (int i = k; i < 4; i ++)
		{
			for (int j = k; j < 4; j ++)
			{
				const float f = abs(m[i][j]);
				if (f > fMax)
				{
					fMax = f;
					is[k] = i;
					js[k] = j;
				}
			}
		}
		if (abs(fMax) < 0.0001f)
			return 0;
		if (is[k] != k)
		{
			f = -f;
			std::swap(m[k][0], m[is[k]][0]);
			std::swap(m[k][1], m[is[k]][1]);
			std::swap(m[k][2], m[is[k]][2]);
			std::swap(m[k][3], m[is[k]][3]);
		}
		if (js[k] != k)
		{
			f = -f;
			std::swap(m[0][k], m[0][js[k]]);
			std::swap(m[1][k], m[1][js[k]]);
			std::swap(m[2][k], m[2][js[k]]);
			std::swap(m[3][k], m[3][js[k]]);
		}
		// 计算行列值
		fDet *= m[k][k];
		// 计算逆矩阵
		// 第二步
		m[k][k] = 1.0f / m[k][k];
		// 第三步
		for (int j = 0; j < 4; j ++)
		{
			if (j != k)
				m[k][j] *= m[k][k];
		}
		// 第四步
		for (int i = 0; i < 4; i ++)
		{
			if (i != k)
			{
				for (int j = 0; j < 4; j ++)
				{
					if (j != k)
						m[i][j] = m[i][j] - m[i][k] * m[k][j];
				}
			}
		}
		// 第五步
		for (int i = 0; i < 4; i ++)
		{
			if (i != k)
				m[i][k] *= -m[k][k];
		}
	}
	for (int k = 3; k >= 0; k --)
	{
		if (js[k] != k)
		{
			std::swap(m[k][0], m[js[k]][0]);
			std::swap(m[k][1], m[js[k]][1]);
			std::swap(m[k][2], m[js[k]][2]);
			std::swap(m[k][3], m[js[k]][3]);
		}
		if (is[k] != k)
		{
			std::swap(m[0][k], m[0][is[k]]);
			std::swap(m[1][k], m[1][is[k]]);
			std::swap(m[2][k], m[2][is[k]]);
			std::swap(m[3][k], m[3][is[k]]);
		}
	}
	memcpy(mOut, m, 16 * sizeof(double));
	return fDet * f;
}

Vector4d multiply( Vector4d p, const double* matrix )
{
	Vector4d p1;
	p1.x = matrix[0] * p.x + matrix[4] * p.y + matrix[8]  * p.z + matrix[12] * p.w;
	p1.y = matrix[1] * p.x + matrix[5] * p.y + matrix[9]  * p.z + matrix[13] * p.w;
	p1.z = matrix[2] * p.x + matrix[6] * p.y + matrix[10] * p.z + matrix[14] * p.w;
	p1.w = matrix[3] * p.x + matrix[7] * p.y + matrix[11] * p.z + matrix[15] * p.w;
	return p1;
}
void printMatrix( double* mIn, int w, int h )
{
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			printf("%lf ", mIn[i * w + j]);
		}
		printf("\n");
	}
}
