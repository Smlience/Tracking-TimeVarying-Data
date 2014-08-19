#ifndef GRADIENTCALCULATOR_H
#define GRADIENTCALCULATOR_H

#include "util/Vector3.h"
#include "util/func.color.h"
#include "util/util.h"
#include <string.h>
/**
 * @brief 计算标量体数据的梯度
 * @param [out] grad float * 用于存放梯度的数组。在外部分配空间，尺寸至少为dim.x * dim.y * dim.z * sizeof(float) * 3
 * @param [in] data T * 标量体数据
 * @param [in] dim Vector3i 数据的维度
 * @date 2013/06/19
 */
template<typename T>
void calcGradientCentralDiff(float* grad, T* data, Vector3i dim)
{
    assert(grad != NULL && data != NULL);
    Vector3i pos;
    int xy = dim.x * dim.y;
    size_t idx_g = 0;
    size_t idx = 0;
	T v0, v1, v2, v3, v4, v5;
    memset(grad, 0, xy * dim.z * 3 * sizeof(float));
    // calculate gradient and the normalized direction
    for(pos.z = 0; pos.z < dim.z; ++pos.z)
    {
        for(pos.y = 0; pos.y < dim.y; ++pos.y)
        {
            for(pos.x = 0; pos.x < dim.x; ++pos.x)
            {
                // 此处边界取0。也可以换成data[idx]
                v0 = (pos.x != dim.x - 1) ? data[idx + 1] : 0;
                v1 = (pos.y != dim.y - 1) ? data[idx + dim.y] : 0;
                v2 = (pos.z != dim.z - 1) ? data[idx + xy] : 0;

                v3 = (pos.x != 0) ?	data[idx - 1]: 0;
                v4 = (pos.y != 0) ? data[idx - dim.x] : 0;
                v5 = (pos.z != 0) ? data[idx - xy] : 0;

                ++idx;

                grad[idx_g] = static_cast<float>(v0 - v3) / 2.0f;
                ++idx_g;
                grad[idx_g] = static_cast<float>(v1 - v4) / 2.0f;
                ++idx_g;
                grad[idx_g] = static_cast<float>(v2 - v5) / 2.0f;
                ++idx_g;
            }
        }
    }
}

template<typename T>
Vector3f calcGradientCentralDiffAt(T* pData, Vector3i dim, Vector3i pos)
{
    double gx, gy, gz;
    int index = pos.z * dim.x * dim.y + pos.y * dim.x + pos.x;
    T value = pData[index];

    if(pos.x > 0)
        if(pos.x < dim.x - 1) gx = (pData[index + 1] - pData[index - 1]) / 2.0;
        else gx = value - pData[index - 1];
    else gx = pData[index + 1] - value;

    if(pos.y > 0)
        if(pos.y < dim.y - 1) gy = (pData[index + dim.x] - pData[index - dim.x]) / 2.0;
        else gy = value - pData[index - dim.x];
    else gy = pData[index + dim.x] - value;

    if(pos.z > 0)
        if(pos.z < dim.z - 1) gz = (pData[index + dim.x * dim.y] - pData[index - dim.x * dim.y]) / 2.0;
        else gz = value - pData[index - dim.x * dim.y];
    else gz = pData[index + dim.x * dim.y] - value;

    return Vector3f(gx, gy, gz);
}

inline void gradientNormalize(float* data, Vector3i dim)
{
    float norm_;
    int len = dim.x * dim.y * dim.z * 3;
    for (int i = 0; i < len;)
    {
        norm_ = sqrt(data[i] * data[i] + data[i + 1] * data[i + 1] + data[i + 2] * data[i + 2]);
        if (norm_ > DOUBLE_EPSILON)
        {
            data[i++] /= norm_;
            data[i++] /= norm_;
            data[i++] /= norm_;
        }
        else
        {
            data[i++] = 0.0;
            data[i++] = 0.0;
            data[i++] = 0.0;
        }
    }
}

inline void gradientF2UC(float* src, unsigned char* dest, Vector3i dim)
{
    int len = dim.x * dim.y * dim.z * 3;
    for (int i = 0; i < len;)
    {
        dest[i] = (unsigned char)((src[i] + 1.0) / 2.0 * 255 + 0.5);
        ++i;
        dest[i] = (unsigned char)((src[i] + 1.0) / 2.0 * 255 + 0.5);
        ++i;
        dest[i] = (unsigned char)((src[i] + 1.0) / 2.0 * 255 + 0.5);
        ++i;
    }
}

