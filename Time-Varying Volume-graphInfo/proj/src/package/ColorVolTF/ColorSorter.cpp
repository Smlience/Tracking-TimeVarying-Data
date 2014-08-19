#include "ColorSorter.h"
#include "util/Functions.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

float distance(Vector3f v1, Vector3f v2)
{
	return ::distance(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
};

//////////////////////////////////////////////////////////////////////////
float distance(const ColorSample& c1, const ColorSample& c2)
{
// 	float alpha = 0.2;
// 	float beta = 0.8;
// 	float d_lab = ::distance(c1.lab, c2.lab);
// 	float d_hsv = (c1.hsv.x - c2.hsv.x);
// 	return alpha * d_lab + beta * d_hsv;
	return ::distance(c1.lab, c2.lab);
};

float distance_hsv(const ColorSample& c1, const ColorSample& c2)
{
	//return ::distance(c1.hsv, c2.hsv);
	return ::distance(c1.lab, c2.lab);
};

float distance_hue(const ColorSample& c1, const ColorSample& c2)
{
	//return fabs(c1.hsv.x - c2.hsv.x);
	return fabs(c1.lab.x - c2.lab.x);
};
//////////////////////////////////////////////////////////////////////////

float distance_lab(const ColorSample& c1, const ColorSample& c2)
{
	return ::distance(c1.lab, c2.lab);
};

float distance_lightness(const ColorSample& c1, const ColorSample& c2)
{
	return fabs(c1.lab.x - c2.lab.x);
};

void ColorSorter::sortBySSM( const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum /*= 256*/ )
{
	if (clusterNum % 2 == 1)
		printf("Cluster number is odd. Sorting may be not correctly performed.\n");
	int blockSize = clusterNum;
	int blockNum = 1;
	Vector3f* target = new Vector3f[clusterNum];
	Vector3f t_value, t_sum;
	Vector3f v1,v2;
	//float Gau[3] = {0.25, 0.5, 0.25}; //(1/sqrt(2 * M_PI) * pow(M_E, -0.5 * k * k));
	float Gau[3] = {0.24197, 0.39894, 0.24197};
	int idx,idx1,idx2;
	int r;//随机的步长，用于随机选取要交换的cell
	bool sorted;
	int count = 0;//统计block细分次数
	int tmp_idx;
	float argmin_before, argmin_after;
	srand(time(NULL));
	for (int i = 0; i < clusterNum; ++i)
	{
		minPath[i] = i;
	}

	while(blockSize > 1)
	{
		blockSize /= 2;
		blockNum *= 2;

		//////////////////////////////////////////////////////////////////////////
// 		for (int i = 0; i < blockNum; ++i)
// 		{
// 			float minDist = 999999;
// 			float tmpDist;
// 			idx = i * blockSize;
// 			//选择初始点，离(0,0,0)最近的点
// 			for (int j = 0; j < blockSize; ++j)
// 			{
// 				tmpDist = distance(clusters[minPath[idx + j]].lab, Vector3f(0, 0, 0));
// 				if (tmpDist < minDist)
// 				{
// 					idx1 = idx + j;
// 					minDist = tmpDist;
// 				}
// 			}
// 			tmp_idx = minPath[idx];
// 			minPath[idx] = minPath[idx1];
// 			minPath[idx1] = tmp_idx;
// 
// 			for (int j = 0; j < blockSize - 1; ++j)
// 			{
// 				minDist = 99999999;
// 				for (int k = j + 1; k < blockSize; ++k)
// 				{
// 					idx2 = idx + k;
// 					tmpDist = ::distance(clusters[minPath[idx + j]].lab, clusters[minPath[idx2]].lab);
// 					if (tmpDist < minDist)
// 					{
// 						idx1 = idx2;
// 						minDist = tmpDist;
// 					}
// 				}
// 				tmp_idx = minPath[idx + j + 1];
// 				minPath[idx + j + 1] = minPath[idx1];
// 				minPath[idx1] = tmp_idx;
// 			}
// 		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//对每个block计算Ti
		for (int i = 0; i < blockNum - 1; ++i)
		{
			t_value.set(0, 0, 0);
			idx = i * blockSize;
			for (int k = 0; k <= 1; ++k)
			{
				t_sum.set(0, 0, 0);
				for (int j = 0; j < blockSize; ++j)
				{
					//使用lab
					t_sum += clusters[idx + j].lab;

// 					//使用hsv
// 					t_sum += clusters[idx + j].hsv;

					//使用rgb
// 					t_sum.x += clusters[idx + j].color.r;
// 					t_sum.y += clusters[idx + j].color.g;
// 					t_sum.z += clusters[idx + j].color.b;
				}
				t_sum *= Gau[k + 1];
				t_value += t_sum;
			}
			target[i] = t_value / blockSize;
		}
// 		//处理右边界
// 		for (int k = -1; k <= 0; ++k)
// 		{
// 			idx = (blockNum - 1 + k) * blockSize;
// 			t_sum.set(0, 0, 0);
// 			for (int j = 0; j < blockSize; ++j)
// 			{
// 				t_sum += clusters[idx + j].lab;
// 			}
// 			t_sum *= Gau[k + 1];
// 			t_value += t_sum;
// 		}
// 		target[blockNum - 1] = (t_value + Vector3f(100, 0, 0) * blockSize * Gau[2]) / blockSize;

		//进行排序
		count = 0;
		int rb = 1;
		while(true)
		{
			sorted = true;
			for (int k = count % 2; k < blockNum - 1; k += 2)
			{
				//r = rand() % blockSize;
				//rb = rand() % (blockNum - k);
				for (int i = 0; i < blockSize; ++i)
				{
					idx = k * blockSize + i;
					if (idx == 0)
						continue;

					for (r = 0; r < blockSize; ++r)
					{
						idx1 = (k + rb) * blockSize + (i + r) % blockSize;


						//使用lab
						v1 = clusters[minPath[idx]].lab;
						v2 = clusters[minPath[idx1]].lab;

						//使用hsv
// 						v1 = clusters[minPath[idx]].hsv;
// 						v2 = clusters[minPath[idx1]].hsv;

						//使用rgb
	// 					Color4 c1 = clusters[minPath[idx]].color;
	// 					Color4 c2 = clusters[minPath[idx1]].color;
	// 					v1.set(c1.r, c1.g, c1.b);
	// 					v2.set(c2.r, c2.g, c2.b);

						argmin_before = (::distance(v1, target[k]) + ::distance(v2, target[k + 1]));
						argmin_after = (::distance(v2, target[k]) + ::distance(v1, target[k + 1]));

	// 					argmin_before = (v1.x - target[k].x) + (v2.x - target[k + 1].x);
	// 					argmin_after  = (v2.x - target[k].x) + (v1.x - target[k + 1].x);

						if (argmin_before < argmin_after)
						{
							//交换两个值
							tmp_idx = minPath[idx];
							minPath[idx] = minPath[idx1];
							minPath[idx1] = tmp_idx;
							sorted = false;
						}
					}
				}
			}

			if(sorted) break;
			++count;
		}

// 		//////////////////////////////////////////////////////////////////////////
// 		//第二次排序
// 		//对每个block计算Ti
// 		for (int i = 0; i < blockNum; ++i)
// 		{
// 			t_value.set(0, 0, 0);
// 			idx = i * blockSize;
// 			for (int k = -1; k <= 1; ++k)
// 			{
// 				t_sum.set(0, 0, 0);
// 				for (int j = 0; j < blockSize; ++j)
// 				{
// // 					//使用lab
// // 					t_sum += clusters[idx + j].lab;
// 
// 					//使用hsv
// 					t_sum += clusters[idx + j].hsv;
// 
// // 					//使用rgb
// // 					t_sum.x += clusters[idx + j].color.r;
// // 					t_sum.y += clusters[idx + j].color.g;
// // 					t_sum.z += clusters[idx + j].color.b;
// 				}
// 				t_sum *= Gau[k + 1];
// 				t_value += t_sum;
// 			}
// 			target[i] = t_value / blockSize;
// 		}
// 
// 		while(true)
// 		{
// 			sorted = true;
// 			for (int k = count % 2; k < blockNum - 1; k += 2)
// 			{
// 				//r = rand() % blockSize;
// 				rb = rand() % (blockNum - k - 1) + 1;
// 				for (r = 0; r < blockSize; ++r)
// 				for (int i = 0; i < blockSize; ++i)
// 				{
// 					idx = k * blockSize + i;
// 					idx1 = (k + rb) * blockSize + (i + r) % blockSize;
// 
// 					//使用lab
// // 					v1 = clusters[minPath[idx]].lab;
// // 					v2 = clusters[minPath[idx1]].lab;
// 
// 					//使用hsv
// 					v1 = clusters[minPath[idx]].hsv;
// 					v2 = clusters[minPath[idx1]].hsv;
// 
// 					//使用rgb
// // 					Color4 c1 = clusters[minPath[idx]].color;
// // 					Color4 c2 = clusters[minPath[idx1]].color;
// // 					v1.set(c1.r, c1.g, c1.b);
// // 					v2.set(c2.r, c2.g, c2.b);
// 
// // 					argmin_before = (::distance(v1, target[k]) + ::distance(v2, target[k + 1]));
// // 					argmin_after = (::distance(v2, target[k]) + ::distance(v1, target[k + 1]));
// 
// 					argmin_before = (v1.x - target[k].x) + (v2.x - target[k + 1].x);
// 					argmin_after  = (v2.x - target[k].x) + (v1.x - target[k + 1].x);
// 
// 					if (argmin_before < argmin_after)
// 					{
// 						//交换两个值
// 						tmp_idx = minPath[idx];
// 						minPath[idx] = minPath[idx1];
// 						minPath[idx1] = tmp_idx;
// 						sorted = false;
// 					}
// 				}
// 			}
// 
// 			if(sorted) break;
// 			++count;
// 		}
// 		//////////////////////////////////////////////////////////////////////////
	}
	delete[] target;
	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < clusterNum; ++i)
	{
		idxMap[minPath[i]] = i;
	}
}

