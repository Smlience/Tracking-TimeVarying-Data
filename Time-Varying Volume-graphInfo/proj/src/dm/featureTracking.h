#ifndef FEATURETRACKING_H
#define FEATURETRACKING_H

/**
* @brief feature tracking implementation
* @param [out] pos_value float *  体数据的特征值
* @param [in] data T * 标量体数据
* @param [in] dim Vector3i 数据的维度
* @date start from: 2014/7/26 
*/
#define DIS 1

#include "dm\Curvature.h"
#include "util\func.math.h"
#include "util\util.h"
#include <time.h>

struct edgenode
{
	int no;
	struct edgenode *next;
	float weight;
};
struct vex
{
	struct edgenode *first;
};

typedef struct vex adjlist; //


struct HEADER
{
	unsigned int vectorwidth;
	unsigned int histogrambins;
};

struct INFO
{
	double		centroid[CLUSTER_NUM];
	float		error;
	unsigned int count;
	unsigned int* timehistogram;//[CLUSTER_NUM][TIME_WIDE][HISTOGRAM_BINS_NUM];
};

struct DESC
{

	unsigned int timestep;
	unsigned int clustersnum;
	unsigned int iterrations;
	float	averageerrors;

	struct INFO * clusters;//[CLUSTER_NUM];
};

class DAG
{
private:
	HEADER header;
	DESC *timesteps;//[M_TIME_NUM];
	adjlist *graph;

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
	void pathFind(int st,int en,ofstream& cout1);
	void pathFindPre();

	void graphOutput();
	void removeDuplicates();
	float distL2(unsigned int* a, unsigned int* b, unsigned int w) ;
	float distEMD(unsigned int* a, unsigned int* b, unsigned int w) ;

	//void graphOutput();
};



#endif