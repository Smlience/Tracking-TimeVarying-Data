#include "Volume.h"
#include <string.h>
#include <vector>
#include <fstream>
#include <stdio.h>

Volume::Volume()
{
	m_alignedDim.set(0, 0, 0);
	m_normalizedDim.set(0.0, 0.0, 0.0);
	m_pFileName = NULL;
	m_timeStep = 0;
	maxnum_of_features = 0;

	meta.data = NULL;
	meta.dim.set(0, 0, 0);
	meta.spacing.set(1.0, 1.0, 1.0);
	
	
	cal_flag = true;
	load_flag = false;

	m_pVolume = NULL;
	m_pFeatureIndex = NULL;
	m_pGradient_tmp = NULL;
	m_pGradient = NULL;

	//图相关
	m_FeatureSum=0;
}

Volume::~Volume()
{
	clear();
}

void Volume::clear()
{
	for (int i = 0 ; i < m_timeStep; ++i)
	{
		delete [] m_pVolume[i]; 
	}
	delete [] m_pVolume;
	m_pVolume = NULL;

	free(meta.data);
	meta.data = NULL;

	delete [] m_pFeatureIndex;
	m_pFeatureIndex = NULL;

	delete [] m_arryNumFeature;
	m_arryNumFeature = NULL;

	meta.dim.set(0, 0, 0);
	meta.spacing.set(1.0, 1.0, 1.0);
	

	m_alignedDim.set(0, 0, 0);
	m_normalizedDim.set(0.0, 0.0, 0.0);
	m_pFileName = NULL;
}

void Volume::clearVolume()
{
	for (int i = 0 ; i < m_timeStep; ++i)
	{
		delete [] m_pVolume[i]; 
	}
	delete [] m_pVolume;
	m_pVolume = NULL;
}
bool Volume::loadVolume(const char* filename)
{
	if (filename == NULL)
		return false;

	FILE *fp = fopen(filename, "r");
	if(fp == NULL) {
		printf("Can't open file %s\n", filename);
		return false;
	}
	printf("---------------------------------\n");
	printf("Load:%s\n", filename);

	if(m_pVolume != NULL)
		clear();
	m_pFileName = filename;

	//////////////////////////////////////////////////////////////////////////
	// read volume information
	char dataFile[1024];
	char dataDir[1024];

	memset(dataFile, 0, 1024);
	memset(dataDir, 0, 1024);

	char dataType;
	int dataWide;
	char volumeDataFileName[200];

	fscanf(fp, "%d %d %d\n", &(meta.dim.x), &(meta.dim.y), &(meta.dim.z));
	fscanf(fp, "%f %f %f\n", &(meta.spacing.x), &(meta.spacing.y), &(meta.spacing.z));
	//
	fscanf(fp, "%d\n", &m_timeStep);

	const char* pSlash = strrchr(filename, '/');
	memset(dataDir, 0, 1024);
	strncpy(dataDir, filename, pSlash - filename + 1);
	//printf("data file dir :[%s]\n", dataFile);
	strcat(dataFile, volumeDataFileName);
	printf("data file  dir   :[%s]\n", dataDir);

	m_pFileDir=dataDir;


	//file name	
	std::vector<std::string> fName(m_timeStep);
	char str[255];
	for (int i=0;i<m_timeStep;i++)
	{
		memset(str, 0, 255);

		sprintf(str, "%s%02d%s", "char/",i+1,".bin");
		//fscanf(fp, "%s\n", str);
		fName[i]=(std::string)str;
		//cout<<fName[i]<<endl;
		printf(" fen  file name     :[%s]\n", str);
	}

	fclose(fp);
	//printf("data file Name:[%s]\n", volumeDataFileName);

	m_alignedDim.x = getPow2(meta.dim.x);
	m_alignedDim.y = getPow2(meta.dim.y);
	m_alignedDim.z = getPow2(meta.dim.z);

	// the range in [0..1] in each dimension
	m_normalizedDim.set(meta.dim.x * meta.spacing.x, meta.dim.y * meta.spacing.y, meta.dim.z * meta.spacing.z);
	double maxSize = Max(m_normalizedDim.x, Max(m_normalizedDim.y, m_normalizedDim.z));
	m_normalizedDim /= maxSize;
// 

	

	// 根据数据尺寸、每个体素元素个数和数据类型计算总共体数据大小
	m_volumeSize = meta.dim.x * meta.dim.y * meta.dim.z;
	m_pVolume = new unsigned char *[m_timeStep];
	for (int k = 0; k < m_timeStep; k++)
	{ 
		m_pVolume[k] = new unsigned char[m_volumeSize];
	}

	m_pFeatureIndex=new unsigned char[m_volumeSize];
	m_pGradient_tmp=new float [3*m_volumeSize];  //3*volumeSize*sizeof(float)
	m_pGradient=new unsigned char [3*m_volumeSize]; 
	// read volume data from file list
	for (int i=0;i<m_timeStep;i++) //m_timeNum
	{
		strcpy(dataFile,dataDir);
		strcat(dataFile, fName[i].c_str());
		//cout<<dataFile<<endl;
		fp = fopen(dataFile, "rb");
		if(fp == NULL) {
			std::cout<<"Can't open data file "<<dataFile<<std::endl;
			return false;
		}
		fread(m_pVolume[i], sizeof(unsigned char), m_volumeSize, fp);
		fclose(fp);

		printf("open data file :[%s]\n",dataFile);
		memset(dataFile, 0, 1024);

		
		
		
	}

	
	printf("Load volume is OK !\n");


	
	fclose(fp);

	loadVolume_process();
	return true;
}

