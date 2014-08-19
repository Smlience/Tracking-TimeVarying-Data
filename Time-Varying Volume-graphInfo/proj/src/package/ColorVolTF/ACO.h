#ifndef ACO_H
#define ACO_H

#include <vector>

class Ant
{
public:
	Ant();
	~Ant();
	void init(float* dist, float a, float b, int nodeNum);
	void nextNode(float* pheromone);
	float pathCost();
	void getPath(int* minPath);
	void updateDelta();
	float* getDelta();

private:
	std::vector<int> m_tabu;	//禁忌表
	std::vector<int> m_allowed;	//允许搜索的节点
	float* m_delta;	//信息素变化矩阵
	float* m_distance;//距离矩阵
	float m_alpha;
	float m_beta;
	int m_pathCost;	//路径长度
	int m_nodeNum;	//节点数量
	int m_first;
	int m_current;
};

class ACO
{
public:
	ACO(int* minPath, float* distMatrix, int nodeNum, int antNum, int iterNum, float alpha, float beta, float rho);
	~ACO();
	void solve();
	void updatePheromone();
private:
	/**
	 * 初始化算法，创建蚂蚁等
	 */
	void init();
private:
	Ant* m_ant;
	int* m_minCostPath;	// 外部传入
	float* m_distance;	// 外部传入
	float* m_pheromone;
	float m_minCost;
	int m_nodeNum;
	int m_antNum;
	int m_iterNum;
	float m_alpha;
	float m_beta;
	float m_rho;
};

#endif // ACO_H