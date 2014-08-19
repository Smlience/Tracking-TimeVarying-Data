#ifndef DAG_H
#define DAG_H

/**
* @brief feature tracking implementation
* @param [out] m_pFeatureIndex float *  �����ݵ�����ֵ
* @param [in] data T * ����������
* @param [in] dim Vector3i ���ݵ�ά��
* @date start from: 2014/7/26 
*/
#define DIS 1   
#define CONTINUEW 2
#define BIFURCATION 3   // ��֧ Bifurcation   amalgamation   Creation    dissipation
#define AMALGAMATION 4  //�ϲ�
#define CREATION 5      //����
#define DISSIPATION 6   //����

#include "src/util\func.math.h"
#include "src/util\util.h"
#include "src/util\Vector2.h"
#include <time.h>

struct edgenode
{
	Vector2i no;
	struct edgenode *next;
};

struct vex
{
	struct edgenode *first;
};

typedef struct vex adjlist; //


struct FeatureInfo
{
	double   mass; //����
	Vector3d centroid;
	double   volume; //������
	int		forFlag;  //con
	int     backFlag;
};

class DAG
{
public:
	
	FeatureInfo **m_FeatureInfo;//	[M_TIME_NUM];   FeatureInfo[m_arryNumFeature[i+1]];
	adjlist **graph;

	//typedef list<edgenode > edgeList;
	//vector<edgeList> graph;

	
	
public:
	DAG();
	~DAG();
	void dataInitialize();
	void readHistograms();

    void saveOneHistogram(int time);

	void generateProbs();
	void cullEdge();
	void findSpecialNode();
	void specialNodeOutput();
	//void pathFind(int st,int en,ofstream& cout1);
	void pathFindPre();

	void graphOutput();
	void removeDuplicates();
	double distL2(unsigned int* a, unsigned int* b, unsigned int w) ;
	void distEMD(unsigned int* a, unsigned int* b, unsigned int w) ;

	//void graphOutput();
};


#endif