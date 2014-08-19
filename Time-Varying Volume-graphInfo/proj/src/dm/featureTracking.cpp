#include "util/Vector3.h"
#include "util/func.color.h"
#include "featureTracking.h"
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
	m_pathCount=0;
}

DAG::~DAG()
{

}
void DAG::dataInitialize()
{

		timesteps=new DESC[M_TIME_NUM];
		for(int i=0;i<M_TIME_NUM;i++)
		{
			timesteps[i].clusters= new INFO [CLUSTER_NUM];
		}

		
		
		for(int i=0;i<M_TIME_NUM;i++)
		{
			for (int j=0;j<CLUSTER_NUM;j++)
				timesteps[i].clusters[j].timehistogram
					= new unsigned int [CLUSTER_NUM*TIME_WIDE*HISTOGRAM_BINS_NUM];

		}
		

}
void DAG::readHistograms()
{


	char fn[30];
	char buffer[1024];

	//header information input
	memset(fn,0,30);
	sprintf(fn, "%s%02d%s", "./pressure/histoInfo/P",3,".bin");//41个时间步
	FILE *input = fopen(fn, "rb");
	if(!input) 
		throw printf("无法打开文件");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	fscanf(input,"%s %u\n",buffer , &header.vectorwidth);
	fscanf(input,"%s %u\n",buffer , &header.histogrambins);
	fclose(input);

	// each cluster information file
	for (int i=0;i<M_TIME_NUM;i++)
	{
		memset(fn,0,30);
		sprintf(fn, "%s%02d%s", "./pressure/histoInfo/P",i+3,".bin");


		FILE *input = fopen(fn, "rb");
		if(!input) 
			throw printf("无法打开文件");

		
		//fread(&param.frequencySteps, INTSIZE, 1, input);

		fgets(buffer, 1024, input);		//min
		fgets(buffer, 1024, input);		//max  弃了
		fgets(buffer, 1024, input);		//min
		fgets(buffer, 1024, input);		//max  弃了


		fscanf(input,"%s %u\n",buffer , &timesteps[i].timestep);
		fscanf(input,"%s %u\n",buffer , &timesteps[i].clustersnum);
		fscanf(input,"%s %u\n",buffer , &timesteps[i].iterrations);
		fscanf(input,"%s %f\n",buffer , &timesteps[i].averageerrors);


		

		for (int j=0;j<CLUSTER_NUM;j++)
		{
			//第 i 个时间步（文件）
			fscanf(input,"%s %lf %lf %lf %lf %lf\n",buffer  
				,&timesteps[i].clusters[j].centroid[0]
			,&timesteps[i].clusters[j].centroid[1]
			,&timesteps[i].clusters[j].centroid[2]
			,&timesteps[i].clusters[j].centroid[3]
			,&timesteps[i].clusters[j].centroid[4]);

			fscanf(input,"%s %f\n",buffer , &timesteps[i].clusters[j].error);
			fscanf(input,"%s %u\n",buffer ,&timesteps[i].clusters[j].count);

			//cout<<timesteps[i].clusters[j].error<<endl;
			//cout<<timesteps[i].clusters[j].count<<endl;
			for (int L=0;L<TIME_WIDE;L++)
			{
			
			
				fscanf(input,"%s",buffer);
				//cout<<"buffer : ";
				for (int k=0;k<HISTOGRAM_BINS_NUM;k++)
				{

					fscanf(input,"%u",&timesteps[i].clusters[j].timehistogram[j * TIME_WIDE * HISTOGRAM_BINS_NUM + L * HISTOGRAM_BINS_NUM + k]);  //j * clusterNum * binsNum + i * binsNum + m
					//cout<<" "<<timesteps[i].clusters[j].timehistogram[j * TIME_WIDE * HISTOGRAM_BINS_NUM + L * HISTOGRAM_BINS_NUM + k];
				}
				//fscanf(input,"\n",buffer);
				cout<<endl;

			}
		}
		fclose(input);
	}

	//saveOneHistogram(0);
	//saveOneHistogram(1);
}