//////////////////////////////////////////////////////////////////////////
void ColorSorter::sortByNearestLab( const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum /*= 256*/ )
{
	float minDist = 999999;
	float tmpDist = 0;
	bool* cflag = new bool[clusterNum]; //记录已经选取过的点
	int idx;

	//选择初始点，离(0,0,0)最近的点
	for (int i = 0; i < clusterNum; ++i)
	{
		cflag[i] = true;
		tmpDist = distance(clusters[i].lab, Vector3f(0, 0, 0));
		if (tmpDist < minDist)
		{
			minPath[0] = i;
			minDist = tmpDist;
		}
	}
	cflag[minPath[0]] = false;

	for (int i = 1; i < clusterNum; ++i)
	{
		minDist = 99999999;
		idx = minPath[i-1];
		for (int j = 0; j < clusterNum; ++j)
		{
			if (cflag[j])
			{
				tmpDist = distance_lab(clusters[j], clusters[idx]);
				if (tmpDist < minDist)
				{
					minDist = tmpDist;
					minPath[i] = j;
				}
			}
		}
		cflag[minPath[i]] = false;
	}
	for (int i = 0; i < clusterNum; ++i)
	{
		idxMap[minPath[i]] = i;
	}
	delete[] cflag;
}

/*
void ColorSorter::sortByVoronoi( const Cluster* clusters, int* minPath, int* idxMap, int clusterNum )
{
	//////////////////////////////////////////////////////////////////////////
	//生成voronoi图
	voro::container con(0, 100.0, -128.0, 128.0, -128.0, 128.0, 1, 1, 1, false, false, false, clusterNum);
	for (int i = 0; i < clusterNum; ++i)
	{
		con.put(i, clusters[i].lab.x, clusters[i].lab.y, clusters[i].lab.z);
	}
	voro::c_loop_all vl(con);
	voro::voronoicell_neighbor c;
	std::vector<int>* nv = new std::vector<int>[clusterNum]; //neighbor vector
	int* nc = new int[clusterNum];//neighbor count
	float* ng = new float[clusterNum * clusterNum]; //neighbor graph,ng[i][j]表示clusters[i]和clusters[j]的距离
	memset(nc, 0, sizeof(int) * clusterNum);
	memset(ng, 0, sizeof(float) * clusterNum * clusterNum);
	if(vl.start()) {
		do if(con.compute_cell(c,vl)) {
			//fprintf(fp,"// cell %d\n",id[vl.ijk][vl.q]);
			//pp=p[vl.ijk]+ps*vl.q;
			//c.draw_pov(*pp,pp[1],pp[2],fp);
			std::vector<int> neighbor;
			c.neighbors(neighbor);
			nc[vl.q] = neighbor.size();
			printf("[%3d][%2d] |", vl.q, nc[vl.q]);
			for (int i = 0; i < neighbor.size(); ++i)
			{
				if (neighbor[i] >= 0)
				{
					nv[vl.q].push_back(neighbor[i]);
					ng[vl.q * clusterNum + neighbor[i]] = distance(clusters[vl.q].lab, clusters[neighbor[i]].lab);
				}
				else
				{
					nc[vl.q] --;
				}
//				printf("%d,", neighbor[i]);
			}
//			printf("\b| [%2d]\n", nc[vl.q]);
		} while(vl.inc());
//		printf("\n");
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 寻找最短路(Hamilton Chain)
	//hamiltonPath();
	minVoronoiPath();
	//////////////////////////////////////////////////////////////////////////
	delete[] nv;
	delete[] nc;
	delete[] ng;
}

void ColorSorter::minVoronoiPath()
{
	printf("I haven't found a appropriate Hamilton Path algorithm (┬＿┬)\n");
}
*/