inline void nonNormlizedGradientF2UC(float* src, unsigned char* dest, Vector3i dim)
{
	float norm_;
	int len = dim.x * dim.y * dim.z * 3;
	for (int i = 0; i < len;)
	{
		norm_ = sqrt(src[i] * src[i] + src[i + 1] * src[i + 1] + src[i + 2] * src[i + 2]);
		if (norm_ > DOUBLE_EPSILON)
		{
			dest[i] = (unsigned char)((src[i] / norm_ + 1.0) / 2.0 * 255 + 0.5);
			++i;
			dest[i] = (unsigned char)((src[i] / norm_ + 1.0) / 2.0 * 255 + 0.5);
			++i;
			dest[i] = (unsigned char)((src[i] / norm_ + 1.0) / 2.0 * 255 + 0.5);
			++i;
		}
		else
		{
			dest[i++] = 128;
			dest[i++] = 128;
			dest[i++] = 128;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
 * 计算彩色体数据的梯度
 * 采用向量数据中值差分的方法计算
 */
template<typename T>
void calcColorGradientCentralDiff(float* grad, T* data, Vector3i dim)
{
    assert(grad != NULL && data != NULL);
    Vector3i pos;
    int xy = dim.x * dim.y;
    size_t idx_g = 0;
    size_t idx = 0;
    size_t cidx;
    Vector3<T> v0, v1, v2, v3, v4, v5;
    memset(grad, 0, xy * dim.z * 3 * sizeof(float));

    // calculate gradient and the normalized direction
    for(pos.z = 0; pos.z < dim.z; ++pos.z)
    {
        for(pos.y = 0; pos.y < dim.y; ++pos.y)
        {
            for(pos.x = 0; pos.x < dim.x; ++pos.x)
            {
                // 此处边界取0。也可以换成data[idx]
                if (pos.x != dim.x - 1)
                {
                    cidx = (idx + 1) * 3;
                    v0.set(data[cidx], data[cidx + 1], data[cidx + 2]);
                }
                else
                {
                    v0.set(0, 0, 0);
                }

                if (pos.y != dim.y - 1)
                {
                    cidx = (idx + dim.y) * 3;
                    v1.set(data[cidx], data[cidx + 1], data[cidx + 2]);
                }
                else
                {
                    v1.set(0, 0, 0);
                }

                if (pos.z != dim.z - 1)
                {
                    cidx = (idx + xy) * 3;
                    v2.set(data[cidx], data[cidx + 1], data[cidx + 2]);
                }
                else
                {
                    v2.set(0, 0, 0);
                }

                if (pos.x != 0)
                {
                    cidx = (idx - 1) * 3;
                    v3.set(data[cidx], data[cidx + 1], data[cidx + 2]);
                }
                else
                {
                    v3.set(0, 0, 0);
                }

                if (pos.y != 0)
                {
                    cidx = (idx - dim.x) * 3;
                    v4.set(data[cidx], data[cidx + 1], data[cidx + 2]);
                }
                else
                {
                    v4.set(0, 0, 0);
                }

                if (pos.z != 0)
                {
                    cidx = (idx - xy) * 3;
                    v5.set(data[cidx], data[cidx + 1], data[cidx + 2]);
                }
                else
                {
                    v5.set(0, 0, 0);
                }

                ++idx;

                grad[idx_g] = (v0.norm() - v3.norm()) / 2.0f;
                ++idx_g;
                grad[idx_g] = (v1.norm() - v4.norm()) / 2.0f;
                ++idx_g;
                grad[idx_g] = (v2.norm() - v5.norm()) / 2.0f;
                ++idx_g;
            }
        }
    }
}

/**
 * 计算彩色体数据的梯度
 * 先将彩色体数据转为标度数据(RGB->Lab)，然后采用彩色数据中值差分的方法计算
 */
inline void calcColorLabGradientCentralDiff(float* grad, unsigned char* data, Vector3i dim)
{
    // 将RGB颜色转为Lab
    size_t len = dim.x * dim.y * dim.z * 3;
    float* lab = new float[len];
    float L, a, b;
    for (int i = 0; i < len; i += 3)
    {
        RGB2Lab3f(data[i], data[i + 1], data[i + 2], L, a, b);
        lab[i] = L;
        lab[i + 1] = a;
        lab[i + 2] = b;
    }
    calcColorGradientCentralDiff(grad, lab, dim);
    delete[] lab;
}
inline void calcColorLUVGradientCentralDiff(float* grad, unsigned char* data, Vector3i dim)
{
    // 将RGB颜色转为LUV
    size_t len = dim.x * dim.y * dim.z * 3;
    float* luv = new float[len];
    float L, U, V;
    for (int i = 0; i < len; i += 3)
    {
        RGB2LUVf(data[i], data[i + 1], data[i + 2], L, U, V);
        luv[i] = L;
        luv[i + 1] = U;
        luv[i + 2] = V;
    }
    calcColorGradientCentralDiff(grad, luv, dim);
    delete[] luv;
}
/**
 * 计算彩色体数据的梯度
 * 先将彩色体数据转为标度数据(RGB->Gray)，然后采用标量数据中值差分的方法计算
 */
inline void calcColorScalarGradientCentralDiff(float* grad, unsigned char* data, Vector3i dim)
{
    assert(grad != NULL && data != NULL);
    //根据原始体数据生成标量体数据
    size_t len = dim.x * dim.y * dim.z;
    unsigned char* scalarData = new unsigned char[len];
    for(size_t idx = 0, idx_s = 0; idx_s < len; ++idx_s, idx += 3)
    {
        scalarData[idx_s] = rgbToGray(data[idx], data[idx + 1], data[idx + 2]);
    }
    //根据标量体数据计算梯度
    calcGradientCentralDiff(grad, scalarData, dim);
    delete[] scalarData;

// 	assert(grad != NULL && data != NULL);
// 	Vector3i pos;
// 	int xy = dim.x * dim.y;
// 	size_t idx_g = 0;
// 	size_t idx = 0;
// 	size_t cidx;
// 	T v0, v1, v2, v3, v4, v5;
// 	memset(grad, 0, xy * dim.z * 3 * sizeof(float));
//
// 	// calculate gradient and the normalized direction
// 	for(pos.z = 0; pos.z < dim.z; ++pos.z)
// 	{
// 		for(pos.y = 0; pos.y < dim.y; ++pos.y)
// 		{
// 			for(pos.x = 0; pos.x < dim.x; ++pos.x)
// 			{
// 				// 此处边界取0。也可以换成data[idx]
// 				if (pos.x != dim.x - 1)
// 				{
// 					cidx = (idx + 1) * 3;
// 					v0 = rgbToGray(data[cidx], data[cidx + 1], data[cidx + 2]);
// 				}
// 				else
// 				{
// 					v0 = 0;
// 				}
//
// 				if (pos.y != dim.y - 1)
// 				{
// 					cidx = (idx + dim.y) * 3;
// 					v1 = rgbToGray(data[cidx], data[cidx + 1], data[cidx + 2]);
// 				}
// 				else
// 				{
// 					v1 = 0;
// 				}
//
// 				if (pos.z != dim.z - 1)
// 				{
// 					cidx = (idx + xy) * 3;
// 					v2 = rgbToGray(data[cidx], data[cidx + 1], data[cidx + 2]);
// 				}
// 				else
// 				{
// 					v2 = 0;
// 				}
//
// 				if (pos.x != 0)
// 				{
// 					cidx = (idx - 1) * 3;
// 					v3 = rgbToGray(data[cidx], data[cidx + 1], data[cidx + 2]);
// 				}
// 				else
// 				{
// 					v3 = 0;
// 				}
//
// 				if (pos.y != 0)
// 				{
// 					cidx = (idx - dim.x) * 3;
// 					v4 = rgbToGray(data[cidx], data[cidx + 1], data[cidx + 2]);
// 				}
// 				else
// 				{
// 					v4 = 0;
// 				}
//
// 				if (pos.z != 0)
// 				{
// 					cidx = (idx - xy) * 3;
// 					v5 = rgbToGray(data[cidx], data[cidx + 1], data[cidx + 2]);
// 				}
// 				else
// 				{
// 					v5 = 0;
// 				}
//
// 				++idx;
//
// 				grad[idx_g] = (v0 - v3) / 2.0f;
// 				++idx_g;
// 				grad[idx_g] = (v1 - v4) / 2.0f;
// 				++idx_g;
// 				grad[idx_g] = (v2 - v5) / 2.0f;
// 				++idx_g;
// 			}
// 		}
// 	}
}


//////////////////////////////////////////////////////////////////////////
// RCMG Gradient Calculate
template<typename T>
void swap(Vector3<T>& l, Vector3<T>& r)
{
	Vector3<T> tmp = l;
	l = r;
	r = tmp;
};

template<typename T>
T mid(const T& x, const T& y, const T& z)
{
	return (x < y ? (y < z ? y : (x < z ? z : x)) : (y > z ? y : (x > z ? z : x)));  
}

template<typename T, size_t N>
Vector3<T> RCMG_H1(Vector3<T> (&c)[N])
{
    int s = 1;
//     Vector3<T> e[N * N];
// 
//     for(int i = 0; i < N; ++i)
//     {
//         for(int j = 0; j < N; ++j)
//         {
//             e[i * N + j] = c[i] - c[j];
//         }
//     }
//     for(int i = 0; i < N * N; ++i) // 从大到小排序
//     {
//         for(int j = i + 1; j < N * N; ++j)
//         {
//             if (e[i].normSquared() < e[j].normSquared())
//             {
// //                 Vector3<T> tmp = e[i];
// //                 e[i] = e[j];
// //                 e[j] = tmp;
// 				swap(e[i], e[j]);
//             }
//         }
//     }
	const int n = N * (N - 1) / 2;
	Vector3<T> e[n];
	int idx = 0;
    for(int i = 0; i < N; ++i)
    {
        for(int j = i + 1; j < N; ++j)
        {
            e[idx++] = c[i] - c[j];
        }
    }
    for(int i = 0; i < n; ++i) // 从大到小排序
    {
        for(int j = i + 1; j < n; ++j)
        {
            if (e[i].normSquared() < e[j].normSquared())
				swap(e[i], e[j]);
        }
    }
    return e[s];
}

template<typename T, size_t N>
float RCMG_H2(Vector3<T> (&c)[N])
{
    Vector3<T> e = RCMG_H1(c);
    return e.norm();
}

// Element-wise median
template<typename T, size_t N>
Vector3<T> RCMG_L1(Vector3<T> (&c)[N])
{
    T tmp;
    Vector3<T> cc[N];
    for (int i = 0; i < N; ++i)
    {
        cc[i] = c[i];
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = i + 1; j < N; ++j)
        {
            if (cc[i].x > cc[j].x)
            {
                tmp = cc[j].x;
                cc[j].x = cc[i].x;
                cc[i].x = tmp;
            }
            if (cc[i].y > cc[j].y)
            {
                tmp = cc[j].y;
                cc[j].y = cc[i].y;
                cc[i].y= tmp;
            }
            if (cc[i].z > cc[j].z)
            {
                tmp = cc[j].z;
                cc[j].z = cc[i].z;
                cc[i].z = tmp;
            }
        }
    }
    return cc[N/2];
}

template<typename T, size_t N>
float RCMG_L2(Vector3<T> (&c)[N])
{
    Vector3<T> median = RCMG_L1(c);
    return median.norm();
}

template<typename T, size_t N1, size_t N2>
float RCMG_A_Mean( float (&g)[N1], Vector3<T> (&c)[N2])
{
    float result = 0.0f;
    for (int i = 0; i < N1; ++i)
        result += g[i];
    result /= N1;

    Vector3<T> sum1 = 0;
    Vector3<T> sum2 = 0;
    int count = N2 / 2;
    for (int i = 0; i < count; ++i)
    {
        sum1 += c[i];
        sum2 += c[N2 - 1 - i];
    }
    int sign = (sum2 / count).norm() - (sum1 / count).norm();

    if (sign < 0)
        result = -result;

    return result;
}

template<size_t N1>
float mean( float (&g)[N1])
{
	float result = 0.0f;
	for (int i = 0; i < N1; ++i)
		result += g[i];
	return result /= N1;
}

// template<typename T, size_t N1, size_t N2>
// float RCMG_A_Mean_W3( float (&g)[N1], Vector3<T> (&c)[N2])
// {
// 	float result = 0.0f;
// 	for (int i = 0; i < N1; ++i)
// 		result += g[i];
// 	result /= N1;
// 
// 	int sign = c[2].norm() - c[0].norm();
// 	if (sign < 0)
// 		result = -result;
// 
// 	return result;
// }
// float RCMG_A_Max( float gx1, float gx2, Vector3f c[5] )
// {
// 	int sign = (((c[N - 2] + c[N - 1])/2).length() - ((c[0] + c[1])/2).length());
// 	float result = (gx1 > gx2 ? gx1 : gx2);
// 	if (sign < 0)
// 		result = -result;
// 	return result;
// }

template<typename T>
inline void calcColorGradientRCMG_W5(float* grad, T* data, Vector3i dim)
{
    assert(grad != NULL && data != NULL);
    Vector3i pos,pos_tmp;
    float gx, gy, gz;
    int idx;
    Vector3<T> win[5][5][5];
    Vector3<T> pass1[3][5][5];	// 第一遍计算后得到5x5的结果，共3组(主方向H,2个次方向L)
    Vector3<T> pass2[6][5];		// 第二遍计算后得到1x5的结果，共6组
    float pass3[6];				// 第三遍计算后得到每种情况下的结果，共6个结果
    Vector3<T> tmp_arr[5];

#ifdef BRUTEFORCE	// 测试用 ->
// 	int idx_cache;
// 	float* grad_cache = new float[dim.x * dim.y * 3 * 6];
// 	char filePath[256];
// 	sprintf(filePath, "leg_RCMG_5x5x5_10_Lab_BruteForce.grad");
// 	printf("Write to file [%s]\n", filePath);
// 	FILE* f = fopen(filePath, "wb+");
// 	if (f == NULL)
// 	{
// 		printf("File open error!\n");
// 		exit(0);
// 	}
#endif	// <- 测试用

    for(pos.z = 0; pos.z < dim.z; ++pos.z)
    {
        for(pos.y = 0; pos.y < dim.y; ++pos.y)
        {
            for(pos.x = 0; pos.x < dim.x; ++pos.x)
            {
#ifdef BRUTEFORCE
  //              idx_cache = (pos.y * dim.x + pos.x) * 3 * 6;
#endif
                //////////////////////////////////////////////////////////////////////////
                //获取5*5*5的窗口
                for (pos_tmp.z = -2; pos_tmp.z < 3; ++pos_tmp.z)
                {
                    for (pos_tmp.y = -2; pos_tmp.y < 3; ++pos_tmp.y)
                    {
                        for (pos_tmp.x = -2; pos_tmp.x < 3; ++pos_tmp.x)
                        {
                            if (pos.x + pos_tmp.x >= 0 && pos.x +  pos_tmp.x < dim.x &&
                                    pos.y + pos_tmp.y >= 0 && pos.y +  pos_tmp.y < dim.y &&
                                    pos.z + pos_tmp.z >= 0 && pos.z +  pos_tmp.z < dim.z)
                            {
                                idx = (pos.x + pos_tmp.x) + (pos.y + pos_tmp.y) * dim.x + (pos.z + pos_tmp.z) * dim.x * dim.y;
                                idx *= 3;
                                win[pos_tmp.z + 2][pos_tmp.y + 2][pos_tmp.x + 2].set(data[idx], data[idx + 1], data[idx + 2]);
                            }
                            else
                            {
                                win[pos_tmp.z + 2][pos_tmp.y + 2][pos_tmp.x + 2].set(0, 0, 0);
                            }
                        }
                    }
                }

                //////////////////////////////////////////////////////////////////////////
                // 基本思想：
                // 每次沿所要计算梯度的方向做H，另外两个方向做L，不同的HLL组合会有6种，得到6个结果，
				// 然后将6个结果聚合。
				//  - H1(x)L2(y)L3(z)  - L1(y)H2(x)L3(z)  - L1(y)L2(z)H3(x)
				//  - H1(x)L2(z)L3(y)  - L1(z)H2(x)L3(y)  - L1(z)L2(y)H3(x)
                //////////////////////////////////////////////////////////////////////////
                //计算x方向的梯度
                for (int i = 0; i < 5; ++i)
                {
                    for (int j = 0; j < 5; ++j)
                    {
                        pass1[0][i][j] = RCMG_H1(win[i][j]);//HLL

						for (int m = 0; m < 5; ++m)
							tmp_arr[m] = win[i][m][j];
                        pass1[1][i][j] = RCMG_L1(tmp_arr);

						for (int m = 0; m < 5; ++m)
							tmp_arr[m] = win[m][i][j];
                        pass1[2][i][j] = RCMG_L1(tmp_arr);
                    }
                }
                for (int i = 0; i < 5; ++i)
                {
                    for (int m = 0; m < 5; ++m)
                        tmp_arr[m] = pass1[0][m][i];
                    pass2[0][i] = RCMG_L1(pass1[0][i]);
                    pass2[1][i] = RCMG_L1(tmp_arr);

                    for (int m = 0; m < 5; ++m)
                        tmp_arr[m] = pass1[1][m][i];
                    pass2[2][i] = RCMG_H1(pass1[1][i]);
                    pass2[3][i] = RCMG_L1(tmp_arr);

                    for (int m = 0; m < 5; ++m)
                        tmp_arr[m] = pass1[2][m][i];
                    pass2[4][i] = RCMG_H1(pass1[2][i]);
                    pass2[5][i] = RCMG_L1(tmp_arr);
                }
                pass3[0] = RCMG_L2(pass2[0]);
                pass3[1] = RCMG_L2(pass2[1]);
                pass3[2] = RCMG_L2(pass2[2]);
                pass3[3] = RCMG_H2(pass2[3]);
                pass3[4] = RCMG_L2(pass2[4]);
                pass3[5] = RCMG_H2(pass2[5]);

#ifdef BRUTEFORCE
// 				// 测试用 ->
// 				for (int i = 0; i < 6; ++i)
// 					grad_cache[idx_cache + i] = RCMG_A_Mean(pass3, pass2[i]);
#else
                gx = RCMG_A_Mean(pass3, pass2[3]);//聚合结果
// 				gx = RCMG_A_Mean(pass3);
// 				if (win[2][2][3].x - win[2][2][1].x < 0)
// 				{
// 					gx = -gx;
// 				}
#endif
                //////////////////////////////////////////////////////////////////////////
                //计算y方向的梯度
                for (int i = 0; i < 5; ++i)
                {
                    for (int j = 0; j < 5; ++j)
                    {
                        for (int m = 0; m < 5; ++m)
                            tmp_arr[m] = win[i][m][j];
                        pass1[0][i][j] = RCMG_H1(tmp_arr);

                        pass1[1][i][j] = RCMG_L1(win[i][j]);

                        for (int m = 0; m < 5; ++m)
                            tmp_arr[m] = win[m][i][j];
                        pass1[2][i][j] = RCMG_L1(tmp_arr);
                    }
                }
                for (int i = 0; i < 5; ++i)
                {
                    for (int m = 0; m < 5; ++m)
                        tmp_arr[m] = pass1[0][m][i];
                    pass2[0][i] = RCMG_L1(pass1[0][i]);
                    pass2[1][i] = RCMG_L1(tmp_arr);

                    for (int m = 0; m < 5; ++m)
                        tmp_arr[m] = pass1[1][m][i];
                    pass2[2][i] = RCMG_H1(pass1[1][i]);
                    pass2[3][i] = RCMG_L1(tmp_arr);

                    for (int m = 0; m < 5; ++m)
                        tmp_arr[m] = pass1[2][m][i];
                    pass2[4][i] = RCMG_H1(tmp_arr);
                    pass2[5][i] = RCMG_L1(pass1[2][i]);
                }
                pass3[0] = RCMG_L2(pass2[0]);
                pass3[1] = RCMG_L2(pass2[1]);
                pass3[2] = RCMG_L2(pass2[2]);
                pass3[3] = RCMG_H2(pass2[3]);
                pass3[4] = RCMG_L2(pass2[4]);
                pass3[5] = RCMG_H2(pass2[5]);
#ifndef BRUTEFORCE
                gy = RCMG_A_Mean(pass3, pass2[3]);//聚合结果

// 				gy = RCMG_A_Mean(pass3);
// 				if (win[2][3][2].y - win[2][1][2].y < 0)
// 				{
// 					gy = -gy;
// 				}
#else
// 				// 测试用 ->
// 				for (int i = 0; i < 6; ++i)
// 					grad_cache[idx_cache + 6 + i] = RCMG_A_Mean(pass3, pass2[i]);
#endif // BRUTEFORCE
                //////////////////////////////////////////////////////////////////////////
                //计算z方向的梯度
                for (int i = 0; i < 5; ++i)
                {
                    for (int j = 0; j < 5; ++j)
                    {
                        for (int m = 0; m < 5; ++m)
                            tmp_arr[m] = win[m][i][j];
                        pass1[0][i][j] = RCMG_H1(tmp_arr);

                        pass1[1][i][j] = RCMG_L1(win[i][j]);

                        for (int m = 0; m < 5; ++m)
                            tmp_arr[m] = win[i][m][j];
                        pass1[2][i][j] = RCMG_L1(tmp_arr);
                    }
                }
                for (int i = 0; i < 5; ++i)
                {
                    for (int m = 0; m < 5; ++m)
                        tmp_arr[m] = pass1[0][m][i];
                    pass2[0][i] = RCMG_L1(pass1[0][i]);
                    pass2[1][i] = RCMG_L1(tmp_arr);

                    for (int m = 0; m < 5; ++m)
                        tmp_arr[m] = pass1[1][m][i];
                    pass2[2][i] = RCMG_H1(tmp_arr);
                    pass2[3][i] = RCMG_L1(pass1[1][i]);

                    for (int m = 0; m < 5; ++m)
                        tmp_arr[m] = pass1[2][m][i];
                    pass2[4][i] = RCMG_H1(tmp_arr);
                    pass2[5][i] = RCMG_L1(pass1[2][i]);
                }
                pass3[0] = RCMG_L2(pass2[0]);
                pass3[1] = RCMG_L2(pass2[1]);
                pass3[2] = RCMG_L2(pass2[2]);
                pass3[3] = RCMG_H2(pass2[3]);
                pass3[4] = RCMG_L2(pass2[4]);
                pass3[5] = RCMG_H2(pass2[5]);
#ifndef BRUTEFORCE
                gz = RCMG_A_Mean(pass3, pass2[3]);//聚合结果

// 				gz = RCMG_A_Mean(pass3);
// 				if (win[3][2][2].z - win[1][2][2].z < 0)
// 				{
// 					gz = -gz;
// 				}
#else
// 				for (int i = 0; i < 6; ++i)
// 					grad_cache[idx_cache + 12 + i] = RCMG_A_Mean(pass3, pass2[i]);
#endif
                //////////////////////////////////////////////////////////////////////////
                idx = (pos.x + pos.y * dim.x + pos.z * dim.x * dim.y) * 3;

#ifdef BRUTEFORCE // 测试用 ->
// 				grad[idx + 0] = grad_cache[idx_cache + 0];
// 				grad[idx + 1] = grad_cache[idx_cache + 8];
// 				grad[idx + 2] = grad_cache[idx_cache + 15];
//             }
//         }
// 		// 测试用 ->
// 		fwrite(grad_cache, sizeof(float), dim.x * dim.y * 3 * 6, f);
//     }
// 	// 测试用 ->
// 	fclose(f);
// 	delete[] grad_cache;
#else
                grad[idx + 0] = gx;
                grad[idx + 1] = gy;
                grad[idx + 2] = gz;
			}
		}
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Window = 3
#include <time.h>

extern clock_t th1, tl1, ta, tsample, ttotal, tbegin, tbegin2, tpass1, tpass2, tpass3;

template<typename T>
Vector3<T> RCMG_H1_W3(Vector3<T> c[3])
{
	tbegin = clock();
	int s = 1;
	Vector3<T> e[3] = {	c[0] - c[1], c[0] - c[2], c[1] - c[2]};

	T en[3] = {	e[0].normSquared(), e[1].normSquared(), e[2].normSquared() };
	int idx[3] = {0, 1, 2}, tmp;

	for(int i = 0; i < 3; ++i) // 从大到小排序
	{
		for(int j = i + 1; j < 3; ++j)
		{
			if (en[idx[i]] < en[idx[i]])
			{
				//swap(e[i], e[j]);
				tmp = idx[i];
				idx[i] = idx[j];
				idx[j] = tmp;
			}
		}
	}
	th1 += clock() - tbegin;
	return e[s];
}

template<typename T>
float RCMG_H2_W3(Vector3<T> c[3])
{
	return RCMG_H1_W3(c).norm();
}

// Element-wise median
template<typename T>
Vector3<T> RCMG_L1_W3(Vector3<T> c[3])
{
	tbegin = clock();
	Vector3<T> rst;
	rst.x = mid(c[0].x, c[1].x, c[2].x);
	rst.y = mid(c[0].y, c[1].y, c[2].y);
	rst.z = mid(c[0].z, c[1].z, c[2].z);
	tl1 += clock() - tbegin;
	return rst;
}

template<typename T>
float RCMG_L2_W3(Vector3<T> c[3])
{
	return RCMG_L1_W3(c).norm();
}

template<typename T>
float RCMG_A_Mean_W3( float g[6], Vector3<T> c[3])
{
	tbegin = clock();
	float result = (g[0] + g[1] + g[2] + g[3] + g[4] + g[5]) / 6.0f;
	
	if (c[2].norm() < c[0].norm())
		result = -result;

	ta += clock() - tbegin;
	return result;
}

template<typename T>
float RCMG_A_Max_W3( float g[6], Vector3<T> c[3])
{
	tbegin = clock();
	float result = max(max(max(g[0], g[1]), max(g[2], g[3])), max(g[4], g[5]));

	if (c[2].norm() < c[0].norm())
		result = -result;

	ta += clock() - tbegin;
	return result;
}

template<typename T>
inline void calcColorGradientRCMG_W3(float* grad, T* data, Vector3i dim)
{
	assert(grad != NULL && data != NULL);

	printf("RCGM ing ... win size = 3x3x3\n");

	ttotal = clock();

	Vector3i pos,pos_tmp;
	float gx, gy, gz;
	int idx;
	Vector3<T> win[3][3][3];
	Vector3<T> pass1[3][3][3];	// 第一遍计算后得到3x3的结果，共3组(主方向H,2个次方向L)
	Vector3<T> pass2[6][3];		// 第二遍计算后得到1x3的结果，共6组
	float pass3[6];				// 第三遍计算后得到每种情况下的结果，共6个结果
	Vector3<T> tmp_arr[3];

#ifdef BRUTEFORCE
	int idx_cache;
	float* grad_cache = new float[dim.x * dim.y * 3 * 6];
	char filePath[256];
	sprintf(filePath, "leg_RCMG_3x3x3_03_Lab_BruteForce.grad");
	printf("Write to file [%s]\n", filePath);
	FILE* f = fopen(filePath, "wb+");
	if (f == NULL)
	{
		printf("File open error!\n");
		exit(0);
	}
#endif

	for(pos.z = 0; pos.z < dim.z; ++pos.z)
	{
		for(pos.y = 0; pos.y < dim.y; ++pos.y)
		{
			for(pos.x = 0; pos.x < dim.x; ++pos.x)
			{
#ifdef BRUTEFORCE
				idx_cache = (pos.y * dim.x + pos.x) * 3 * 6;
#endif
				//////////////////////////////////////////////////////////////////////////
				//获取3*3*3的窗口
				tbegin = clock();
				memset(win, 0, sizeof(Vector3<T>) * 27);
				for (pos_tmp.z = -1; pos_tmp.z < 2; ++pos_tmp.z)
				{
					for (pos_tmp.y = -1; pos_tmp.y < 2; ++pos_tmp.y)
					{
						for (pos_tmp.x = -1; pos_tmp.x < 2; ++pos_tmp.x)
						{
							if (pos.x + pos_tmp.x >= 0 && pos.x +  pos_tmp.x < dim.x &&
								pos.y + pos_tmp.y >= 0 && pos.y +  pos_tmp.y < dim.y &&
								pos.z + pos_tmp.z >= 0 && pos.z +  pos_tmp.z < dim.z)
							{
								idx = ((pos.x + pos_tmp.x) + (pos.y + pos_tmp.y) * dim.x + (pos.z + pos_tmp.z) * dim.x * dim.y) * 3;
								win[pos_tmp.z + 1][pos_tmp.y + 1][pos_tmp.x + 1].set(data[idx], data[idx + 1], data[idx + 2]);
							}
// 							else
// 							{
// 								win[pos_tmp.z + 1][pos_tmp.y + 1][pos_tmp.x + 1].set(0, 0, 0);
// 							}
						}
					}
				}
				tsample += clock() - tbegin;
				//////////////////////////////////////////////////////////////////////////
				// 基本思想：
				// 每次沿所要计算梯度的方向做H，另外两个方向做L，不同的HLL组合会有6种，得到6个结果，
				// 然后将6个结果聚合。
				//  - H1(x)L2(y)L3(z)  - L1(y)H2(x)L3(z)  - L1(y)L2(z)H3(x)
				//  - H1(x)L2(z)L3(y)  - L1(z)H2(x)L3(y)  - L1(z)L2(y)H3(x)
				//////////////////////////////////////////////////////////////////////////
				//计算x方向的梯度
				tbegin2 = clock();
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						pass1[0][i][j] = RCMG_H1_W3(win[i][j]);

						for (int m = 0; m < 3; ++m)
							tmp_arr[m] = win[i][m][j];
						pass1[1][i][j] = RCMG_L1_W3(tmp_arr);

						for (int m = 0; m < 3; ++m)
							tmp_arr[m] = win[m][i][j];
						pass1[2][i][j] = RCMG_L1_W3(tmp_arr);
					}
				}
				tpass1 += clock() - tbegin2;
				tbegin2 = clock();
				for (int i = 0; i < 3; ++i)
				{
					for (int m = 0; m < 3; ++m)
						tmp_arr[m] = pass1[0][m][i];
					pass2[0][i] = RCMG_L1_W3(pass1[0][i]);
					pass2[1][i] = RCMG_L1_W3(tmp_arr);

					for (int m = 0; m < 3; ++m)
						tmp_arr[m] = pass1[1][m][i];
					pass2[2][i] = RCMG_H1_W3(pass1[1][i]);
					pass2[3][i] = RCMG_L1_W3(tmp_arr);

					for (int m = 0; m < 3; ++m)
						tmp_arr[m] = pass1[2][m][i];
					pass2[4][i] = RCMG_H1_W3(pass1[2][i]);
					pass2[5][i] = RCMG_L1_W3(tmp_arr);
				}
				tpass2 += clock() - tbegin2;
				tbegin2 = clock();
				pass3[0] = RCMG_L2_W3(pass2[0]);
				pass3[1] = RCMG_L2_W3(pass2[1]);
				pass3[2] = RCMG_L2_W3(pass2[2]);
				pass3[3] = RCMG_H2_W3(pass2[3]);
				pass3[4] = RCMG_L2_W3(pass2[4]);
				pass3[5] = RCMG_H2_W3(pass2[5]);
				tpass3 += clock() - tbegin2;
#ifdef BRUTEFORCE 
				// 测试用 ->
				for (int i = 0; i < 6; ++i)
					grad_cache[idx_cache + i] = RCMG_A_Mean_W3(pass3, pass2[i]);
#else
				gx = RCMG_A_Mean_W3(pass3, pass2[3]);//聚合结果
				//gx = RCMG_A_Max_W3(pass3, pass2[3]);//聚合结果
#endif
				//////////////////////////////////////////////////////////////////////////
				//计算y方向的梯度
				tbegin2 = clock();
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						for (int m = 0; m < 3; ++m)
							tmp_arr[m] = win[i][m][j];
						pass1[0][i][j] = RCMG_H1_W3(tmp_arr);

						pass1[1][i][j] = RCMG_L1_W3(win[i][j]);

						for (int m = 0; m < 3; ++m)
							tmp_arr[m] = win[m][i][j];
						pass1[2][i][j] = RCMG_L1_W3(tmp_arr);
					}
				}
				tpass1 += clock() - tbegin2;
				tbegin2 = clock();
				for (int i = 0; i < 3; ++i)
				{
					for (int m = 0; m < 3; ++m)
						tmp_arr[m] = pass1[0][m][i];
					pass2[0][i] = RCMG_L1_W3(pass1[0][i]);
					pass2[1][i] = RCMG_L1_W3(tmp_arr);

					for (int m = 0; m < 3; ++m)
						tmp_arr[m] = pass1[1][m][i];
					pass2[2][i] = RCMG_H1_W3(pass1[1][i]);
					pass2[3][i] = RCMG_L1_W3(tmp_arr);

					for (int m = 0; m < 3; ++m)
						tmp_arr[m] = pass1[2][m][i];
					pass2[4][i] = RCMG_H1_W3(tmp_arr);
					pass2[5][i] = RCMG_L1_W3(pass1[2][i]);
				}
				tpass2 += clock() - tbegin2;
				tbegin2 = clock();
				pass3[0] = RCMG_L2_W3(pass2[0]);
				pass3[1] = RCMG_L2_W3(pass2[1]);
				pass3[2] = RCMG_L2_W3(pass2[2]);
				pass3[3] = RCMG_H2_W3(pass2[3]);
				pass3[4] = RCMG_L2_W3(pass2[4]);
				pass3[5] = RCMG_H2_W3(pass2[5]);
				tpass3 += clock() - tbegin2;
#ifdef BRUTEFORCE
				// 测试用 ->
				for (int i = 0; i < 6; ++i)
					grad_cache[idx_cache + 6 + i] = RCMG_A_Mean_W3(pass3, pass2[i]);
#else
				gy = RCMG_A_Mean_W3(pass3, pass2[3]);//聚合结果
				//gy = RCMG_A_Max_W3(pass3, pass2[3]);//聚合结果
#endif

				//////////////////////////////////////////////////////////////////////////
				//计算z方向的梯度
				tbegin2 = clock();
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						for (int m = 0; m < 3; ++m)
							tmp_arr[m] = win[m][i][j];
						pass1[0][i][j] = RCMG_H1_W3(tmp_arr);

						pass1[1][i][j] = RCMG_L1_W3(win[i][j]);

						for (int m = 0; m < 3; ++m)
							tmp_arr[m] = win[i][m][j];
						pass1[2][i][j] = RCMG_L1_W3(tmp_arr);
					}
				}
				tpass1 += clock() - tbegin2;
				tbegin2 = clock();
				for (int i = 0; i < 3; ++i)
				{
					for (int m = 0; m < 3; ++m)
						tmp_arr[m] = pass1[0][m][i];
					pass2[0][i] = RCMG_L1_W3(pass1[0][i]);
					pass2[1][i] = RCMG_L1_W3(tmp_arr);

					for (int m = 0; m < 3; ++m)
						tmp_arr[m] = pass1[1][m][i];
					pass2[2][i] = RCMG_H1_W3(tmp_arr);
					pass2[3][i] = RCMG_L1_W3(pass1[1][i]);

					for (int m = 0; m < 3; ++m)
						tmp_arr[m] = pass1[2][m][i];
					pass2[4][i] = RCMG_H1_W3(tmp_arr);
					pass2[5][i] = RCMG_L1_W3(pass1[2][i]);
				}
				tpass2 += clock() - tbegin2;
				tbegin2 = clock();
				pass3[0] = RCMG_L2_W3(pass2[0]);
				pass3[1] = RCMG_L2_W3(pass2[1]);
				pass3[2] = RCMG_L2_W3(pass2[2]);
				pass3[3] = RCMG_H2_W3(pass2[3]);
				pass3[4] = RCMG_L2_W3(pass2[4]);
				pass3[5] = RCMG_H2_W3(pass2[5]);
				tpass3 += clock() - tbegin2;
#ifdef BRUTEFORCE
				// 测试用 ->
				for (int i = 0; i < 6; ++i)
					grad_cache[idx_cache + 12 + i] = RCMG_A_Mean_W3(pass3, pass2[i]);
#else
				gz = RCMG_A_Mean_W3(pass3, pass2[3]);//聚合结果
				//gz = RCMG_A_Max_W3(pass3, pass2[3]);//聚合结果
#endif
				//////////////////////////////////////////////////////////////////////////
				idx = (pos.x + pos.y * dim.x + pos.z * dim.x * dim.y) * 3;
#ifdef BRUTEFORCE
				grad[idx + 0] = grad_cache[idx_cache + 3];
				grad[idx + 1] = grad_cache[idx_cache + 8];
				grad[idx + 2] = grad_cache[idx_cache + 15];
#else
				grad[idx + 0] = gx;
				grad[idx + 1] = gy;
				grad[idx + 2] = gz;
#endif
			}
		}
