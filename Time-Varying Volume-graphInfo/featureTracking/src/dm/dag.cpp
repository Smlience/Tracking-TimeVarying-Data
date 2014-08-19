#include "src/util/Vector3.h"
#include "src/util/func.color.h"
#include "dag.h"
using namespace std;

//====================================================================================
//计算属性
//Mass、centroid、Maximum、Volume、Moment、Domain-specific variables、Bounding surface 
//====================================================================================


//====================================================================================
//特征匹配

//-------------------
//continuation
//Using centroid to find the closest feature 
//Test whether the volume  mass and other attribute within the prescribed tolerance

//-------------------
//Bifurcation and amalgamation
//The average weighted centroid, total volume, total mass,

//-------------------
//Creation and dissipation
//====================================================================================


//====================================================================================
//DAG图生成
//Mass、centroid、Maximum、Volume、Moment、Domain-specific variables、Bounding surface 
//====================================================================================





DAG::DAG()
{
//	m_pathCount=0;
}

DAG::~DAG(){}
void DAG::dataInitialize(){}
void DAG::readHistograms(){}

void DAG::saveOneHistogram(int t){}
void DAG::generateProbs()
{
	////create graph and edge cull
	//// calculate the forward transition probabilities
	//struct edgenode *q,*p,*pre;

	////============
	////初始化 保留
	//int numNode=NODE_NUM;
	//graph=(adjlist *)malloc( numNode *sizeof(adjlist) );
	//for(int i=0;i<numNode;i++) 
	//	graph[i].first=NULL;


	//unsigned int aBuffer[HISTOGRAM_BINS_NUM];
	//unsigned int bBuffer[HISTOGRAM_BINS_NUM];


	//for (int i=0;i<M_TIME_NUM - 1;i++)  //M_TIME_NUM - 1
	//{
	//	//cout<<"M_TIME_NUM : "<<i<<endl;
	//	for (int j=0;j<CLUSTER_NUM;j++)  //放个数组 存储每个时间步里的节点数
	//	{
	//		//cout<<"		CLUSTER_NUM a : "<<j<<endl;
	//		float total=0.0;

	//		//////////////////////////////////////////////////////////////////////////
	//		for (int k=0;k< CLUSTER_NUM;k++)
	//		{

	//			//cout<<"				CLUSTER_NUM b : "<<k<<endl;
	//			// create an edge in the graph
	//			q=(struct edgenode *)malloc(sizeof(struct edgenode));

	//			// total histogram distance
	//			float dist = 0.0;
	//			for (int L=1;L< TIME_WIDE;L++)
	//			{	
	//				//unsigned int * a=&timesteps[i].clusters[j].timehistogram[L][0];
	//				//unsigned int * b=&timesteps[i+1].clusters[k].timehistogram[L-1][0];

	//				for (unsigned int M=0;M<HISTOGRAM_BINS_NUM;M++)
	//				{
	//					aBuffer[M]=timesteps[i].clusters[j].timehistogram[j*TIME_WIDE*HISTOGRAM_BINS_NUM + L*HISTOGRAM_BINS_NUM  +   M];
	//					bBuffer[M]=timesteps[i+1].clusters[k].timehistogram[k*TIME_WIDE*HISTOGRAM_BINS_NUM + (L-1)*HISTOGRAM_BINS_NUM  +   M];
	//				}
	//				dist = dist + distEMD(aBuffer, bBuffer,HISTOGRAM_BINS_NUM);

	//			}
	//			// normalized by buckets
	//			dist = dist / (header.vectorwidth * header.histogrambins);
	//			//cout<<"dist:  "<<dist<<endl;

	//			q->no=(i+1)*CLUSTER_NUM+k;//node ID  PROBLEM
	//			q->weight=dist;
	//			q->next=graph[i*CLUSTER_NUM+j].first;
	//			graph[i*CLUSTER_NUM+j].first=q;

	//			total+=(float)1.0/pow(dist,2);

	//		}
	//		//cout<<"total: b "<<total<<endl;
	//		p=graph[i*CLUSTER_NUM+j].first;
	//		while (p!=NULL)
	//		{
	//			float weightTmp=float(1.0) / pow(p->weight,2) / total;//pow(float(1.0)  / p->weight, 2) * total;


	//			p->weight=weightTmp;           
	//			p=p->next;

	//		}
	//	}
	//}

}


void DAG::cullEdge(){}
void DAG::graphOutput()
{
	

	//struct edgenode *p;
	//for (int i=0;i<NODE_NUM;i++)//NODE_NUM
	//{
	//	cout<<i<<"_____________________________________________"<<endl;
	//	p=graph[i].first;
	//	//fprintf(output, "%d\n",i);
	//	while (p!=NULL)
	//	{
	//            cout<<"no: "<<p->no<< "weight:"<<p->weight<<endl;
	//			//fprintf(output, "       no: %d weight: %f\n",p->no,p->weight);
	//			p=p->next;        
	//	
	//	}
	//}

	//if (graph[201].first!=NULL)
	//{
	//	cout<<"201 ' first is not  NULL  Thx";
	//}
	//else
	//{
	//	cout<<"201 ' first is   NULL  Thx";
	//}

}
void DAG::findSpecialNode(){}
void DAG::pathFindPre(){}
//void DAG::pathFind(int st,int en,ofstream& cout1){}
void DAG::removeDuplicates(){}

double DAG::distL2(unsigned int* a, unsigned int* b, unsigned int w) 
{
	float dist = 0;
	for(unsigned  int i = 0; i < w; i = i + 1) 
	{
		float val =abs( float (a[i] - b[i])  ) ;
		dist = dist + val * val;
		//cout<<a[i]<<" "<<b[i]<<" "<<dist<<" "<<endl;
	}
	return sqrt(dist);
}

void DAG::distEMD(unsigned int* a, unsigned int* b, unsigned int w){}

void DAG::specialNodeOutput(){}