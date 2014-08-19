#include "Volume.h"
#include <string.h>

Volume::Volume()
{
	m_alignedDim.set(0, 0, 0);
	m_normalizedDim.set(0.0, 0.0, 0.0);
	m_pFileName = NULL;
	num_of_timestep = 0;
	maxnum_of_features = 0;
	meta.data = NULL;
	meta.dim.set(0, 0, 0);
	meta.spacing.set(1.0, 1.0, 1.0);
	pos_value = NULL;
	grad_value = NULL;
	cal_flag = true;
	load_flag = false;
}

Volume::~Volume()
{
	clear();
}

void Volume::clear()
{
	free(meta.data);
	free(pos_value);
	free(grad_value);
	meta.data = NULL;
	meta.dim.set(0, 0, 0);
	meta.spacing.set(1.0, 1.0, 1.0);
	pos_value = NULL;
	grad_value = NULL;
	m_alignedDim.set(0, 0, 0);
	m_normalizedDim.set(0.0, 0.0, 0.0);
	m_pFileName = NULL;
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

	if(meta.data != NULL)
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

	strcpy(m_FileDir,dataDir);
	//=;  //字符串是个什么东西


//	fscanf(fp, "%s", volumeDataFileName);
	int dtRet = fscanf(fp, "%d%c%d", &dataWide, &dataType, &(meta.component_count));

	switch(dtRet)
	{
	case -1:
	case 0:
		meta.component_count = 1;
		dataType = 'u';
		dataWide = 8;
		break;
	case 1:
		meta.component_count = 1;
		dataType = 'u';
		break;
	case 2:
		meta.component_count = 1;
		break;
	}
	meta.type = getDataType(dataWide / 8, dataType);
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
	m_volumeSize=meta.dim.x * meta.dim.y * meta.dim.z;
	//reloadVolume_all(1);
	reloadVolume_all(1);

	return true;
	


}