bool Volume::loadVolume_process()
{
	unsigned char num_of_feature;

		
	m_arryNumFeature = new unsigned char[m_timeStep+1];

	for(int i = 1;i <= m_timeStep;i++){
		
		// 根据数据尺寸、每个体素元素个数和数据类型计算总共体数据大小
		if (m_pVolume != NULL)
		{
			
			memset(m_pFeatureIndex,0,m_volumeSize*sizeof(unsigned char));
			memset(m_pGradient_tmp,0,3*m_volumeSize*sizeof(float));
			memset(m_pGradient,0,3*m_volumeSize*sizeof(unsigned char));

			calcFeature(m_pFeatureIndex, m_pVolume[i-1], getDimension()  ,num_of_feature);
			//输出：Mass、centroid、Maximum、Volume 特征对应的point number
			calFeatureAttribute(i-1,num_of_feature);

			if(num_of_feature > maxnum_of_features)
				maxnum_of_features = num_of_feature;
		
			m_arryNumFeature[i]=num_of_feature;
			m_FeatureSum+=num_of_feature;

			//printf("pos value calculated!\n");

			outputFeature(i);

			//计算梯度
			calcGradientCentralDiff(m_pGradient_tmp, m_pVolume[i-1], meta.dim);
			gradientNormalize(m_pGradient_tmp, meta.dim);
			gradientF2UC(m_pGradient_tmp, m_pGradient, meta.dim);

			outputGradient(i);
		}
		else
		{
			printf("m_pVolume == NULL!\tBad allocate memory!\n");
		}
		

		//printf("data\t%d\tprocessed....\n\n\n",i);
	}
	
	m_arryNumFeature[0]=maxnum_of_features;

	outputNumOfFeature(m_arryNumFeature);

	initFeatureList();
	generateProbs();
	graphOutput();
	
	return true;
}



double Volume::getValue(unsigned char* pData, double x, double y, double z)
{
	x = Max(Min(x + 0.5, meta.dim.x - 1.0), 0.0);
	y = Max(Min(y + 0.5, meta.dim.y - 1.0), 0.0);
	z = Max(Min(z + 0.5, meta.dim.z - 1.0), 0.0);
	int index = (int)z * meta.dim.x * meta.dim.y + (int)y * meta.dim.z + (int)x;
	return pData[index];
}

//////////////////////////////////////////////////////////////////////////
/*
*	get the power of 2 greater than or equal to size
*/
int Volume::getPow2(int size)
{
	double tmp = size / 2.0;
	int i = 2;
	while(tmp > 1.0){
		i *= 2;
		tmp /= 2.0;
	}
	return i;
}

bool Volume::outputFeature(int timestep)
{
	

	int len = meta.dim.x * meta.dim.y * meta.dim.z;

	//======
	char fn3[255];
	memset(fn3, 0, 255);
	//sprintf(str, "%s%s%02d%s", m_pFileDir,"char/",i+1,".bin");
	sprintf(fn3, "%s%s%02d%s", m_pFileDir,"pos/",timestep,".pos");// fileDir    /

	std::ofstream cout1(fn3,std::ios::binary);      //    Uf01_char_out.bin 
	if(!cout1) 
		printf("无法打开文件");

	cout1.write((char *)(m_pFeatureIndex),len*sizeof(unsigned char));

	cout1.close();

	
	return true;
}
bool Volume::outputGradient(int timestep)
{


	int len =3* meta.dim.x * meta.dim.y * meta.dim.z;

	//======
	char fn3[255];
	memset(fn3, 0, 255);
	//sprintf(str, "%s%s%02d%s", m_pFileDir,"char/",i+1,".bin");
	sprintf(fn3, "%s%s%02d%s", m_pFileDir,"gra/",timestep,".gra");// fileDir    /

	std::ofstream cout1(fn3,std::ios::binary);      //    Uf01_char_out.bin 
	if(!cout1) 
		printf("无法打开文件");

	cout1.write((char *)(m_pGradient),len*sizeof(unsigned char));

	cout1.close();


	return true;
}