void ColorSorter::sortHierarchical( const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum /*= 256*/ )
{
	float minDist = 999999;
	float tmpDist = 0;
//	bool* cflag = new bool[clusterNum]; //记录已经选取过的点
	int idx, tmp;
	for (int i = 0; i < clusterNum; ++i)
	{
//		cflag[i] = true;
		minPath[i] = i;
	}

	//排序
	for (int i = 0; i < clusterNum - 1; ++i)
	{
		minDist = 999999;
		for (int j = i + 1; j < clusterNum; ++j)
		{
//			if(clusters[minPath[i]].hsv.x > clusters[minPath[j]].hsv.x)
//			if(clusters[minPath[i]].lab.x > clusters[minPath[j]].lab.x)
//			if(clusters[minPath[i]].lab.y > clusters[minPath[j]].lab.y)
// 			if(::distance(clusters[minPath[i]], clusters[minPath[0]]) >
// 				::distance(clusters[minPath[j]], clusters[minPath[0]]))
// 			{
// 				idx = minPath[i];
// 				minPath[i] = minPath[j];
// 				minPath[j] = idx;
// 			}
			tmpDist = ::distance_lab(clusters[minPath[i]], clusters[minPath[j]]);
			if(tmpDist < minDist)
			{
				minDist = tmpDist;
				idx = j;
			}
// 			else if(clusters[minPath[i]].hsv.x == clusters[minPath[j]].hsv.x)
// 			{
// 				if (clusters[minPath[i]].lab.x > clusters[minPath[j]].lab.x)
// 				{
// 					idx = minPath[i];
// 					minPath[i] = minPath[j];
// 					minPath[j] = idx;
// 				}
// 			}
		}

		tmp = minPath[i + 1];
		minPath[i + 1] = minPath[idx];
		minPath[idx] = tmp;
	}

// 	//按L排序
// 	int section_size = 32;
// 	for (int m = 0; m < clusterNum / section_size; ++m)
// 	{
// 		for (int i = m * section_size; i < (m + 1) * section_size; ++i)
// 		{
// 			for (int j = i + 1; j < (m + 1) * section_size; ++j)
// 			{
// 				if (Lab[minPath[i] * 3 + 2] > Lab[minPath[j] * 3 + 2])
// 				{
// 					idx = minPath[i];
// 					minPath[i] = minPath[j];
// 					minPath[j] = idx;
// 				}
// 			}
// 		}
// 	}

// 	std::vector< std::vector<int> > vCluster;
// 	bool flag;
// 	for (int i = 0; i < clusterNum; ++i)
// 	{
// 		flag = true;
// 		for (std::vector< std::vector<int> >::iterator it = vCluster.begin(); flag && it != vCluster.end(); ++it)
// 		{
// 			for (int j = 0; j < it->size(); ++j)
// 			{
// 				if (fabs(hsv[i * 3] - hsv[it->at(j) * 3]) < 0.1)
// 				{
// 					it->push_back(i);
// 					flag = false;
// 					break;
// 				}
// 			}
// 		}
// 		if (flag) //没有归类
// 		{
// 			std::vector<int> vC;
// 			vC.push_back(i);
// 			vCluster.push_back(vC);
// 		}
// 	}
// 	printf("Cluster Count: %d\n", vCluster.size());
// 	idx = 0;
// 	for (int i = 0; i < vCluster.size(); ++i)
// 	{
// 		std::vector<int> vC = vCluster[i];
// 		for (std::vector<int>::iterator it = vC.begin(); it != vC.end(); ++it)
// 		{
// 			minPath[idx++] = *it;
// 		}
// 	}
// 	return;
// 
// 	//第一个聚类为黑色，默认位置不改变
// 	minPath[0] = 0;
// 	cflag[0] = false;
// 
// 	for (int i = 1; i < clusterNum; ++i)
// 	{
// 		minDist = 99999999;
// 		idx = minPath[i-1] * 3;
// 		for (int j = 0; j < clusterNum; ++j)
// 		{
// 			if (cflag[j])
// 			{
// 				tmpDist = ::distance(Lab[j * 3], Lab[j * 3 + 1], Lab[j * 3 + 2],
// 					Lab[idx], Lab[idx + 1], Lab[idx + 2]);
// 				if (tmpDist < minDist)
// 				{
// 					minDist = tmpDist;
// 					minPath[i] = j;
// 				}
// 			}
// 		}
// 		cflag[minPath[i]] = false;
// 	}
/*
	//////////////////////////////////////////////////////////////////////////
	// 对Cluster进行聚类，种子点为6个颜色的Hue
	//cout<<"color num"<<tmp<<endl;//查看有多少个有效颜色样本点
	int* cc = new int[clusterNum];
	float seed[6] = {0.0f, 1.f / 6.0, 2.f / 6.0, 3.f / 6.0, 4.f / 6.0, 5.f / 6.0 };
	int min_tag;
	float min_distance,tmp_distance;
	int entry_count[6];
	float sum_hue[6];
	float hue_pre;
	int variance;
	int ter_condition = 1.0;//调整终止条件值 控制迭代精度 
	//int count_w=0;
	do
	{
		variance = 0;
		//count_w++;
		//计算每个Cluster的最近Seed
		for(int j = 0; j < clusterNum; ++j)
		{
			min_tag=0;
			min_distance = fabsf(hsv[j * 3] - seed[0]);

			for(int k = 1; k < 6; k++)
			{
				tmp_distance = fabsf(hsv[j * 3] - seed[k]);
				if(tmp_distance < min_distance)
				{
					min_tag = k;
					min_distance = tmp_distance;
				}
			}
			cc[j] = min_tag;
		}

		//recompute the cluster center;
		memset(entry_count, 8, sizeof(int) * 6);
		memset(sum_hue, 8, sizeof(float) * 6);
		for(int i = 0; i < clusterNum; ++i)
		{
			entry_count[cc[i]] ++;
			sum_hue[cc[i]] += hsv[i * 3];
		}

		for(int i = 0; i < 6; ++i)
		{
			hue_pre = seed[i];
			if(entry_count[i] > 0)
			{
				seed[i] = (sum_hue[i] + hue_pre) / entry_count[i];
			}
			if(variance < ter_condition)
			{
				variance += fabsf(hue_pre - seed[i]);
			}
		}
	}while(variance > ter_condition); //调整
	//////////////////////////////////////////////////////////////////////////
// 	minPath[0] = 0;
// 	cflag[0] = false;
	idx = 0;
	int idx_left;
	std::vector<int> vcc[6];
	for(int i = 0; i < clusterNum; ++i)
	{
		vcc[cc[i]].push_back(i);
	}

	for (int i = 0; i < 6; ++i)
	{
		while(vcc[i].size() > 0)
		{
			min_distance = 3.402823466e+38F;
			std::vector<int>::iterator iter = vcc[i].begin();
			std::vector<int>::iterator min_iter = vcc[i].begin();
			idx_left = (idx - 1) * 3;
	
			for (; iter != vcc[i].end(); ++iter)
			{
				if (idx_left < 0)
				{
					tmp_distance = ::distance(Lab[0], Lab[1], Lab[2],
											  Lab[(*iter) * 3], Lab[(*iter) * 3 + 1], Lab[(*iter) * 3 + 2]);
				}
				else
				{
					tmp_distance = ::distance(Lab[(*iter) * 3], Lab[(*iter) * 3 + 1], Lab[(*iter) * 3 + 2],
											  Lab[idx_left], Lab[idx_left + 1], Lab[idx_left + 2]);
				}
				if (tmp_distance < min_distance)
				{
					min_distance = tmp_distance;
					min_iter = iter;
				}
			}
			minPath[idx++] = *min_iter;
			vcc[i].erase(min_iter);
		}
	}
*/
	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < clusterNum; ++i)
	{
		idxMap[minPath[i]] = i;
	}