bool Volume::loadVolume_process(const char* filename)
{
	if (filename == NULL){
		printf("Filename is null?\n");
		return false;
	}

	FILE *fp = fopen(filename, "r");
	if(fp == NULL) {
		printf("Can't open file %s\n", filename);
		return false;
	}
	printf("---------------------------------\n");
	printf("Data file director file:\t%s\n", filename);
	clear();
	m_pFileName = filename;

	//////////////////////////////////////////////////////////////////////////
	// read volume information
	char data_file_name[1024];
	char pos_value_file_name[1024];
	char pos_value_list_file_name[1024];
	char grad_value_file_name[1024];
	char grad_value_list_file_name[1024];

	char dataType;
	int dataWide;
	char volumeDataFileName[1024];

	fscanf(fp, "%d %d %d\n", &(meta.dim.x), &(meta.dim.y), &(meta.dim.z));
	fscanf(fp, "%f %f %f\n", &(meta.spacing.x), &(meta.spacing.y), &(meta.spacing.z));
	fscanf(fp, "%d\n", &num_of_timestep);
	printf("dim:\t%d\t%d\t%d\n",meta.dim.x,meta.dim.y,meta.dim.z);
	printf("spacing:\t%d\t%d\t%d\n",meta.spacing.x, meta.spacing.y, meta.spacing.z);

	meta.component_count = 1;
	dataType = 'u';
	dataWide = 8;
	meta.type = getDataType(dataWide / 8, dataType);


	m_alignedDim.x = getPow2(meta.dim.x);
	m_alignedDim.y = getPow2(meta.dim.y);
	m_alignedDim.z = getPow2(meta.dim.z);
	m_normalizedDim.set(meta.dim.x * meta.spacing.x, meta.dim.y * meta.spacing.y, meta.dim.z * meta.spacing.z);
	double maxSize = Max(m_normalizedDim.x, Max(m_normalizedDim.y, m_normalizedDim.z));
	m_normalizedDim /= maxSize;
	int volumeSize = meta.dim.x * meta.dim.y * meta.dim.z * meta.component_count * bytePerComponent(meta.type);

	meta.data = (void*)malloc(volumeSize);
	pos_value= (unsigned char*)malloc(volumeSize);
	grad_value= (unsigned char*)malloc(3*volumeSize);
	float *grad_value_tmp = (float *)malloc(3*volumeSize*sizeof(float));

	unsigned char num_of_feature;
	FILE *data_fp,*pos_value_fp,*pos_value_list_fp,*grad_value_fp,*grad_value_list_fp;
	const char* pSlash = strrchr(filename, '/');
	memset(pos_value_list_file_name, 0, 1024);
	memset(grad_value_list_file_name, 0, 1024);
	strncpy(pos_value_list_file_name, filename, pSlash - filename + 1);
	strncpy(grad_value_list_file_name, filename, pSlash - filename + 1);
	strcat(pos_value_list_file_name, "pos.vifo");
	strcat(grad_value_list_file_name, "grad.vifo");
	//	printf("%s %s\n",pos_value_list_file_name,grad_value_list_file_name);

	pos_value_list_fp = fopen(pos_value_list_file_name,"w");
	grad_value_list_fp = fopen(grad_value_list_file_name,"w");
	//fprintf(pos_value_list_fp,"%d\n",num_of_timestep);
	//fprintf(grad_value_list_fp,"%d\n",num_of_timestep);
	for(int i = 1;i <= num_of_timestep;i++){
		fscanf(fp, "%s", volumeDataFileName);
		memset(data_file_name, 0, 1024);
		strncpy(data_file_name, filename, pSlash - filename + 1);
		//printf("data file dir :[%s]\n", dataFile);
		strcat(data_file_name, volumeDataFileName);
		printf("data file:[%s]\n", data_file_name);


		const char* pSlash2 = strrchr(data_file_name, '.');
		memset(pos_value_file_name, 0, 1024);
		strncpy(pos_value_file_name, data_file_name, pSlash2 - data_file_name + 1);
		//printf("data file dir :[%s]\n", dataFile);
		strcat(pos_value_file_name, "pos");
		printf("pos file     :[%s]\n", pos_value_file_name);

		memset(grad_value_file_name, 0, 1024);
		strncpy(grad_value_file_name, data_file_name, pSlash2 - data_file_name + 1);
		//printf("data file dir :[%s]\n", dataFile);
		strcat(grad_value_file_name, "grad");
		printf("grad file     :[%s]\n", grad_value_file_name);


		//////////////////////////////////////////////////////////////////////////
		// read volume data from file
		data_fp = fopen(data_file_name, "rb");
		pos_value_fp = fopen(pos_value_file_name,"wb");
		grad_value_fp = fopen(grad_value_file_name,"wb");
		if(data_fp == NULL) {
			printf("Can't open data file %s\n", data_file_name);
			return false;
		}
		// 根据数据尺寸、每个体素元素个数和数据类型计算总共体数据大小
		if (meta.data != NULL)
		{
			fread(meta.data, 1, volumeSize, data_fp);
			memset(pos_value,0,volumeSize*sizeof(unsigned char));
			calcFeature(pos_value, data<unsigned char>(), getDimension()  ,num_of_feature);
			if(i < 10)fprintf(pos_value_list_fp,"0");
			fprintf(pos_value_list_fp,"%d.pos %d\n",i,(int)num_of_feature);
			if(num_of_feature > maxnum_of_features)
				maxnum_of_features = num_of_feature;
			fwrite(pos_value,1,volumeSize,pos_value_fp);
			printf("pos value calculated!\n");

			memset(grad_value,0,3*volumeSize*sizeof(unsigned char));
			//	memset(grad_value_tmp,0,3*volumeSize*sizeof(float));

			calcGradientCentralDiff(grad_value_tmp, data<unsigned char>(), meta.dim);
			gradientNormalize(grad_value_tmp, meta.dim);
			gradientF2UC(grad_value_tmp, grad_value, meta.dim);
			if(i < 10)fprintf(grad_value_list_fp,"0");
			fprintf(grad_value_list_fp,"%d.grad\n",i);
			fwrite(grad_value,1,3*volumeSize,grad_value_fp);
			printf("grad value calculated!\n");
		}
		else
		{
			printf("meta.data == NULL!\tBad allocate memory!\n");
		}
		fclose(data_fp);
		fclose(pos_value_fp);
		fclose(grad_value_fp);
		printf("*******************data\t%d\tprocessed*****************\n\n\n",i);
	}
	free(grad_value_tmp);
	fclose(fp);
	fclose(grad_value_list_fp);
	fclose(pos_value_list_fp);
	return true;
}