#ifdef BRUTEFORCE
		// 测试用 ->
		fwrite(grad_cache, sizeof(float), dim.x * dim.y * 3 * 6, f);
#endif
	}
#ifdef BRUTEFORCE
	// 测试用 ->
	fclose(f);
	delete[] grad_cache;
#endif
	printf("\n");
	printf("--------------------\n");
	printf("|    Time Spend    |\n");
	printf("--------------------\n");
	printf("| Total  | %7d |\n", clock() - ttotal);
	printf("| Pass1  | %7d |\n", tpass1);
	printf("| Pass2  | %7d |\n", tpass2);
	printf("| Pass3  | %7d |\n", tpass3);
	printf("| Sample | %7d |\n", tsample);
	printf("| H1     | %7d |\n", th1);
	printf("| L1     | %7d |\n", tl1);
	printf("| A      | %7d |\n", ta);
	printf("--------------------\n");
}

template<typename T>
inline void calcColorLabGradientRCMG(float* grad, T* data, Vector3i dim, int win = 3)
{
	// 将RGB颜色转为LUV
	size_t len = dim.x * dim.y * dim.z * 3;
	float* lab = new float[len];
	float L, a, b;
	for (int i = 0; i < len; i += 3)
	{
		RGB2Lab3f(data[i], data[i + 1], data[i + 2], L, a, b);
		lab[i] = L;
		lab[i + 1] = a;
		lab[i + 2] = b;
	}
	if (win == 3)
	{
		calcColorGradientRCMG_W3(grad, lab, dim);
	} 
	else if (win == 5)
	{
		calcColorGradientRCMG_W5(grad, lab, dim);
	}
	delete[] lab;
}

