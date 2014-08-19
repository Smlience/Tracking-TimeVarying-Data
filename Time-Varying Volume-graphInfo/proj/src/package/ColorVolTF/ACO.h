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
	std::vector<int> m_tabu;	//���ɱ�
	std::vector<int> m_allowed;	//���������Ľڵ�
	float* m_delta;	//��Ϣ�ر仯����
	float* m_distance;//�������
	float m_alpha;
	float m_beta;
	int m_pathCost;	//·������
	int m_nodeNum;	//�ڵ�����
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
	 * ��ʼ���㷨���������ϵ�
	 */
	void init();
private:
	Ant* m_ant;
	int* m_minCostPath;	// �ⲿ����
	float* m_distance;	// �ⲿ����
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