//每次一个新的时间步，重新从文件读入数据  render 里面设置时间步 调用的那个 函数
bool Volume::reloadVolume(int time){

	//////////////////////////////////////////////////////////////////////////
	// read volume information
	char c_time[3];
	if(time < 10){
		c_time[0] = '0';
		c_time[1] = '0'+time;
	}else{
		c_time[0] = '0'+time/10;
		c_time[1] = '0'+time%10;
	}
	c_time[2] = '\0';
	char data_file_name[1024];
	char pos_value_file_name[1024];
	char grad_value_file_name[1024];
	char dataType;
	int dataWide;
	char volumeDataFileName[1024];

	int volumeSize = meta.dim.x * meta.dim.y * meta.dim.z * meta.component_count * bytePerComponent(meta.type);
	//if(meta.data == NULL )
	meta.data = (void*)malloc(volumeSize);
	//if(pos_value == NULL)
	pos_value= (unsigned char*)malloc(volumeSize);
	//if(grad_value == NULL)
	grad_value= (unsigned char*)malloc(3*volumeSize);

	FILE *data_fp,*pos_value_fp,*pos_value_list_fp,*grad_value_fp,*grad_value_list_fp;
	const char* pSlash = strrchr(m_pFileName, '/');
	memset(data_file_name, 0, 1024);
	strncpy(data_file_name, m_pFileName, pSlash - m_pFileName + 1);
	//printf("data file dir :[%s]\n", dataFile);

	strcat(data_file_name, c_time);
	strcat(data_file_name,".data");
	printf("data file:[%s]\n", data_file_name);


	const char* pSlash2 = strrchr(data_file_name, '.');
	memset(pos_value_file_name, 0, 1024);
	strncpy(pos_value_file_name, data_file_name, pSlash2 - data_file_name + 1);
	//printf("data file dir :[%s]\n", dataFile);
	strcat(pos_value_file_name, "pos");
	printf("pos file     :[%s]\n", pos_value_file_name);

	memset(grad_value_file_name, 0, 1024);
	strncpy(grad_value_file_name, data_file_name, pSlash2 - data_file_name + 1);
	//printf("data file dir :[%s]\n", dataFile);
	strcat(grad_value_file_name, "grad");
	printf("grad file     :[%s]\n", grad_value_file_name);


	//////////////////////////////////////////////////////////////////////////
	// read volume data from file
	data_fp = fopen(data_file_name, "rb");
	pos_value_fp = fopen(pos_value_file_name,"rb");
	grad_value_fp = fopen(grad_value_file_name,"rb");
	if(data_fp == NULL||pos_value_fp == NULL||grad_value_fp == NULL) {
		printf("Can't open data file %s\n", data_file_name);
		return false;
	}
	// 根据数据尺寸、每个体素元素个数和数据类型计算总共体数据大小
	if (meta.data != NULL)
	{
		fread(meta.data, 1, volumeSize, data_fp);

		fread(pos_value,1,volumeSize,pos_value_fp);
		printf("pos value read!\n");

		fread(grad_value,1,3*volumeSize,grad_value_fp);
		printf("grad value read!\n");
	}
	else
	{
		printf("meta.data == NULL!\tBad allocate memory!\n");
	}
	fclose(data_fp);
	fclose(pos_value_fp);
	fclose(grad_value_fp);
	return true;
}

//直接把所有文件数据读入，对应时间步，访问对应时间步。
bool Volume::reloadVolume_all(int time){

	//////////////////////////////////////////////////////////////////////////
	// read volume information

	meta.data = (unsigned char*)malloc( m_volumeSize);
	pos_value= (unsigned char*)malloc(m_volumeSize);
	grad_value= (unsigned char*)malloc(3*m_volumeSize);
		

	

		
		char str[255];
		char dataFile[1024];

		//////////////////////////////////////////////////////////////////////////
		//原始体数据读入
		memset(str, 0, 255);
		memset(dataFile, 0, 1024);

		sprintf(str, "%s%02d%s", "char/",time,".bin");
		//strcpy(dataFile,m_pFileDir);
		//strcat(dataFile, str);
		sprintf(dataFile, "%s%s", m_FileDir,str);

		FILE *	fp = fopen(dataFile, "rb");
		if(fp == NULL) {
			std::cout<<"Can't open data file "<<dataFile<<std::endl;
			return false;
		}
		fread(meta.data, 1, m_volumeSize, fp);		
		fclose(fp);

			
		//////////////////////////////////////////////////////////////////////////
		//梯度数据读入
		memset(str, 0, 255);
		memset(dataFile, 0, 1024);

		sprintf(str, "%s%02d%s", "gra/",time,".gra");
		strcpy(dataFile,m_FileDir);
		strcat(dataFile, str);


		fp = fopen(dataFile, "rb");
		if(fp == NULL) {
			std::cout<<"Can't open data file "<<dataFile<<std::endl;
			return false;
		}
		fread(grad_value, 1, 3*m_volumeSize, fp);		
		fclose(fp);



		//////////////////////////////////////////////////////////////////////////
		//特征体数据读入
		memset(str, 0, 255);
		memset(dataFile, 0, 1024);

		sprintf(str, "%s%02d%s", "pos/",time,".pos");
		strcpy(dataFile,m_FileDir);
		strcat(dataFile, str);


		fp = fopen(dataFile, "rb");
		if(fp == NULL) {
			std::cout<<"Can't open data file "<<dataFile<<std::endl;
			return false;
		}
		fread(pos_value, 1, m_volumeSize, fp);		
		fclose(fp);


		printf("time step  %d read!\n",time);

	
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
