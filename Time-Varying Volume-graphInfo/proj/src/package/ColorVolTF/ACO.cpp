#include "ACO.h"
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>

Ant::Ant()
	: m_nodeNum(0), m_delta(0), m_distance(0), m_alpha(0), m_beta(0)
{
	m_pathCost = 0;
}

Ant::~Ant()
{
	delete[] m_delta;
}

void Ant::init(float* dist, float a, float b, int nodeNum)
{
	srand((int)time(0));
	m_allowed.clear();
	m_tabu.clear();
	m_nodeNum = nodeNum;
	m_distance = dist;
	m_delta = new float[m_nodeNum * m_nodeNum];
	m_alpha = a;
	m_beta = b;

	memset(m_delta, 0, sizeof(float) * m_nodeNum * m_nodeNum);
	for (int i = 0; i < m_nodeNum; ++i)
	{
		m_allowed.push_back(i);
	}

	m_first = rand() % m_nodeNum;
	for (std::vector<int>::iterator iter = m_allowed.begin(); iter != m_allowed.end(); ++iter)
	{
		if ((*iter) == m_first)
		{
			m_allowed.erase(iter);
			break;
		}
	}

	m_tabu.push_back(m_first);
	m_current = m_first;
}

void Ant::nextNode(float* pheromone)
{
	float* p = new float[m_nodeNum]; //概率矩阵，即从当前node走到每一个node的概率
	float sum = 0.0f;
	int idx;
	//计算分母部分
	for (std::vector<int>::iterator iter = m_allowed.begin(); iter != m_allowed.end(); ++iter)
	{
		idx = m_current * m_nodeNum + (*iter);
		sum += pow(pheromone[idx], m_alpha) * pow(1.0f / m_distance[idx], m_beta);
	}
	//计算概率矩阵
	memset(p, 0, sizeof(float) * m_nodeNum);
	for (std::vector<int>::iterator iter = m_allowed.begin(); iter != m_allowed.end(); ++iter)
	{
		idx = m_current * m_nodeNum + (*iter);
		p[*iter] = (float)(pow(pheromone[idx], m_alpha) * pow(1.0f / m_distance[idx], m_beta)) / sum;
	}
// 	for(int i = 0; i < m_nodeNum; i++)
// 	{
// 		bool flag = false;
// 		for (std::vector<int> iter = m_allowed.begin(); iter != m_allowed.end(); ++iter)
// 		{
// 			if (i == (*iter))
// 			{
// 				idx = m_current * m_nodeNum + i;
// 				p[i] = (float)(pow(pheromone[idx], m_alpha) * pow(1.0 / m_distance[idx], m_beta)) / sum;
// 				flag = true;
// 				break;
// 			}
// 		}
// 		if (flag == false)
// 		{
// 			p[i] = 0.f;
// 		}
// 	}

	//轮盘赌选择下一个城市
	srand((int)time(0));
	float sleectP = rand() / float(RAND_MAX);
	int selectNode = 0;
	float sum1 = 0.f;
	for (int i = 0; i < m_nodeNum; i++)
	{
		sum1 += p[i];
		if (sum1 >= sleectP)
		{
			selectNode = i;
			break;
		}
	}

	//从允许选择的城市中去除select city
	for (std::vector<int>::iterator iter = m_allowed.begin(); iter != m_allowed.end(); ++iter)
	{
		if (selectNode == (*iter))
		{
			m_allowed.erase(iter);
			break;
		}
	}

	//在禁忌表中添加select city
	m_tabu.push_back(selectNode);

	//将当前城市改为选择的城市
	m_current = selectNode;
}

float Ant::pathCost()
{
	return m_pathCost;
}

void Ant::getPath( int* minPath )
{
	for (int i = 0; i < m_nodeNum + 1; i++)
	{
		minPath[i] = m_tabu[i];
	}
}

/**
 * 计算路径长度，并计算Delta矩阵(蚂蚁在路径上留下的信息素)
 */