void DAG::saveOneHistogram(int t)
{

	char fn[30];
	sprintf(fn, "%02d%s", t+3,"pressure.bin");

	FILE* output = fopen(fn, "w");
	if(!output) {
		fprintf(stderr, "ERROR: unable to open %s for writing\n\n", fn);
		exit(1);
	}

	fprintf(output, "vectorwidth: %u\n", 5);
	fprintf(output, "histogrambins: %u\n", 256);
	/*fprintf(output, "min: %e\n", min);
	fprintf(output, "max: %e\n", max);*/

	fprintf(output, "timestep: %u\n",timesteps[t].timestep);
	fprintf(output, "clusters: %u\n",timesteps[t].clustersnum);
	fprintf(output, "iterations: %u\n", timesteps[t].iterrations);
	fprintf(output, "averageerror: %e\n", timesteps[t].averageerrors);

	for(unsigned int i = 0; i < CLUSTER_NUM; i = i + 1) {
		fprintf(output, "cluster:");
		for(unsigned int j = 0; j < TIME_WIDE; j = j + 1) {
			fprintf(output, " %lf", timesteps[t].clusters[i].centroid[j] );
		}
		fprintf(output, "\n");
		fprintf(output, "error: %e\n", timesteps[t].clusters[i].error);
		fprintf(output, "count: %u\n", timesteps[t].clusters[i].count);


		for(unsigned int j = 0; j < TIME_WIDE; j = j + 1) {
			fprintf(output, "timehistogram: ");
			for(unsigned int m = 0; m < HISTOGRAM_BINS_NUM; m = m + 1) {
				fprintf( output, "%u ", timesteps[t].clusters[i].timehistogram[ i * TIME_WIDE * HISTOGRAM_BINS_NUM + j * HISTOGRAM_BINS_NUM + m] );
			}
			fprintf(output, "\n");
		}
	}

	fclose(output);

}
void DAG::generateProbs()
{
	//create graph and edge cull
	// calculate the forward transition probabilities
	struct edgenode *q,*p,*pre;
	
	int numNode=NODE_NUM;
	graph=(adjlist *)malloc( numNode *sizeof(adjlist) );
	for(int i=0;i<numNode;i++) 
		graph[i].first=NULL;

	 unsigned int aBuffer[HISTOGRAM_BINS_NUM];
	 unsigned int bBuffer[HISTOGRAM_BINS_NUM];

	for (int i=0;i<M_TIME_NUM - 1;i++)  //M_TIME_NUM - 1
	{
		//cout<<"M_TIME_NUM : "<<i<<endl;
		for (int j=0;j<CLUSTER_NUM;j++)
		{
			//cout<<"		CLUSTER_NUM a : "<<j<<endl;
			float total=0.0;

			//////////////////////////////////////////////////////////////////////////
			for (int k=0;k< CLUSTER_NUM;k++)
			{

				//cout<<"				CLUSTER_NUM b : "<<k<<endl;
				// create an edge in the graph
				q=(struct edgenode *)malloc(sizeof(struct edgenode));

				// total histogram distance
				float dist = 0.0;
				for (int L=1;L< TIME_WIDE;L++)
				{	
					//unsigned int * a=&timesteps[i].clusters[j].timehistogram[L][0];
					//unsigned int * b=&timesteps[i+1].clusters[k].timehistogram[L-1][0];

					for (unsigned int M=0;M<HISTOGRAM_BINS_NUM;M++)
					{
						aBuffer[M]=timesteps[i].clusters[j].timehistogram[j*TIME_WIDE*HISTOGRAM_BINS_NUM + L*HISTOGRAM_BINS_NUM  +   M];
						bBuffer[M]=timesteps[i+1].clusters[k].timehistogram[k*TIME_WIDE*HISTOGRAM_BINS_NUM + (L-1)*HISTOGRAM_BINS_NUM  +   M];
					}
					dist = dist + distEMD(aBuffer, bBuffer,HISTOGRAM_BINS_NUM);

				}
				// normalized by buckets
				dist = dist / (header.vectorwidth * header.histogrambins);
				//cout<<"dist:  "<<dist<<endl;

				q->no=(i+1)*CLUSTER_NUM+k;//node ID  PROBLEM
				q->weight=dist;
				q->next=graph[i*CLUSTER_NUM+j].first;
				graph[i*CLUSTER_NUM+j].first=q;

				total+=(float)1.0/pow(dist,2);

			}
			//cout<<"total: b "<<total<<endl;
			p=graph[i*CLUSTER_NUM+j].first;
			while (p!=NULL)
			{
				float weightTmp=float(1.0) / pow(p->weight,2) / total;//pow(float(1.0)  / p->weight, 2) * total;
			
		          
				p->weight=weightTmp;           
				p=p->next;
				
			}
		}
	}

}