bool Volume::outputNumOfFeature(unsigned char* pFeatureNumArray)
{
	
	char fn3[255];
	sprintf(fn3, "%s%s", m_pFileDir,"num.data");// fileDir    /

	unsigned char tmp; 
	std::ofstream cout1(fn3,std::ios::binary);      //    Uf01_char_out.bin 
	if(!cout1) 
		printf("无法打开文件");
	//tmp=pFeatureNumArray[0];
	//pFeatureNumArray[0]=45;
	//cout1.write((char *)(pFeatureNumArray),(m_timeStep+1)*sizeof(unsigned char));
	cout1.write((char *)(pFeatureNumArray),(m_timeStep+1)*sizeof(unsigned char));

	cout1.close();

	//pFeatureNumArray[0]=tmp;

	for (int i=0;i<m_timeStep+1;i++)
	{
		printf("%d  %d....\n",i,pFeatureNumArray[i]);
	}

	return true;
}
/*
*	 Trilinear interpolation, assume 0<=xIndex<xiSize 0<=yIndex<yiSize 0<=zIndex<ziSize 
*/

/*暂时没用到--YRF*/
double Volume::intpTrilinear(unsigned char* pData, double x, double y, double z)
{
	int xIndex = (int)x, yIndex = (int)y, zIndex = (int)z;
	double xFraction = x - xIndex;
	double yFraction = y - yIndex;
	double zFraction = z - zIndex;
	if(xIndex < 0 || xIndex > meta.dim.x - 1 || yIndex < 0 || yIndex > meta.dim.y || zIndex < 0 || zIndex > meta.dim.z - 1) {
		printf("interpolation size out of range\n");
		return 0;
	}
	int index = zIndex * meta.dim.x * meta.dim.y + yIndex * meta.dim.x + xIndex;
	int xNext = (xIndex < meta.dim.x - 1) ? 1 : 0;
	int yNext = (yIndex < meta.dim.y - 1) ? meta.dim.x : 0;
	int zNext = (zIndex < meta.dim.z - 1) ? meta.dim.x * meta.dim.y : 0;
	int f000 = pData[index];
	int f100 = pData[index + xNext];
	int f010 = pData[index + yNext];
	int f001 = pData[index + zNext];
	int f101 = pData[index + xNext + zNext];
	int f011 = pData[index + yNext + zNext];
	int f110 = pData[index + xNext + yNext];
	int f111 = pData[index + xNext + yNext + zNext];
	double fResult = f000 * (1 - xFraction) * (1 - yFraction) * (1 - zFraction) +
		f100 * xFraction * (1 - yFraction) * (1 - zFraction) +
		f010 * (1 - xFraction) * yFraction * (1 - zFraction) +
		f001 * (1 - xFraction) * (1 - yFraction) * zFraction +
		f101 * xFraction * (1 - yFraction) * zFraction +
		f011 * (1 - xFraction) * yFraction * zFraction +
		f110 * xFraction * yFraction * (1 - zFraction) +
		f111 * xFraction * yFraction * zFraction;
	return fResult;
}

/*
*  trilinear cubic BSpline scalar interpolation
*/
double Volume::cubicIntpValue(double v0, double v1, double v2, double v3, double mu)
{
	const double mu2 = mu * mu;
	const double mu3 = mu2 * mu;

	const double a0 = -1.0 * v0 + 3.0 * v1 - 3.0 * v2 + 1.0 * v3;
	const double a1 =  3.0 * v0 - 6.0 * v1 + 3.0 * v2 + 0.0 * v3;
	const double a2 = -3.0 * v0 + 0.0 * v1 + 3.0 * v2 + 0.0 * v3;
	const double a3 =  1.0 * v0 + 4.0 * v1 + 1.0 * v2 + 0.0 * v3;

	return (a0 * mu3 + a1 * mu2 + a2 * mu + a3) / 6.0;
}