template<typename T>
inline void calcColorLuvGradientRCMG(float* grad, T* data, Vector3i dim, int win = 3)
{
	// 将RGB颜色转为LUV
	size_t len = dim.x * dim.y * dim.z * 3;
	float* luv = new float[len];
	float L, U, V;
	for (int i = 0; i < len; i += 3)
	{
		RGB2LUVf(data[i], data[i + 1], data[i + 2], L, U, V);
		luv[i] = L;
		luv[i + 1] = U;
		luv[i + 2] = V;
	}
	if (win == 3)
	{
		calcColorGradientRCMG_W3(grad, luv, dim);
	} 
	else if (win == 5)
	{
		calcColorGradientRCMG_W5(grad, luv, dim);
	}
	delete[] luv;
}
// END // RCMG Gradient Calculate
//////////////////////////////////////////////////////////////////////////

inline void calcColorDistanceGradient(float* grad, unsigned char* data, Vector3i dim)
{
	assert(grad != NULL && data != NULL);
	// 将RGB颜色转为LUV
	size_t len = dim.x * dim.y * dim.z * 3;
	float* luv = new float[len];
	float L, U, V;
	for (int i = 0; i < len; i += 3)
	{
		RGB2LUVf(data[i], data[i + 1], data[i + 2], L, U, V);
		luv[i] = L;
		luv[i + 1] = U;
		luv[i + 2] = V;
	}

	Vector3i pos;
	int xy = dim.x * dim.y;
	size_t idx_g = 0;
	size_t idx = 0;
	size_t cidx;
	Vector3<float> v0, v1, v2, v3, v4, v5;
	memset(grad, 0, xy * dim.z * 3 * sizeof(float));

	// calculate gradient and the normalized direction
	for(pos.z = 0; pos.z < dim.z; ++pos.z)
	{
		for(pos.y = 0; pos.y < dim.y; ++pos.y)
		{
			for(pos.x = 0; pos.x < dim.x; ++pos.x)
			{
				// 此处边界取0。也可以换成data[idx]
				if (pos.x != dim.x - 1)
				{
					cidx = (idx + 1) * 3;
					v0.set(luv[cidx], luv[cidx + 1], luv[cidx + 2]);
				}
				else
				{
					v0.set(0, 0, 0);
				}

				if (pos.y != dim.y - 1)
				{
					cidx = (idx + dim.x) * 3;
					v1.set(luv[cidx], luv[cidx + 1], luv[cidx + 2]);
				}
				else
				{
					v1.set(0, 0, 0);
				}

				if (pos.z != dim.z - 1)
				{
					cidx = (idx + xy) * 3;
					v2.set(luv[cidx], luv[cidx + 1], luv[cidx + 2]);
				}
				else
				{
					v2.set(0, 0, 0);
				}

				if (pos.x != 0)
				{
					cidx = (idx - 1) * 3;
					v3.set(luv[cidx], luv[cidx + 1], luv[cidx + 2]);
				}
				else
				{
					v3.set(0, 0, 0);
				}

				if (pos.y != 0)
				{
					cidx = (idx - dim.x) * 3;
					v4.set(luv[cidx], luv[cidx + 1], luv[cidx + 2]);
				}
				else
				{
					v4.set(0, 0, 0);
				}

				if (pos.z != 0)
				{
					cidx = (idx - xy) * 3;
					v5.set(luv[cidx], luv[cidx + 1], luv[cidx + 2]);
				}
				else
				{
					v5.set(0, 0, 0);
				}

				++idx;

				grad[idx_g] = (v0 - v3).norm();
				//grad[idx_g] = (v0.norm() - v3.norm())/2;
				++idx_g;
				grad[idx_g] = (v1 - v4).norm();
				//grad[idx_g] = (v1.norm() - v4.norm())/2;
				++idx_g;
				grad[idx_g] = (v2 - v5).norm();
				//grad[idx_g] = (v2.norm() - v5.norm())/2;
				++idx_g;
			}
		}
	}
	delete[] luv;
}

