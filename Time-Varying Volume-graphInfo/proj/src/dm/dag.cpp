#include "src/util/Vector3.h"
#include "src/util/func.color.h"
#include "src/dm/dag.h"
#include <stdio.h>
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
	m_timeNum=0;
	m_arryNumFeature=NULL;

	m_FeatureInfo=NULL;
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
	//	graph[i].first=NU LL;  


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
void DAG::setTimeNum(int timeNum)
{
	m_timeNum=timeNum;
}
void DAG::addEdge(Vector2i start,Vector2i end)
{
	struct edgenode *q;

	//fscanf(fp, "%d %d", node.x,node.y);                

	q=(struct edgenode *)malloc(sizeof(struct edgenode));
	q->no=end;  //信息读入的说   end
	q->next=graph[start.x][start.y].first;  //边  左边节点的链表  ij 代表 start

	graph[start.x][start.y].first=q;


}
void DAG::setFileDir(const char* fileDirName)
{
	m_pFileDir=fileDirName;
}

void DAG::setFeatureArray(unsigned char * a)
{
	m_arryNumFeature=a;
}



bool DAG::graphOutput()
{
	
	if (m_timeNum==0|| m_arryNumFeature==NULL)
	{
		printf("there is no graph  information  ....\n");
		return false;
	}

	char fn3[255];
	memset(fn3, 0, 255);
	sprintf(fn3, "%s", "graphInformation.txt");// fileDir    *******  文件名  呵呵呵 痴汉笑


	FILE *fp = fopen(fn3, "w");
	if(fp == NULL) {
		printf("Can't open file %s\n", fn3);
		return false;
	}

	
	fprintf(fp, "%d\n", m_timeNum);                //  *******
	for (int i=0;i<m_timeNum+1;i++)
	{
		
		fprintf(fp, "%u ", m_arryNumFeature[i]);                
	}


	struct edgenode *p;
	for (int i=0;i<m_timeNum;i++)//NODE_NUM
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)
		{
			int edgeSum=0;
			p=graph[i][j].first;
			while (p!=NULL)
			{
				p=p->next;        
				edgeSum++;
			}

			fprintf(fp, "%d\n", edgeSum);                
			p=graph[i][j].first;
			while (p!=NULL)
			{
				fprintf(fp, "%d %d\n", p->no.x,p->no.y);                
				p=p->next;        
			}
		}
		
	}
	fclose(fp);
	return true;

}
bool DAG::graphInput()
{
	

	

	char fn3[255];
	memset(fn3, 0, 255);
	sprintf(fn3, "%s%s",m_pFileDir, "char/graphInfomation.txt");
	// fileDir    *******  文件名  呵呵呵 痴汉笑
	//I:\project\featuretracking\Time-Varying Volume\proj\data\char

	FILE *fp = fopen(fn3, "r");
	if(fp == NULL) {
		printf("Can't open file %s\n", fn3);
		return false;
	}

	//===============================
	//  time num input 
	fscanf(fp, "%d\n", &m_timeNum);                //  *******

	//===============================
	//  time numArray input 
	/*if (m_arryNumFeature!=NULL)		
	{
		delete [] m_arryNumFeature;
		m_arryNumFeature=NULL;
	}
	m_arryNumFeature=new unsigned char [m_timeNum+1];*/
	
	for (int i=0;i<m_timeNum+1;i++)
	{
		unsigned char a;
		fscanf(fp, "%u ", &a);   
		//m_arryNumFeature[i]=a;
	}


	//===============================
	//graph  节点  链表初始化
	graph = new adjlist *[m_timeNum];
	for (int i = 0; i < m_timeNum; i++)
		graph[i] = new adjlist[m_arryNumFeature[i+1]];//

	for(int i=0;i<m_timeNum;i++) 
		for (int j=0;j<= m_arryNumFeature[i+1];j++)
			graph[i][j].first=NULL;


	//===============================
	//节点连接信息读入  图构建完成
	for (int i=0;i<m_timeNum;i++)//NODE_NUM
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)
		{
			int edgeSum=0;
			fscanf(fp, "%d\n", &edgeSum);
			graph[i][j].edgeSum=edgeSum;
			for (int k=0;k<edgeSum;k++)
			{
				struct edgenode *q;

				Vector2i node;
				fscanf(fp, "%d %d\n", &node.x,&node.y);                

				q=(struct edgenode *)malloc(sizeof(struct edgenode));
				q->no=node;  //信息读入的说
				q->next=graph[i][j].first;  //边  左边节点的链表

				graph[i][j].first=q;
			}
			

		}

	}
	fclose(fp);
	return true;

}