double Volume::triCubicIntpValue(unsigned char* pData, double x, double y, double z)
{
	// Extract the integer and decimal components of the x, y co-ordinates
	double ulo, vlo, wlo;
	const double ut = modf( x, &ulo );
	const double vt = modf( y, &vlo );
	const double wt = modf( z, &wlo );

	int		xlo = int( ulo );
	int		ylo = int( vlo );
	int		zlo = int( wlo );

	// We need all the voxels around the primary
	double voxels[4][4][4];
	{
		for( int z=0; z<4; z++ ) {
			for( int y=0; y<4; y++ ) {
				for( int x=0; x<4; x++ ) {
					int px = (xlo-1+x);
					int py = (ylo-1+y);
					int pz = (zlo-1+z);
					voxels[z][y][x] =  getValue(pData, px, py, pz);
				}
			}
		}
	}

	// Now that we have all our voxels, run the cubic interpolator in one dimension to collapse it (we choose to collapase x)
	double voxelcol[4][4];
	{
		for( int z=0; z<4; z++ ) {
			for( int y=0; y<4; y++ ) {
				voxelcol[z][y] = cubicIntpValue(voxels[z][y][0], voxels[z][y][1], voxels[z][y][2], voxels[z][y][3], ut);
			}
		}
	}

	// Then collapse the y dimension 
	double voxelcol2[4];
	{
		for( int z=0; z<4; z++ ) {
			voxelcol2[z] = cubicIntpValue(voxelcol[z][0], voxelcol[z][1], voxelcol[z][2], voxelcol[z][3], vt);
		}
	}

	// The collapse the z dimension to get our value
	return cubicIntpValue(voxelcol2[0], voxelcol2[1], voxelcol2[2], voxelcol2[3], wt);
}

bool Volume::loaded()
{
	//return m_pData != NULL;
	return meta.data != NULL;
}

//====================================================================================
//计算属性
//Mass、centroid、Maximum、Volume、Moment、Domain-specific variables、Bounding surface 
//====================================================================================
bool Volume::calFeatureAttribute(int timeStep,int featureNum) 
	// 37   calFeatureAttribute(i-1,num_of_feature);
{
	//mass   所有体素 标量值的和  W
	char fn3[255];
	memset(fn3, 0, 255);
	//sprintf(fn3, "%s%s%02d%s", m_pFileDir,"pos/",timestep,".pos");// fileDir    /
	sprintf(fn3, "%s%s%02d%s", m_pFileDir, "feaAttri/",timeStep+1,".fea");// fileDir    /
	 FILE *fp=fopen(fn3,"w");;

	double f_mass[40];
	Vector3d f_centroid[40];  //Vector3f m_normalizedDim;   typedef Vector3<double> Vector3d;
	double f_volume[40];


	for(int z = 0; z<meta.dim.z ; ++z)  {
		for(int y = 0; y < meta.dim.y; ++y)  {
			for(int x = 0; x <meta.dim.x ; ++x) {

				int index=z*meta.dim.y*meta.dim.x + y*meta.dim.x + x;
				Vector3d pos=Vector3d((double)x, (double)y, (double)z);
				if (m_pFeatureIndex[index]!=0)
				{
					unsigned char f_SN_idx=m_pFeatureIndex[index];  //feature serial number  index  0  [1- 37]  共38个
					unsigned char scalar=m_pVolume[timeStep][index];

					f_mass[f_SN_idx]+=scalar;  //初始化为0  别忘记
					f_centroid[f_SN_idx]+=pos*scalar*1.0;
					f_volume[f_SN_idx]++;
				}

			}
		}
	}

	for (int i=1;i<featureNum+1;i++)
	{
		f_centroid[i]=f_centroid[i]/f_mass[i];
	}

	//out<<"feature attribute calculated!\n";
	fprintf(fp,"%d\n",featureNum);
	for (int i=1;i<featureNum+1;i++)   //feature serial number  index   [1- 37]  共38个  零  不考虑这里
	{
		 fprintf(fp,"%lf,%lf,%lf,%lf,%lf\n",
			 f_mass[i],f_centroid[i].x,f_centroid[i].y,f_centroid[i].z,f_volume[i]);
	}

	 fclose(fp);

	//-------------------
	//continuation
	//Using centroid to find the closest feature 
	//Test whether the volume  mass and other attribute within the prescribed tolerance

	//-------------------
	//Bifurcation and amalgamation
	//The average weighted centroid, total volume, total mass,


	//-------------------
	//centroid     加权平均
	//坐标* 标量值 / 标量值总和  W
	//volume  总的体素个数


	return true;
}

