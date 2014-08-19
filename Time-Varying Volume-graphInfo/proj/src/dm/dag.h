#ifndef DAG_H
#define DAG_H

/**
* @brief feature tracking implementation
* @param [out] m_pFeatureIndex float *  体数据的特征值
* @param [in] data T * 标量体数据
* @param [in] dim Vector3i 数据的维度
* @date start from: 2014/7/26 
*/
#define DIS 1   
#define CONTINUEW 2
#define BIFURCATION 3   // 分支 Bifurcation   amalgamation   Creation    dissipation
#define AMALGAMATION 4  //合并
#define CREATION 5      //创建
#define DISSIPATION 6   //消亡

#include "src/util\func.math.h"
#include "src/util\util.h"
#include "src/util\Vector2.h"
#include <time.h>
#include <vector>
#include <set>
#include <list>
#include <fstream>
#include <iostream>
using namespace std;
struct edgenode
{
	Vector2i no;
	struct edgenode *next;
};

struct vex
{
	int edgeSum;
	struct edgenode *first;
};

typedef struct vex adjlist; //


struct FeatureInfo
{
	double   mass; //重量
	Vector3d centroid;
	double   volume; //例子数
	int		forFlag;  //con
	int     backFlag;
};

class DAG
{
public:
	
	FeatureInfo **m_FeatureInfo;//[M_TIME_NUM];
	adjlist **graph;

	std::set <Vector2i> starters, enders, isolated;
	//std::vector<std::list <Vector2i> >paths;
	int m_pathCount;

	vector<vector<Vector2i> > m_pathInfo;
	set<Vector2i> pathNode;

	//typedef list<edgenode > edgeList;
	//vector<edgeList> graph;

	int m_timeNum;
	unsigned char *m_arryNumFeature;
	
	const char* m_pFileDir;
	
public:
	DAG();
	~DAG();
	void setTimeNum(int timeNum);
	void setFeatureArray(unsigned char * a);
	void setFileDir(const char* fileDirName);
	void dataInitialize();
	void readHistograms();

    void saveOneHistogram(int time);

	void generateProbs();
	void findSpecialNode();
	void specialNodeOutput();
	void addEdge(Vector2i start,Vector2i end);

	void pathFind(Vector2i st,Vector2i en,ofstream& cout1);
	void pathFindPre();

	bool graphOutput();
	bool graphInput();
	
	double distL2(unsigned int* a, unsigned int* b, unsigned int w) ;
	void distEMD(unsigned int* a, unsigned int* b, unsigned int w) ;

	bool initFeatureList();

	
	void pathGenerate();
	//void graphOutput();
};


#endif