template<typename T>
void calcSobelGradient(float* grad, T* data, Vector3i dim)
{
	ttotal = tsample = 0;
	Vector3f win[3][3][3];
	float winf[3][3][3];
	float gxf, gyf, gzf;
	Vector3i pos,pos_tmp;
	Vector3f gx, gy, gz;
	int idx = 0;
	static int dx[3][3][3] = {{{ 1, 0,-1},{ 2, 0,-2},{ 1, 0,-1}},
							  {{ 2, 0,-2},{ 4, 0,-4},{ 2, 0,-2}},
							  {{ 1, 0,-1},{ 2, 0,-2},{ 1, 0,-1}}};
	static int dy[3][3][3] = {{{ 1, 2, 1},{ 0, 0, 0},{-1,-2,-1}},
							  {{ 2, 4, 2},{ 0, 0, 0},{-2,-4,-2}},
							  {{ 1, 2, 1},{ 0, 0, 0},{-1,-2,-1}}};
	static int dz[3][3][3] = {{{ 1, 2, 1},{ 2, 4, 2},{ 1, 2, 1}}, //[0][0][0]~[0][2][2]
							  {{ 0, 0, 0},{ 0, 0, 0},{ 0, 0, 0}}, //[1][0][0]~[2][2][2]
							  {{-1,-2,-1},{-2,-4,-2},{-1,-2,-1}}};//[2][0][0]~[2][2][2]
	memset(grad, 0, sizeof(float) * dim.x * dim.y * dim.z * 3);
	for(pos.z = 0; pos.z < dim.z; ++pos.z)
	{
		for(pos.y = 0; pos.y < dim.y; ++pos.y)
		{
			for(pos.x = 0; pos.x < dim.x; ++pos.x)
			{
				//////////////////////////////////////////////////////////////////////////
				//获取3*3*3的窗口
				tbegin = clock();
				memset(win, 0, sizeof(Vector3<T>) * 27);
				memset(winf, 0, sizeof(float) * 27);
				for (pos_tmp.z = -1; pos_tmp.z < 2; ++pos_tmp.z)
				{
					for (pos_tmp.y = -1; pos_tmp.y < 2; ++pos_tmp.y)
					{
						for (pos_tmp.x = -1; pos_tmp.x < 2; ++pos_tmp.x)
						{
							if (pos.x + pos_tmp.x >= 0 && pos.x +  pos_tmp.x < dim.x &&
								pos.y + pos_tmp.y >= 0 && pos.y +  pos_tmp.y < dim.y &&
								pos.z + pos_tmp.z >= 0 && pos.z +  pos_tmp.z < dim.z)
							{
								idx = ((pos.x + pos_tmp.x) + (pos.y + pos_tmp.y) * dim.x + (pos.z + pos_tmp.z) * dim.x * dim.y) * 3;
								//win[pos_tmp.z + 1][pos_tmp.y + 1][pos_tmp.x + 1].set(data[idx], data[idx + 1], data[idx + 2]);
								winf[pos_tmp.z + 1][pos_tmp.y + 1][pos_tmp.x + 1] = sqrt(data[idx] * data[idx] + data[idx + 1] * data[idx + 1] + data[idx + 2] * data[idx + 2]);
							}
						}
					}
				}
				tsample += clock() - tbegin;
				gx.set(0, 0, 0);
				gy.set(0, 0, 0);
				gz.set(0, 0, 0);
				gxf = gyf = gzf = 0;
				for (pos_tmp.z = 0; pos_tmp.z < 3; ++pos_tmp.z)
				{
					for (pos_tmp.y = 0; pos_tmp.y < 3; ++pos_tmp.y)
					{
						for (pos_tmp.x = 0; pos_tmp.x < 3; ++pos_tmp.x)
						{
// 							gx += win[pos_tmp.z][pos_tmp.y][pos_tmp.x] * dx[pos_tmp.z][pos_tmp.y][pos_tmp.x];
// 							gy += win[pos_tmp.z][pos_tmp.y][pos_tmp.x] * dy[pos_tmp.z][pos_tmp.y][pos_tmp.x];
// 							gz += win[pos_tmp.z][pos_tmp.y][pos_tmp.x] * dz[pos_tmp.z][pos_tmp.y][pos_tmp.x];
							gxf += winf[pos_tmp.z][pos_tmp.y][pos_tmp.x] * dx[pos_tmp.z][pos_tmp.y][pos_tmp.x];
							gyf += winf[pos_tmp.z][pos_tmp.y][pos_tmp.x] * dy[pos_tmp.z][pos_tmp.y][pos_tmp.x];
							gzf += winf[pos_tmp.z][pos_tmp.y][pos_tmp.x] * dz[pos_tmp.z][pos_tmp.y][pos_tmp.x];
						}
					}
				}
// 				grad[idx++] = gx.norm();
// 				grad[idx++] = gy.norm();
// 				grad[idx++] = gz.norm();
				idx = (pos.x + pos.y * dim.x + pos.z * dim.x * dim.y) * 3;
				grad[idx] = gxf;
				grad[idx + 1] = gyf;
				grad[idx + 2] = gzf;
				ttotal += clock() - tbegin;
			}
		}
	}
	printf("\n");
	printf("--------------------\n");
	printf("|    Time Spend    |\n");
	printf("--------------------\n");
	printf("| Total  | %7d |\n", ttotal);
	printf("| Sample | %7d |\n", tsample);
	printf("--------------------\n");
}

inline void calcLUVSobelGradient(float* grad, unsigned char* data, Vector3i dim)
{
	size_t len = dim.x * dim.y * dim.z * 3;
	float* luv = new float[len];
	float L, U, V;
	for (int i = 0; i < len; i += 3)
	{
		RGB2LUVf(data[i], data[i + 1], data[i + 2], L, U, V);
		luv[i] = L;
		luv[i + 1] = U;
		luv[i + 2] = V;
	}

	calcSobelGradient(grad, luv, dim);

	delete []luv;
}
#endif // GRADIENTCALCULATOR_H