bool Volume::initFeatureList()
{
	
		//=============== 释放内存
		dag.m_FeatureInfo = new FeatureInfo *[m_timeStep];
		for (int i = 0; i < m_timeStep; i++)
		{ 
			dag.m_FeatureInfo[i] = new FeatureInfo[m_arryNumFeature[i+1]];
		}
		int featureNum=0;
		//==============
		//file 读入
		for (int i = 0; i < m_timeStep; i++)
		{
			char fn3[255];
			memset(fn3, 0, 255);
			//sprintf(fn3, "%s%s%02d%s", m_pFileDir, "feaAttri/",timeStep+1,".fea");// fileDir    /
			sprintf(fn3, "%s%s%02d%s", m_pFileDir, "feaAttri/",i+1,".fea");// fileDir    *******

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
					&(dag.m_FeatureInfo[i][j].mass),   //i : 时间步  【0, x-1 】  j:特征数 【0,x-1】

					&(dag.m_FeatureInfo[i][j].centroid.x), 
					&(dag.m_FeatureInfo[i][j].centroid.y), 
					&(dag.m_FeatureInfo[i][j].centroid.z),

					&(dag.m_FeatureInfo[i][j].volume)
					);
				dag.m_FeatureInfo[i][j].backFlag=0;
				dag.m_FeatureInfo[i][j].forFlag=0;
			}
			
			fclose(fp);
		}
		return true;

}
bool Volume::graphOutput()
{

	if (m_timeStep==0|| m_arryNumFeature==NULL)
	{
		printf("there is no graph  information  ....\n");
		return false;
	}

	char fn3[255];
	memset(fn3, 0, 255);
	sprintf(fn3, "%s%s", m_pFileDir, "graphInfomation.txt");// fileDir    *******  文件名  呵呵呵 痴汉笑


	FILE *fp = fopen(fn3, "w");
	if(fp == NULL) {
		printf("Can't open file %s\n", fn3);
		return false;
	}


	fprintf(fp, "%d\n", m_timeStep);                //  *******
	for (int i=0;i<m_timeStep+1;i++)
	{
		fprintf(fp, "%u ", m_arryNumFeature[i]);                
	}


	struct edgenode *p;
	for (int i=0;i<m_timeStep;i++)//NODE_NUM
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)
		{
			int edgeSum=0;
			p=dag.graph[i][j].first;
			while (p!=NULL)
			{
				p=p->next;        
				edgeSum++;
			}

			fprintf(fp, "%d\n", edgeSum);                
			p=dag.graph[i][j].first;
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
bool Volume::graphInput()
{




	char fn3[255];
	memset(fn3, 0, 255);  //graphInfomation.txt
	sprintf(fn3, "%s%s", m_pFileDir, "graphInfomation.txt");// **


	FILE *fp = fopen(fn3, "r");
	if(fp == NULL) {
		printf("Can't open file %s\n", fn3);
		return false;
	}

	//===============================
	//  time num input 
	fscanf(fp, "%d\n", &m_timeStep);                //  *******

	//===============================
	//  time numArray input 
	if (m_arryNumFeature!=NULL)		
	{
		delete [] m_arryNumFeature;
		m_arryNumFeature=NULL;
	}
	m_arryNumFeature=new unsigned char [m_timeStep+1];

	for (int i=0;i<m_timeStep+1;i++)
		fscanf(fp, "%c ", &m_arryNumFeature[i]);                


	//===============================
	//graph  节点  链表初始化
	dag.graph = new adjlist *[m_timeStep];
	for (int i = 0; i < m_timeStep; i++)
		dag.graph[i] = new adjlist[m_arryNumFeature[i+1]];//

	for(int i=0;i<m_timeStep;i++) 
		for (int j=0;j<= m_arryNumFeature[i+1];j++)
			dag.graph[i][j].first=NULL;


	//===============================
	//节点连接信息读入  图构建完成
	for (int i=0;i<m_timeStep;i++)//NODE_NUM
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)
		{

			struct edgenode *q;

			Vector2i node;
			fscanf(fp, "%d %d", node.x,node.y);                

			q=(struct edgenode *)malloc(sizeof(struct edgenode));
			q->no=node;  //信息读入的说
			q->next=dag.graph[i][j].first;  //边  左边节点的链表

			dag.graph[i][j].first=q;

		}

	}

	fclose(fp);
	return true;

}