//	delete[] cflag;
//	delete[] cc;
}

void ColorSorter::sort(const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum)
{
	sortByNearestLab(clusters, minPath, idxMap, clusterNum);
	//sortHierarchical(clusters, minPath, idxMap, clusterNum);
	//sortBySSM(clusters, minPath, idxMap, clusterNum);
	//if(clusterNum > 6)
	//	sortHierarchical_2(clusters, minPath, idxMap, clusterNum); 
}

void ColorSorter::sortHierarchical_2( const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum /*= 256*/ )
{
	int idx, count;
	for (int i = 0; i < clusterNum; ++i)
	{
		minPath[i] = i;
	}
	//////////////////////////////////////////////////////////////////////////
	// 对Cluster进行聚类
	int k = 6;
	std::vector<ColorSample> seed;
	int c[256];
	memset(c, 0, 256 * sizeof(int));

	srand(time(NULL));
	for(int i = 0; i < k; ++i)
	{
		seed.push_back(clusters[rand() % clusterNum]);
	}

	Vector3f lab_tmp;
	float dist;
	float dist_tmp;
	float changes;
	idx = 0;
	do 
	{
		for (int i = 0; i < clusterNum; ++i)
		{
			dist = 999999;
			for (int j = 0; j < seed.size(); ++j)
			{
				dist_tmp = distance(clusters[i], seed[j]);
				if (dist_tmp < dist)
				{
					c[i] = j;
					dist = dist_tmp;
				}
			}
		}
		// 重新计算聚类中心
		changes = 0.0;
		for (int j = 0; j < seed.size(); ++j)
		{
			lab_tmp.set(0, 0, 0);
			count = 0;
			for (int i = 0; i < clusterNum; ++i)
			{
				if (c[i] == j)
				{
					lab_tmp += clusters[i].lab;
					++count;
				}
			}
			if (count > 0)
			{
				changes += (seed[j].lab - lab_tmp / count).norm();
				seed[j].lab = lab_tmp / count;
				//printf(" > cluster %d changed\n", j);
			}
		}
		//printf("[%d] Changes:%f\n", ++idx, changes);
	} while (changes > 0.05);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 对聚出的几个大类进行排序(对聚类中心排序)
	int* seed_sort_map = new int[k];
	for(int i = 0; i < k; ++i)
		seed_sort_map[i] = i;

	for(int i = 0; i < k; ++i)
	{
		for(int j = i + 1; j < k; ++j)
		{
			if (seed[seed_sort_map[i]].lab.x > seed[seed_sort_map[j]].lab.x) //i的亮度大于j，交换
				std::swap(seed_sort_map[i], seed_sort_map[j]);
		}
	}

	for (int i = 0; i < clusterNum; ++i)
	{
		for(int j = 0; j < k; ++j)
		{
			if (c[i] == seed_sort_map[j])
			{
				c[i] = j;
				break;
			}
		}
	}

	delete seed_sort_map;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 根据聚类的结果对所有的颜色排序
	std::vector<int> seed_colors;
	std::vector<int> seed_color_count;
	int idx_tmp;
	idx = 0;
	for (int i = 0; i < k; ++i)
	{
		seed_colors.clear();
		for (int j = 0; j < clusterNum; ++j)
		{
			if (c[j] == i)
			{
				seed_colors.push_back(j);
			}
		}
		//printf("%d colors in cluster[%d]\n", seed_colors.size(), i);
		seed_color_count.push_back(seed_colors.size());

		// 对每个聚类内部进行颜色排序
#define SORT_METHOD 3
#if (SORT_METHOD == 1) //根据距离第一个点的距离排序
		dist = 99999;
		for (int j = 1; j < seed_colors.size() - 1; ++j)
		{
			dist = distance(clusters[seed_colors[0]], clusters[seed_colors[j]]);
			idx_tmp = j;
			for (int k = j + 1; k < seed_colors.size(); ++k)
			{
				dist_tmp = distance(clusters[seed_colors[0]], clusters[seed_colors[k]]);
				if (dist_tmp < dist)
				{
					dist = dist_tmp;
					idx_tmp = k;
				}
			}
			if (idx_tmp != j)
			{
				int tmp = seed_colors[j];
				seed_colors[j] = seed_colors[idx_tmp];
				seed_colors[idx_tmp] = tmp;
			}
		}

#elif (SORT_METHOD == 2) //根据距离第i个点的距离排序
		for (int j = 0; j < seed_colors.size() - 1; ++j)
		{
			dist = distance(clusters[seed_colors[j]], clusters[seed_colors[j + 1]]);
			idx_tmp = j + 1;
			for (int k = j + 2; k < seed_colors.size(); ++k)
			{
				dist_tmp = distance(clusters[seed_colors[j]], clusters[seed_colors[k]]);
				if (dist_tmp < dist)
				{
					dist = dist_tmp;
					idx_tmp = k;
				}
			}
			if (idx_tmp != j + 1)
			{
				int tmp = seed_colors[j+1];
				seed_colors[j+1] = seed_colors[idx_tmp];
				seed_colors[idx_tmp] = tmp;
			}
		}
#elif (SORT_METHOD == 3) 

#endif

		for (int j = 0; j < seed_colors.size(); ++j)
		{
			minPath[idx] = seed_colors[j];
			++idx;
		}
	}

	int l_idx = 0;
	int r_idx = 0;
	int l_min_idx, r_min_idx;
	std::vector<int> min_neighbour;
	min_neighbour.push_back(0);//把第一个color扔进去
	for (int i = 1; i < k; ++i)//找出每组与相邻组距离最近的点
	{
		int l_count = seed_color_count[i - 1];
		int r_count = seed_color_count[i];
		r_idx += l_count;
		for (int j = 0; j < l_count; ++j)
		{
			dist = 9999;
			for (int k = 0; k < r_count; ++k)
			{
				 dist_tmp = distance_lab(clusters[minPath[l_idx + j]], clusters[minPath[r_idx + k]]);
				 if (dist_tmp < dist)
				 {
					 dist =  dist_tmp;
					 l_min_idx = l_idx + j;
					 r_min_idx = r_idx + k;
				 }
			}
		}
		min_neighbour.push_back(l_min_idx);
		min_neighbour.push_back(r_min_idx);
		l_idx = r_idx;
	}
	min_neighbour.push_back(255);//把最后一个color扔进去
	//////
	// 根据最近点排序
	idx = 0;
	for (int i = 0; i < k; ++i) //对中间的k-2个cluster排序
	{
		l_idx = min_neighbour[i * 2];
		r_idx = min_neighbour[i * 2 + 1];
		std::swap(minPath[l_idx], minPath[idx]);
		idx = idx + seed_color_count[i] - 1;
		std::swap(minPath[r_idx], minPath[idx]);

		//对当前区段的数据排序
		for (int j = l_idx + 1; j < r_idx; ++j)
		{
			idx_tmp = j;
			dist = distance_lab(clusters[minPath[j]], clusters[minPath[j - 1]]);
			for(int k = j + 1; k < r_idx; ++k)
			{
				dist_tmp = distance_lab(clusters[minPath[k]], clusters[minPath[j - 1]]);
				if (dist_tmp < dist)
				{
					dist = dist_tmp;
					idx_tmp = k;
				}
			}
			if (i != idx_tmp)
			{
				std::swap(minPath[idx_tmp], minPath[j]);
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < clusterNum; ++i)
	{
		idxMap[minPath[i]] = i;
	}
}

template<class T>
inline bool findIn(T* first, T* last, const T& val)
{
	for (; first != last; ++first)
		if (*first == val)
			return true;
	return false;
};

void ColorSorter::sortNearestTo( int center, const ColorSample* clusters, int* idxMap, int* mask, int& sidx, int& start, int& span, float threshold, int clusterNum)
{
	assert(center < clusterNum);
	int* sorted = new int[clusterNum];
	float dist_tmp;
	float* dist = new float[clusterNum];

	threshold = 25.5; /////// 测试用 lab
	//threshold = 0.1; /////// 测试用 hue
	for (int i = 0; i < clusterNum; ++i)
	{
		sorted[i] = idxMap[i];
		dist[sorted[i]] = distance_lab(clusters[idxMap[center]], clusters[idxMap[i]]);
	}

	for (int i = 0; i < clusterNum; ++i)
	{
		for (int j = i + 1; j < clusterNum; ++j)
		{
			if(dist[sorted[i]] > dist[sorted[j]])
				std::swap(sorted[i], sorted[j]);
		}
	}

	for(int i = 0; i < clusterNum; ++i)
	{
		printf("%f ", dist[sorted[i]]);
		if (dist[sorted[i]] > threshold)
		{
			span = i;
			break;
		}
	}
	printf(" -> %d\n", span);

	//////////////////////////////////////////////////////////////////////////
	// 更新mask和idxMap
//	memcpy(idxMap, sorted,clusterNum * sizeof(int));
	bool flag;
	int idx = 0;
	for(int i = 0; i < center; ++i)
	{
		flag = findIn(&sorted[0], &sorted[0] + span, idxMap[i]);
		if (!flag)
		{
			if(idx < i) //不在span区间内且需要交换
				idxMap[idx] = idxMap[i];
			++idx;
		}
	}
	start  = idx;
	idx = clusterNum - 1;
	for(int i = clusterNum - 1; i > center; --i)
	{
		flag = findIn(&sorted[0], &sorted[0] + span, idxMap[i]);
		if (!flag)
		{
			if (idx > i) //不在span区间内且需要交换
				idxMap[idx] = idxMap[i];
			--idx;
		}
	}
	//printf("[%d,%d]_%d\n", start, idx, span);

	sidx = start;
	idx = start;
	for (int i = 0; i < span; ++i)
	{
		idxMap[idx] = sorted[i];
		//mask[idx] = sidx;
		++idx;
	}
	
	delete[] sorted;
	delete[] dist;
}