void Ant::updateDelta()
{
	float tmp;
	// 计算路径长度
	m_pathCost = 0;
	for (std::vector<int>::iterator iter = m_tabu.begin(); iter != m_tabu.end(); ++iter)
	{
		m_pathCost += (*iter);
	}
	//计算Delta矩阵
	for (int j = 0; j < m_nodeNum; j++)
	{
		tmp = 1.0 / m_pathCost;
		m_delta[m_tabu[j] * m_nodeNum + m_tabu[j+1]] = tmp;
		m_delta[m_tabu[j + 1] * m_nodeNum + m_tabu[j]] = tmp;
	}
}

float* Ant::getDelta()
{
	return m_delta;
}

//////////////////////////////////////////////////////////////////////////

ACO::ACO(int* minPath, float* distMatrix, int nodeNum, int antNum, int iterNum, float alpha, float beta, float rho)
 : m_minCostPath(minPath), m_distance(distMatrix), m_nodeNum(nodeNum), m_iterNum(iterNum), m_alpha(alpha), m_beta(beta), m_rho(rho)
 , m_pheromone(NULL)
{}

ACO::~ACO()
{
	delete[] m_pheromone;
	delete[] m_ant;
}

void ACO::init()
{
	//初始化信息素矩阵
	int len = m_nodeNum * m_nodeNum;
	m_pheromone = new float[len];
	for(int i = 0; i < len; ++i)
	{
		m_pheromone[i] = 0.1f;  //初始化为0.1
	}
	
	m_minCost = FLT_MAX;
	//随机放置蚂蚁
	m_ant = new Ant[m_antNum];
	for(int i = 0; i < m_antNum; ++i)
	{
		m_ant[i].init(m_distance, m_alpha, m_beta, m_nodeNum);
	}
}

void ACO::solve()
{
	m_minCost = FLT_MAX;
	for (int g = 0; g < m_iterNum; ++g)
	{
		for (int i = 0; i < m_antNum; ++i)
		{
			for (int j = 1; j < m_nodeNum; ++j)
			{
				m_ant[i].nextNode(m_pheromone);
			}
			m_ant[i].updateDelta();

			// 取到更短的路径，则更新路径
			if (m_ant[i].pathCost() < m_minCost)
			{
				m_minCost = m_ant[i].pathCost();
				m_ant[i].getPath(m_minCostPath);
// 				for (int k = 0; k < m_nodeNum + 1; k++)
// 				{
// 					m_minCostPath[k] = m_ant[i].getTabu().get(k);
// 				}
			}
// 			for (int j = 0; j < m_nodeNum; j++)
// 			{
// 				m_ant[i].getDelta()[m_ant[i].getTabu().get(j)][m_ant[i].getTabu().get(j+1)] = (float) (1./m_ant[i].pathCost());
// 				m_ant[i].getDelta()[m_ant[i].getTabu().get(j+1)][m_ant[i].getTabu().get(j)] = (float) (1./m_ant[i].pathCost());
// 			}
		}

		//更新信息素
		updatePheromone();

		//重新初始化蚂蚁
		for(int i = 0; i < m_antNum; ++i)
		{
		   m_ant[i].init(m_distance, m_alpha, m_beta, m_nodeNum);
		}
	}
}

void ACO::updatePheromone()
{
	float* delta;
	int len = m_nodeNum * m_nodeNum;
	//信息素挥发  
	for(int i = 0; i < len; i++)
			m_pheromone[i] = m_pheromone[i] * (1 - m_rho);

	//信息素更新
	for(int k = 0; k < m_antNum; k++)
	{
		delta = m_ant[k].getDelta();
		for(int i = 0; i < len; i++)
		{
			m_pheromone[i] += delta[i];
		}
	}
// 	for(int i = 0; i < m_nodeNum; i++) {
// 		for(int j = 0; j < m_nodeNum; j++) {
// 			for(int k = 0; k < m_antNum; k++) {
// 				m_pheromone[i][j] += m_ant[k].getDelta()[i][j];
// 			}
// 		}
// 	}
}