void Volume::generateProbs()
{

	std::ofstream cout1("graphInfo.txt");      //    Uf01_char_out.bin 
	if(!cout1) 
		printf("无法打开文件");


	
	
	//create graph and edge cull
	// calculate the forward transition probabilities
	struct edgenode *q,*p,*pre;

	//============
	//初始化 保留  adjlist **graph;

	//int numNode=m_FeatureSum;
	//dag.graph=(adjlist *)malloc( numNode *sizeof(adjlist) );

	dag.graph = new adjlist *[m_timeStep];
	for (int i = 0; i < m_timeStep; i++)
	{ 
		
			dag.graph[i] = new adjlist[m_arryNumFeature[i+1]];//
			
	}



	for(int i=0;i<m_timeStep;i++) 
		for (int j=0;j<= m_arryNumFeature[i+1];j++)
			dag.graph[i][j].first=NULL;

	//////////////////////////////////////////////////////////////////////////
	//continue 判定
	//////////////////////////////////////////////////////////////////////////
	for (int i=0;i<m_timeStep - 1;i++)  //M_TIME_NUM - 1
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)  //放个数组 存储每个时间步里的节点数  第一个时间步
		{
			//cout<<"		CLUSTER_NUM a : "<<j<<endl;
			float total=0.0;

			//////////////////////////////////////////////////////////////////////////
			for (int k=0;k< m_arryNumFeature[i+2];k++)  //***********注意    第二个时间步
			{

				

				// total histogram distance
				double dist = 0.0;
				double volume_diff=0.0;
				bool volume_diff_flag=false;
				// normalized by buckets
				Vector3d centroid1= dag.m_FeatureInfo[i][j].centroid ;   //i 是时间步  j是feature编号
				Vector3d centroid2=dag.m_FeatureInfo[i+1][k].centroid ; 

				double a=dag.m_FeatureInfo[i][j].volume;
				double b=dag.m_FeatureInfo[i+1][k].volume;
				if (a<1000 || b<1000 )
				{
					if ( a>b )
						volume_diff=a/b;
					else
						volume_diff=b/a;

					volume_diff_flag=(volume_diff<10)?true:false;
				}
				else
				{
					if ( a>b )
						volume_diff=(a-b)/a;
					else
						volume_diff=(b-a)/b;

					volume_diff_flag=(volume_diff<0.25)?true:false;
				}
				
				dist = (centroid1-centroid2).norm();
				
				if (dist<10.0)
				{

					//continuation
					// create an edge in the graph
					if (volume_diff_flag)
					{
						std::cout<<volume_diff<<"  "<<dag.m_FeatureInfo[i+1][k].volume<<"  "<<dag.m_FeatureInfo[i][j].volume<<std::endl;
						dag.m_FeatureInfo[i][j].backFlag=CONTINUEW;
						dag.m_FeatureInfo[i+1][k].forFlag=CONTINUEW;

						q=(struct edgenode *)malloc(sizeof(struct edgenode));

						q->no=Vector2i(i+1,k);//(i+1)*CLUSTER_NUM+k;//  边右边节点的  编号					
						q->next=dag.graph[i][j].first;  //边  左边节点的链表

						dag.graph[i][j].first=q;
					}
					
				}

			}

			//图输出 的  嗯嗯
			/*	cout1<<"graph :  "<<i<<" "<<j<<dag.m_FeatureInfo[i][j].centroid<<std::endl;
			p=dag.graph[i][j].first;
			while (p!=NULL)
			{	
			cout1<<p->no<<dag.m_FeatureInfo[p->no.x][p->no.y].centroid<<std::endl;
			p=p->next;
			}*/

		}
	}


	std::cout<<" after continuation ："<<std::endl;
	for (int i=0;i<m_timeStep - 1;i++)  //M_TIME_NUM - 1
	{
		double num_left=0;
		for (int j=0;j<m_arryNumFeature[i+1];j++)  //放个数组 存储每个时间步里的节点数  第一个时间步   forFlag
		{
			if (dag.m_FeatureInfo[i][j].backFlag==CONTINUEW|| dag.m_FeatureInfo[i][j].backFlag==BIFURCATION )
				continue;
			
			num_left++;	
				
			
		} //end for creation j
		std::cout<<i<<" 剩余 ："<<"  "<<num_left<<" 特征总数 ："<<"  "<<(int)m_arryNumFeature[i+1]<<std::endl;
		}

	//int num_amalgamatjion=0;
	//for (int i=0;i<m_timeStep - 1;i++)  //M_TIME_NUM - 1
	//{
	//	for (int j=0;j<m_arryNumFeature[i+1];j++)  //放个数组 存储每个时间步里的节点数  第一个时间步
	//	{
	//		if (dag.m_FeatureInfo[i][j].backFlag==0)
	//			num_amalgamatjion++;
	//	}
	//}

	//////////////////////////////////////////////////////////////////////////
	//bifur 分裂 判定
	//可供融合的特征个数
	//////////////////////////////////////////////////////////////////////////
	
	for (int i=0;i<m_timeStep - 1;i++)  //M_TIME_NUM - 1
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)  //放个数组 存储每个时间步里的节点数  第一个时间步
		{
			if (dag.m_FeatureInfo[i][j].backFlag==CONTINUEW)
				continue;
			
			double num_amalgamation=0;
			for (int k=0;k< m_arryNumFeature[i+2];k++)  //***********注意    第二个时间步
			{

				if (dag.m_FeatureInfo[i+1][k].forFlag==CONTINUEW)
					continue;

				double dist = 0.0;
				
				// normalized by buckets
				Vector3d centroid1= dag.m_FeatureInfo[i][j].centroid ;   //i 是时间步  j是feature编号
				Vector3d centroid2=dag.m_FeatureInfo[i+1][k].centroid ; 
				dist = (centroid1-centroid2).norm();
				
				if (dist<10.0)
				{
					num_amalgamation++;
					dag.m_FeatureInfo[i+1][k].forFlag=DIS;  //这里 有问题

				}
					
				
			}

			if (num_amalgamation<2)
			{
				for (int k=0;k< m_arryNumFeature[i+2];k++)  //***********注意    第二个时间步
				{
					if (dag.m_FeatureInfo[i+1][k].forFlag==DIS)
					{
						dag.m_FeatureInfo[i+1][k].forFlag=0;
					}						
					
				}
				continue;
			}
				
			std::cout<<i<<" * "<<j<<" * "<<num_amalgamation<<std::endl;


			Vector3d sumCentroid=Vector3d(0,0,0);
			double sumMass=0;

			for (int k=0;k< m_arryNumFeature[i+2];k++)  //***********注意    第二个时间步
			{
				if (dag.m_FeatureInfo[i+1][k].forFlag==DIS)
				{
					
					// normalized by buckets
					sumCentroid+= dag.m_FeatureInfo[i+1][k].centroid * dag.m_FeatureInfo[i+1][k].mass ;   //i 是时间步  j是feature编号
					sumMass +=  dag.m_FeatureInfo[i+1][k].mass; 
				}						
					
			}

			Vector3d centroid1= dag.m_FeatureInfo[i][j].centroid ;   //i 是时间步  j是feature编号
			sumCentroid=sumCentroid/sumMass;

			double dist = (centroid1-sumCentroid).norm();

			std::cout<<" * i *  "<<i<<" * j *  "<<j<<" centroid : "<<centroid1<<" centroidSum： : "<<sumCentroid<<" dist ： : "<<dist<<std::endl;
			if (dist<10.0)
			{

				
				dag.m_FeatureInfo[i][j].backFlag=BIFURCATION;
				for (int k=0;k< m_arryNumFeature[i+2];k++)  //***********注意    第二个时间步
				{
					if (dag.m_FeatureInfo[i+1][k].forFlag!=DIS)
						continue;


					//std::cout<<" * i *  "<<i<<" * j *  "<<j<<" * k  * "<<k<<std::endl;
					dag.m_FeatureInfo[i+1][k].forFlag=BIFURCATION;
					

					q=(struct edgenode *)malloc(sizeof(struct edgenode));

					q->no=Vector2i(i+1,k);//(i+1)*CLUSTER_NUM+k;//  边右边节点的  编号					
					q->next=dag.graph[i][j].first;  //边  左边节点的链表

					dag.graph[i][j].first=q;

				}
					
			
			}
		}
	} //for amalmation end




	//////////////////////////////////////////////////////////////////////////
	//amamltation  融合  判定  end  contimutation  and bifurcation
	//可供融合的特征个数
	//////////////////////////////////////////////////////////////////////////  [i-1][k] backFlag     [i][j]forFlag
	for (int i=m_timeStep - 1; i>0;i--)  //M_TIME_NUM - 1
	{
		for (int j=0;j<m_arryNumFeature[i+1];j++)  //放个数组 存储每个时间步里的节点数  第一个时间步   forFlag
		{
			if (dag.m_FeatureInfo[i][j].forFlag==CONTINUEW|| dag.m_FeatureInfo[i][j].forFlag==BIFURCATION)
				continue;
			
			double num_amalgamation=0;
			for (int k=0;k< m_arryNumFeature[i];k++)  //***********注意    第二个时间步
			{

				if (dag.m_FeatureInfo[i-1][k].backFlag==CONTINUEW|| dag.m_FeatureInfo[i][j].forFlag==BIFURCATION)
					continue;

				double dist = 0.0;
				
				// normalized by buckets
				Vector3d centroid1= dag.m_FeatureInfo[i][j].centroid ;   //i 是时间步  j是feature编号
				Vector3d centroid2=dag.m_FeatureInfo[i-1][k].centroid ; 
				dist = (centroid1-centroid2).norm();
				
				if (dist<10.0)
				{
					num_amalgamation++;
					dag.m_FeatureInfo[i-1][k].backFlag=DIS;  //这里 有问题

				}
					
				
			}

			if (num_amalgamation<2)
			{
				for (int k=0;k< m_arryNumFeature[i];k++)  //***********注意    第二个时间步
				{
					if (dag.m_FeatureInfo[i-1][k].backFlag==DIS)
					{
						dag.m_FeatureInfo[i-1][k].backFlag=0;
					}						
					
				}
				continue;
			}
				
			std::cout<<i<<" 合并 * "<<j<<" * "<<num_amalgamation<<std::endl;


			//Vector3d sumCentroid=Vector3d(0,0,0);
			//double sumMass=0;

			//for (int k=0;k< m_arryNumFeature[i+2];k++)  //***********注意    第二个时间步
			//{
			//	if (dag.m_FeatureInfo[i-1][k].backFlag==DIS)
			//	{
			//		
			//		// normalized by buckets
			//		sumCentroid+= dag.m_FeatureInfo[i-1][k].centroid * dag.m_FeatureInfo[i-1][k].mass ;   //i 是时间步  j是feature编号
			//		sumMass +=  dag.m_FeatureInfo[i-1][k].mass; 
			//	}						
			//		
			//}

			//Vector3d centroid1= dag.m_FeatureInfo[i][j].centroid ;   //i 是时间步  j是feature编号
			//sumCentroid=sumCentroid/sumMass;

			//double dist = (centroid1-sumCentroid).norm();

			//std::cout<<" * i *  "<<i<<" * j *  "<<j<<" centroid : "<<centroid1<<" centroidSum： : "<<sumCentroid<<" dist ： : "<<dist<<std::endl;
			//if (dist<10.0)
			//{

			//	
			//	dag.m_FeatureInfo[i][j].forFlag=AMALGAMATION;
			//	for (int k=0;k< m_arryNumFeature[i+2];k++)  //***********注意    第二个时间步
			//	{
			//		if (dag.m_FeatureInfo[i-1][k].backFlag!=DIS)
			//			continue;


			//		//std::cout<<" * i *  "<<i<<" * j *  "<<j<<" * k  * "<<k<<std::endl;
			//		dag.m_FeatureInfo[i-1][k].backFlag=AMALGAMATION;
			//		

			//		q=(struct edgenode *)malloc(sizeof(struct edgenode));

			//		q->no=Vector2i(i+1,k);//(i+1)*CLUSTER_NUM+k;//  边右边节点的  编号					
			//		q->next=dag.graph[i][j].first;  //边  左边节点的链表

			//		dag.graph[i][j].first=q;

			//	}
			//		
			//
			//}//(dist<10.0)
		}
	} //for amalmation end


	//////////////////////////////////////////////////////////////////////////
	//creation  初始化  判定
	//可供融合的特征个数
	//////////////////////////////////////////////////////////////////////////
	std::cout<<" creation ："<<std::endl;
	for (int i=0;i<m_timeStep - 1;i++)  //M_TIME_NUM - 1
	{
		double num_left=0;
		for (int j=0;j<m_arryNumFeature[i+1];j++)  //放个数组 存储每个时间步里的节点数  第一个时间步   forFlag
		{
			if (dag.m_FeatureInfo[i][j].backFlag==CONTINUEW|| dag.m_FeatureInfo[i][j].backFlag==BIFURCATION )
				continue;
			
			num_left++;	
				
			
		} //end for creation j
		std::cout<<i<<" 剩余 ："<<" * "<<num_left<<std::endl;
	}//end for creation  i



	//////////////////////////////////////////////////////////////////////////
	//dissimation  消亡  判定
	//可供融合的特征个数
	//////////////////////////////////////////////////////////////////////////
	std::cout<<" dissimation ："<<std::endl;
	for (int i=1;i<m_timeStep ;i++)  //M_TIME_NUM - 1
	{
		double num_left=0;
		for (int j=0;j<m_arryNumFeature[i+1];j++)  //放个数组 存储每个时间步里的节点数  第一个时间步   forFlag
		{
			if (dag.m_FeatureInfo[i][j].forFlag==CONTINUEW|| dag.m_FeatureInfo[i][j].forFlag==BIFURCATION )
				continue;
			num_left++;	
		} //end for creation j
		std::cout<<i<<" 剩余 ："<<" * "<<num_left<<std::endl;
	}//end for creation  i

	cout1.close();

}