//void DAG::pathFind(int st,int en,ofstream& cout1){}


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


bool DAG::initFeatureList() //load Feature list
{

	//=============== 释放内存
	m_FeatureInfo = new FeatureInfo *[m_timeNum];
	for (int i = 0; i < m_timeNum; i++)
	{ 
		m_FeatureInfo[i] = new FeatureInfo[m_arryNumFeature[i+1]];
	}
	int featureNum=0;
	//==============
	//file 读入
	for (int i = 0; i < m_timeNum; i++)
	{
		char fn3[255];
		memset(fn3, 0, 255);
		sprintf(fn3, "%s%s%02d%s", m_pFileDir,"feaAttri/", i+1,".fea");// fileDir    *******

		FILE *fp = fopen(fn3, "r");
		if(fp == NULL) {
			printf("Can't open file %s\n", fn3);
			return false;
		}



		//////////////////////////////////////////////////////////////////////////
		fscanf(fp, "%d\n", &featureNum);                //  *******
		for (int j=0;j<featureNum;j++)
		{
			fscanf(fp, "%lf,%lf,%lf,%lf,%lf\n", 
				&(m_FeatureInfo[i][j].mass),   //i : 时间步  【0, x-1 】  j:特征数 【0,x-1】

				&(m_FeatureInfo[i][j].centroid.x), 
				&(m_FeatureInfo[i][j].centroid.y), 
				&(m_FeatureInfo[i][j].centroid.z),

				&(m_FeatureInfo[i][j].volume)
				);
		}

		fclose(fp);
	}
	return true;

}


void DAG::pathFindPre()
{

	//file output
	char fn[30];
	sprintf(fn, "%s", "patopressure.txt");

	ofstream cout1(fn);      //    Uf01_char_out.bin 
	if(!cout1) 
		printf("无法打开文件");


	set<Vector2i>::iterator itStart;
	set<Vector2i>::iterator itEnd;

	//for (from start  to enders )
	for(itStart=starters.begin();itStart!=starters.end();itStart++)
	{
		for (itEnd=enders.begin();itEnd!=enders.end();itEnd++)
		{
			if (  m_FeatureInfo[itStart->x][itStart->y].volume> 4000)//( itStart->x) >20 )
			{
				pathFind(*itStart,*itEnd,cout1);

			}
		}
	}

	//file output
	cout1.close();

}
void DAG::pathFind(Vector2i st,Vector2i en,ofstream& cout1)
{


	//cout<<"Path form:"<<st<<"to "<<en<<endl;
	//cout1<< "hello begin \n";

	int top,head=1,i;
	Vector2i v;
	struct edgenode *p;

	//visit=(int *)malloc(NODE_NUM*sizeof(int));
	vector<int> line;
	vector< vector<int> > visit;

	for(int i = 0; i < m_timeNum; i++)
	{
		for(int j = 0; j < m_arryNumFeature[i+1]; j++)
			line.push_back(0);
		visit.push_back(line);
		//line.erase(line.begin(), line.end());
	}

	vector<Vector2i>  stack(m_timeNum*4);//=(int *)malloc((NODE_NUM+1)*sizeof(int));

	
	v=st;
	visit[st.x][st.y]=1;
	top=1;
	stack[top]=v;  //problem

	do
	{
		if(head==1) 
		{
			p=graph[v.x][v.y].first;
			head=0;
		}
		else p=p->next;

		if(p)
		{
			if(visit[p->no.x][p->no.y]==0)
			{
				visit[p->no.x][p->no.y]=1;
				top++;
				stack[top]=p->no;
				if(p->no==en)
				{
					cout1<<top<<" ";
					vector<Vector2i> onepath;

					for(i=1;i<=top;i++)
					{
						onepath.push_back(stack[i]);
						pathNode.insert(stack[i]);

						cout1<<stack[i]<<" ";
						//fprintf(put, "# %d ",stack[i]);
						//intf("%d %d",stack[i].x,stack[i].y);
						cout1<<"wo are u\n"<<endl;
					}

					m_pathInfo.push_back(onepath);

					cout1<<endl;
					//					printf("\n");
					visit[en.x][en.y]=0;
					top--;
					v=stack[top];
					head=0;
				}
				else 
				{
					v=stack[top];
					head=1;
				}
			} //
		}
		else 
		{
			visit[stack[top].x][stack[top--].y]=0; //
			if(top)
			{
				p=graph[stack[top].x][stack[top].y].first;
				while(p->no!=v) p=p->next;
				v=stack[top];
				head=0;
			}
		}
		//cout1<< "while end \n";
	}while(top);



}