void DAG::cullEdge()
{
	struct edgenode *q,*p,*pre;
	
	for (int i=0;i<NODE_NUM;i++)//NODE_NUM
	{
		//cout<<"******************************"<<endl;
		pre=graph[i].first;
		p=graph[i].first;
		//删除＜ cull的节点 阿鲁
		while (p!=NULL)
		{

			if (p->weight<CULL)    
			{   
				struct edgenode *tmp;
				//cout<<"p->weight<CULL "<<endl;
				if (graph[i].first==p)
				{
					//cout<<"graph->first==p "<<endl;
					
				}

				if (pre==p)
				{
					tmp=p;
					graph[i].first=p->next;
					pre=p->next;

					p=p->next;
					free(tmp);

				}
				else
				{
					tmp=p;
					pre->next=p->next;
					p=p->next;
					free(tmp);
				}
				

			}        
			else        
			{   
				//cout<<"p->weight>CULL "<<endl;
				pre=p;            
				p=p->next;        
			}
		}
	}
	
}
void DAG::graphOutput()
{
	//file output
	//char fn[30];
	//sprintf(fn, "%s", "graphInfo.bin");

	//FILE* output = fopen(fn, "w");
	//if(!output) {
	//	fprintf(stderr, "ERROR: unable to open %s for writing\n\n", fn);
	//	exit(1);
	//}



	struct edgenode *p;
	for (int i=0;i<NODE_NUM;i++)//NODE_NUM
	{
		cout<<i<<"_____________________________________________"<<endl;
		p=graph[i].first;
		//fprintf(output, "%d\n",i);
		while (p!=NULL)
		{
	            cout<<"no: "<<p->no<< "weight:"<<p->weight<<endl;
				//fprintf(output, "       no: %d weight: %f\n",p->no,p->weight);
				p=p->next;        
		
		}
	}

	if (graph[201].first!=NULL)
	{
		cout<<"201 ' first is not  NULL  Thx";
	}
	else
	{
		cout<<"201 ' first is   NULL  Thx";
	}


	////file output
	//fclose(output);
}
void DAG::findSpecialNode()
{
	struct edgenode *p;

	//pre=g[i*CLUSTER_NUM+j].first;
	set<int>  normalNode;
	//vector<list <int> >paths;

	//中间节点
	for (int i=0;i<NODE_NUM;i++)
	{
		p=graph[i].first;
		while (p!=NULL)
		{
			normalNode.insert(p->no);
			p=p->next;
			
		}
	}
	if (graph[201].first!=NULL)
	{
		cout<<"201 ' first is not  NULL  Thx";
	}
	

	//set<int>::iterator itNormal;
	////for (from start  to enders )
	//cout<<"set normalNode :"<<endl;
	//for(itNormal=normalNode.begin();itNormal!=normalNode.end();itNormal++)
	//{
	//	cout<<"  "<<*itNormal;
	//}

	//start node  
	//|g|!=0 && !∈normalNode
	for (int i=0;i<NODE_NUM;i++)
	{
		set<int>::iterator it;

		if(graph[i].first!=NULL)
			{
				it=normalNode.find(i);
				if(it==normalNode.end()) //not find
					starters.insert(i);
			}
	}


	//end node
	//|g|==0 &&  ∈normalNode
	for (int i=0;i<NODE_NUM;i++)
	{
		set<int>::iterator it;

		if(graph[i].first==NULL)
		{
			it=normalNode.find(i);
			if(it!=normalNode.end()) // find
				enders.insert(i);
		}
	}

	// isolated node
	//|g|==0 &&  !∈normalNode
	for (int i=0;i<NODE_NUM;i++)
	{
		set<int>::iterator it;

		if(graph[i].first==NULL)
		{
			it=normalNode.find(i);
			if(it!=normalNode.end()) //not find
				isolated.insert(i);
		}
	}
	
}
void DAG::pathFindPre()
{

	//file output
	char fn[30];
	sprintf(fn, "%s", "patopressure.txt");

	ofstream cout1(fn);      //    Uf01_char_out.bin 
	if(!cout1) 
		printf("无法打开文件");


	set<int>::iterator itStart;
	set<int>::iterator itEnd;

	//for (from start  to enders )
	for(itStart=starters.begin();itStart!=starters.end();itStart++)
	{
		for (itEnd=enders.begin();itEnd!=enders.end();itEnd++)
		{
			pathFind(*itStart,*itEnd,cout1);
		}
	}
	
	//file output
	cout1.close();

}
void DAG::pathFind(int st,int en,ofstream& cout1)
{
	

	//cout<<"Path form:"<<st<<"to "<<en<<endl;
	//cout1<< "hello begin \n";

	int *visit,*stack,top,v,head=1,i;
	struct edgenode *p;
	visit=(int *)malloc(NODE_NUM*sizeof(int));
	stack=(int *)malloc((NODE_NUM+1)*sizeof(int));
	for(i=0;i<NODE_NUM;i++) 
		visit[i]=0;

	v=st;visit[st]=1;top=1;stack[top]=v;

	do
	{
		if(head==1) 
		{
			p=graph[v].first;
			head=0;
		}
		else p=p->next;

		if(p)
		{
			if(visit[p->no]==0)
			{
				visit[p->no]=1;
				top++;
				stack[top]=p->no;
				if(p->no==en)
				{
					cout1<<top<<" ";
					for(i=1;i<=top;i++)
					{
						cout1<<stack[i]<<" ";
						//fprintf(put, "# %d ",stack[i]);
						//printf("%d ",stack[i]);
						//cout1<<"wo are u\n"<<endl;
					}
					cout1<<endl;
//					printf("\n");
					visit[en]=0;
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
			visit[stack[top--]]=0; //
			if(top)
			{
				p=graph[stack[top]].first;
				while(p->no!=v) p=p->next;
				v=stack[top];
				head=0;
			}
		}
		//cout1<< "while end \n";
	}while(top);

	
	
}
void DAG::removeDuplicates()
{
	//for ()
	//{
	//	for ()
	//	{

	//	}
	//}
	
}
float DAG::distL2(unsigned int* a, unsigned int* b, unsigned int w) 
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

float DAG::distEMD(unsigned int* a, unsigned int* b, unsigned int w) 
{
	int ta=0;
	int tb=0;
	int tmax=0;
	for(unsigned  int i = 0; i < w; i = i + 1) 
	{
		ta+=a[i];
		tb+=b[i];
	}

	tmax=max(ta,tb);

	for(unsigned  int i = 0; i < w; i = i + 1) 
	{
		a[i]=a[i]*tmax/ ta;   
		b[i]=b[i]*tmax/ tb;   
	}

	int moved=0;
	int j=0;
	int c=0;
	int d=0;

	for (int i=0;i<w-1;i++)
	{
		if (a[i]>b[i])
		{
			j=i+1;
			while(  (a[i]>b[i])  &&  (j<w)  )
			{
				if (a[j]<b[j])
				{
					c=a[i]-b[i];
					d=b[j]-a[j];
					if (d>c)
					{
						moved=moved+c*(j-i);
						a[j]=a[j]+c;
						a[i]=a[i]-c;
					} 
					else
					{
						moved=moved+d*(j-i);
						a[j]=a[j]+d;
						a[i]=a[i]-d;
					}
					
				}
				j=j+1;
			}
		}
		if(a[i]<b[i])  
		{
			j=i+1;
			while(  (a[i]<b[i])  &&  (j<w)  )
			{
				if (a[j]>b[j])
				{
					c=b[i]-a[i];
					d=a[j]-b[j];
						if (d>c)
						{
							moved=moved+c*(j-i);
							a[j]=a[j]-c;
							a[i]=a[i]+c;
						} 
						else
						{
							moved=moved+d*(j-i);
							a[j]=a[j]-d;
							a[i]=a[i]+d;
						}

				}
				j=j+1;
			}

		}
		
	}
	
	if (moved<1)
		return 0.5;
	return moved;

	
}

void DAG::specialNodeOutput()
{
	set<int>::iterator itStart;
	set<int>::iterator itEnd;
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