//path generation assolated
void DAG::findSpecialNode()
{
	struct edgenode *p;

	//pre=g[i*CLUSTER_NUM+j].first;
	set<Vector2i>  normalNode;
	//vector<list <int> >paths;

	//中间节点
	for (int i=0;i<m_timeNum;i++)
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)
		{
			p=graph[i][j].first;
			while (p!=NULL)
			{
				normalNode.insert(p->no);
				p=p->next;

			}
		}
		
	}
	//////////////////////////////////////////////////////////////////////////
	// whkat is this for?
/*
	if (graph[201].first!=NULL)
	{
		cout<<"201 ' first is not  NULL  Thx";
	}*/


	//set<int>::iterator itNormal;
	////for (from start  to enders )
	//cout<<"set normalNode :"<<endl;
	//for(itNormal=normalNode.begin();itNormal!=normalNode.end();itNormal++)
	//{
	//	cout<<"  "<<*itNormal;
	//}


	//start node  
	//|g|!=0 && !∈normalNode
	for (int i=0;i<m_timeNum;i++)
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)
		{
			set<Vector2i>::iterator it;
			Vector2i node=Vector2i(i,j);
			if(graph[i][j].first!=NULL)
			{
				it=normalNode.find(node);
				if(it==normalNode.end()) //not find
					starters.insert(node);
			}
		}
		
	}


	//end node
	//|g|==0 &&  ∈normalNode
	for (int i=0;i<m_timeNum;i++)
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)
		{
			set<Vector2i>::iterator it;
			Vector2i node=Vector2i(i,j);

			if(graph[i][j].first==NULL)
			{
				it=normalNode.find(node);
				if(it!=normalNode.end()) // find
					enders.insert(node);
			}
		}
	}

	// isolated node
	//|g|==0 &&  !∈normalNode
	for (int i=0;i<m_timeNum;i++)
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)
		{
			set<Vector2i>::iterator it;
			Vector2i node=Vector2i(i,j);

			if(graph[i][j].first==NULL)
			{
				it=normalNode.find(node);
				if(it!=normalNode.end()) //not find
					isolated.insert(node);
			}
		}
	}
}

void DAG::pathGenerate()
{
	findSpecialNode();
	specialNodeOutput();
	pathFindPre();
}

void DAG::specialNodeOutput()
{
	set<Vector2i>::iterator itStart;
	set<Vector2i>::iterator itEnd;
	//for (from start  to enders )
	cout<<"set start :"<<endl;
	for(itStart=starters.begin();itStart!=starters.end();itStart++)
	{
		cout<<"  "<<*itStart;
	}

	cout<<"set end :"<<enders.size()<<endl;
	for (itEnd=enders.begin();itEnd!=enders.end();itEnd++)
	{
		cout<<"  "<<*itEnd;
	}
}