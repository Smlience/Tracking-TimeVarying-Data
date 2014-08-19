#include "IntensityHistogram.h"
#include <ctime>
#include <algorithm>
#include "Color.h"

IntensityHistogram::IntensityHistogram() : m_threshold(0.14), m_pVolume(NULL), m_growSpan(0)
										 , m_alpha(0.4), m_beta(0.3), m_gamma(0.3),m_tao(0.25)
{
	m_entry = new IntensityEntry[256];
}

IntensityHistogram::~IntensityHistogram()
{
	delete []m_entry;
}

void IntensityHistogram::clear()
{
	for (int i = 0; i < 256; ++i)
	{
		clearEntry(&m_entry[i]);
		m_entry[i].scalar = i;
	}

	std::vector<IntensityFeature*>::iterator it = m_vFeature.begin();
	for (; it != m_vFeature.end(); ++ it)
	{
		delete (*it);
	}
	m_vFeature.clear();

	m_vSelected.clear();

	m_vUserSpecified.clear();
}

void IntensityHistogram::clearEntry( IntensityEntry* pEntry )
{
	pEntry->featureLabel = -1;
	pEntry->voxelCount = 0;
	pEntry->baryCenter.set(0.0, 0.0, 0.0);
	pEntry->variance = 0.0;
	pEntry->size = 0.0;
	pEntry->weight = 99999.0;
	pEntry->isSeed = false;
}

void IntensityHistogram::addToEntry( int sv, Vector3d& pos )
{
	if (sv >= 0 && sv < 256)
	{
		m_entry[sv].voxelCount ++;
		m_entry[sv].baryCenter += pos;
	}
}

void IntensityHistogram::calcHistogram( Volume* pVolume )
{
	if (pVolume == NULL)
		return;

	m_pVolume = pVolume;
	clear();
	Vector3i dim = Vector3i(pVolume->getXiSize(), pVolume->getYiSize(), pVolume->getZiSize());
	int dataLen = dim.x * dim.y * dim.z;

	int i,j,k,index;
	double x,y,z;
	unsigned char sv; //scalar value
	float gn;         //gradient normal
	int value, magnitude;
	//double* pGradientNormals = pVolume->getGradientNormals();
	unsigned char* pVolumeData = pVolume->getVolume();
// 	unsigned char* pMagnitude = pVolume->getMagnitude();
// 	double maxGradientNormal = pVolume->maxGradientNormal();
	double xRecip = 1.f / dim.x;
	double yRecip = 1.f / dim.y;
	double zRecip = 1.f / dim.z;

	time_t t1 = time(0);
	//计算每个entry的重心
	index = 0;
	for(i = 0; i < dim.z; ++i) {
		z = i * zRecip;
		for(j = 0; j < dim.y; ++j) {
			y = j * yRecip;
			for(k = 0; k < dim.x; ++k) {
				x = k * xRecip;
				sv = pVolumeData[index];
				addToEntry(sv, Vector3d(x, y, z));
				index ++;
			}
		}
	}
	//计算重心
	for (i = 0; i < 256; ++i)
	{
		if (m_entry[i].voxelCount > 0)
		{
			m_entry[i].baryCenter /= m_entry[i].voxelCount;
		}
	}

	//计算每个Entry的方差
	double distanceToCenter = 0.0;
	for(index = 0, i = 0; i < dim.z; ++i) {
		z = i * zRecip;
		for(j = 0; j < dim.y; ++j) {
			y = j * yRecip;
			for(k = 0; k < dim.x; ++k) {
				x = k * xRecip;
				sv = pVolumeData[index];
				//当前点到重心的距离
				distanceToCenter = (Vector3d(x, y, z) - m_entry[sv].baryCenter).normSquared();
				m_entry[sv].variance += sqrt(distanceToCenter); //用标准差还是方差？
				//计算Entry中所有点到重心的最大距离
				distanceToCenter = sqrt(distanceToCenter);
				if( m_entry[sv].size < distanceToCenter)
					m_entry[sv].size = distanceToCenter;
				index ++;
			}
		}
	}
	for (i = 0; i < 256; ++i)
	{
		if (m_entry[i].voxelCount > 0)
		{
			m_entry[i].variance /= m_entry[i].voxelCount;
		}
	}
	time_t t2 = time(0);
	printf("计算重心与方差[%d ms]\n", t2 - t1);

	//对直方图进行聚类
	clusterRG();
	//统计每个Feature的Entry数量及体素数量
	IntensityFeature* pFeature = NULL;
	for (i = 0; i < 256; ++i)
	{
		if (m_entry[i].featureLabel >= 0)
		{
			pFeature = m_vFeature[m_entry[i].featureLabel];
			pFeature->entryCount ++;
			pFeature->voxelCount += m_entry[i].voxelCount;
			pFeature->vEntry.push_back(i);

			//可以在此计算Feature重心
		}
	}

	time_t t3 = time(0);
	printf("一维传输函数聚类[%d ms]\n", t3 - t2);
	printf("总共花费时间[%d ms]\n", t3 - t1);

	//输出直方图统计结果
// 	printf("Histogram >> \n");
	m_iMaxVoxelCount = 0;
	m_iMinVoxelCount = 999999;
	m_maxEntrySize = 0;
	m_minEntrySize = 999999;
	for (i = 0; i < 256; ++i)
	{
		if (m_entry[i].voxelCount > m_iMaxVoxelCount)
		{
			m_iMaxVoxelCount = m_entry[i].voxelCount;
		}
		else if (m_entry[i].voxelCount < m_iMinVoxelCount)
		{
			m_iMinVoxelCount = m_entry[i].voxelCount;
		}

		if (m_entry[i].size > m_maxEntrySize)
		{
			m_maxEntrySize = m_entry[i].size;
		}
		else if (m_entry[i].size < m_minEntrySize)
		{
			m_minEntrySize = m_entry[i].size;
		}
// 		printf("[%3d] %7d -> (%3.3lf, %3.3lf, %3.3lf) %lf\n", m_entry[i].scalar, m_entry[i].voxelCount, m_entry[i].baryCenter.x,
// 				m_entry[i].baryCenter.y, m_entry[i].baryCenter.z, m_entry[i].variance);
	}

// 	//输出所有Feature的信息
// 	printf("Features >> \n");
// 	std::vector<IntensityFeature*>::iterator it = m_vFeature.begin();
// 	for (; it != m_vFeature.end(); ++it)
// 	{
// 		printf("[%d]\t%d >>", (*it)->label, (*it)->entryCount);
// 		for each(int j in (*it)->vEntry)
// 		{
// 			printf(" %d", j);
// 		}
// 		printf("\n");
// 	}

// 对所有的Feature进行排序
// 	AttributeNormalize(m_vFeatureList);
// 
// 	FeatureSort(m_vFeatureList);
// 	for (int i = 0; i < m_vFeatureList.size(); i++)
// 	{
// 		printf("Feature[%d]:[%f,%f],[voxel count:%f],[color:%u,%u,%u],[entry count:%d]\n", 
// 			m_vFeatureList[i]->label, m_vFeatureList[i]->n_scalar, m_vFeatureList[i]->n_magnitude, m_vFeatureList[i]->n_voxerCount, 
// 			m_vFeatureList[i]->color.r, m_vFeatureList[i]->color.g, m_vFeatureList[i]->color.b, m_vFeatureList[i]->entryCount);
// 	}
	m_vFeature[0]->color.a = 0;
}

void IntensityHistogram::setThreshold( double delta )
{
	m_threshold = delta;
	if (m_pVolume != NULL)
	{
		calcHistogram(m_pVolume);
	}
}

void IntensityHistogram::cluster()
{
	int i;
	int featureLabel = 0;
	int changedEntryCount;
	int seedIdx;
	int maxCount;
	double distance;
	double r, g, b;

	srand(time(0));
	while(true) {
		//		changedEntryCount = 0;

		//选取数据最多的Entry作为种子点
		for(seedIdx = -1, maxCount = 0, i = 0; i < 256; ++i) {
			if(m_entry[i].featureLabel < 0 && m_entry[i].voxelCount > maxCount) {
				maxCount = m_entry[i].voxelCount;
				seedIdx = i;
			}
		}
		if (seedIdx < 0)
			break;

		m_entry[seedIdx].isSeed = true;
		m_entry[seedIdx].weight = 0;
		m_entry[seedIdx].featureLabel = featureLabel;
		// 
		// 		if(maxCount < 100)
		// 			break;
		// 
		// 		// generate a RGB color Crgb triple with random hue value
		hsvToRgb((double)rand() / RAND_MAX, 1.f, 1.f, &r, &g, &b);
		IntensityFeature* f = new IntensityFeature;
		f->label = featureLabel;
		f->variance = m_entry[seedIdx].variance;
		f->baryCenter = m_entry[seedIdx].baryCenter;
		f->entryCount = 0;
		f->voxelCount = 0;
		f->color.set(ColorF2Ub(r), ColorF2Ub(g), ColorF2Ub(b), 1);
// 		f->scalar = refTupleIndex % width;
// 		f->magnitude = refTupleIndex / width;
// 		f->n_scalar = 0.0;
// 		f->n_magnitude = 0.0;
// 		f->n_voxerCount = 0.0;
// 
// 		// 计算与种子点属于同一分类的Entry
// 		labelCount       = 0;
		for(i = 0; i < 256; ++i)
		{
			//计算那些不是其他Feature种子点且有数据的Entry
			if(m_entry[i].isSeed == false && m_entry[i].voxelCount > 0)
			{
				distance =  calcDistance(&(m_entry[i]), f);//(m_entry[i].baryCenter - f->baryCenter).norm() + fabs(m_entry[i].variance - f->variance);
				if(distance < m_threshold && distance < m_entry[i].weight)
				{
					//设置Entry的分类信息，并将其加入到对应的Feature中
					m_entry[i].featureLabel = featureLabel;
					m_entry[i].weight = distance;
				}
			}
		}
		m_vFeature.push_back(f);
		// 		if (changedEntryCount == 0)
		// 			break;

		featureLabel++;
	}

}

void IntensityHistogram::clusterRG()
{
	time_t t1 = time(0);
	int i;
	int featureLabel = 0;
	int changedEntryCount;
	int seedIdx;
	int maxCount;
	double distance;
	double r, g, b;

	srand(time(0));
	while(true) {
		//		changedEntryCount = 0;

		//选取数据最多的Entry作为种子点
		for(seedIdx = -1, maxCount = 0, i = 0; i < 256; ++i) {
			if(m_entry[i].featureLabel < 0 && m_entry[i].voxelCount > maxCount) {
				maxCount = m_entry[i].voxelCount;
				seedIdx = i;
			}
		}
		if (seedIdx < 0/* || maxCount < 100*/)
			break;

		m_entry[seedIdx].isSeed = true;
		m_entry[seedIdx].weight = 0;
		m_entry[seedIdx].featureLabel = featureLabel;

		hsvToRgb((double)rand() / RAND_MAX, 1.f, 1.f, &r, &g, &b);
		IntensityFeature* f = new IntensityFeature;
		f->label = featureLabel;
		f->variance = m_entry[seedIdx].variance;
		f->baryCenter = m_entry[seedIdx].baryCenter;
		f->maxSize = m_entry[seedIdx].size;
		f->entryCount = 0;
		f->voxelCount = 0;
		f->color.set(ColorF2Ub(r), ColorF2Ub(g), ColorF2Ub(b), 1);

		// 计算与种子点属于同一分类的Entry
		// 计算那些不是其他Feature种子点且有数据的Entry
		// 向左增长
		int growSpan = 0;
		i = seedIdx - 1;
		while(i >= 0) {
			if(m_entry[i].isSeed == false && m_entry[i].voxelCount > 0) {
				distance = calcDistance(&(m_entry[i]), f);
				if (distance < m_entry[i].weight)
				{
					m_entry[i].weight = distance;
				}
				if(distance < m_threshold/* && distance < m_entry[i].weight*/)
				{
					//设置Entry的分类信息，并将其加入到对应的Feature中
					m_entry[i].featureLabel = featureLabel;
					f->baryCenter = (f->baryCenter + m_entry[i].baryCenter) / 2;
					//m_entry[i].weight = distance;
					while (growSpan > 0)
					{
						m_entry[i + growSpan].featureLabel = featureLabel;
						growSpan --;
					}
				} else if (growSpan < m_growSpan) {
					growSpan ++;
				} else {
					break;
				}
				i --;
			} else {
				break;
			}
		}

		// 向右增长
		growSpan = 0;
		i = seedIdx + 1;
		while(i < 256) {
			if(m_entry[i].isSeed == false && m_entry[i].voxelCount > 0) {
				distance = calcDistance(&(m_entry[i]), f);
				if (distance < m_entry[i].weight)
				{
					m_entry[i].weight = distance;
				}
				if(distance < m_threshold/* && distance < m_entry[i].weight*/)
				{
					//设置Entry的分类信息，并将其加入到对应的Feature中
					m_entry[i].featureLabel = featureLabel;
					f->baryCenter = (f->baryCenter + m_entry[i].baryCenter) / 2;
					while (growSpan > 0)
					{
						m_entry[i - growSpan].featureLabel = featureLabel;
						growSpan --;
					}
				} else if (growSpan < m_growSpan) {
					growSpan ++;
				} else {
					break;
				}
				i ++;
			} else {
				break;
			}
		}

		m_vFeature.push_back(f);
// 		if (changedEntryCount == 0)
// 			break;

		featureLabel++;
	}

	time_t t2 = time(0);
	printf("一维传输函数聚类[%d ms]\n", t2 - t1);
}

void IntensityHistogram::colorAt( int sv, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a )
{
	int label = m_entry[sv].featureLabel;
	if (label >= 0)
	{
		IntensityFeature* pFeature = m_vFeature[label];
		r = pFeature->color.r;
		g = pFeature->color.g;
		b = pFeature->color.b;
// 		if (pFeature->vEntry.size() > 2 && (sv == pFeature->vEntry.at(0) || sv == pFeature->vEntry.back()))
// 			a = pFeature->color.a / 2;
// 		else
			a = pFeature->color.a;
	}
	else
	{
		r = g = b = a = 0;
	}
}

std::vector<IntensityFeature*> IntensityHistogram::getFeatureList()
{
	return m_vFeature;
}

void IntensityHistogram::setFeatureSelected( int label )
{
	if (label == -1)
	{
		m_vSelected.clear();
		for each(IntensityFeature* pFeature in m_vFeature)
		{
			pFeature->color.a = 1;
		}
		m_vFeature[0]->color.a = 0;
	}
	else
	{
		std::vector<int>::iterator it = std::find(m_vSelected.begin(), m_vSelected.end(), label);
		if (it == m_vSelected.end())
		{
			m_vSelected.push_back(label);
		}
		else
		{
			m_vSelected.erase(it);
		}
		for each(IntensityFeature* pFeature in m_vFeature)
		{
			pFeature->color.a = 0;
		}
		for each(int i in m_vSelected)
		{
			m_vFeature[i]->color.a = 20;
		}
	}
}

void IntensityHistogram::setFeatureSelected( std::vector<int> vLabel )
{
	m_vSelected.clear();
	m_vSelected = vLabel;

	std::vector<IntensityFeature*>::iterator it = m_vFeature.begin();
	for(; it != m_vFeature.end(); ++it)
	{
		(*it)->color.a = 0;
	}

	//为选中特征设置不透明度
// 	for each(int i in m_vSelected)
// 	{
// 		m_vFeature[i]->color.a = 10;
// 	}
	calcOpacityForSelectedFeatures();
}

float IntensityHistogram::getNormalized( int entry )
{
	return (static_cast<float>(m_entry[entry].voxelCount) / static_cast<float>(m_iMaxVoxelCount));
}

float IntensityHistogram::getLogNormalized( int entry )
{
	return logf(static_cast<float>(1 + m_entry[entry].voxelCount )) / log( static_cast<float>(1+m_iMaxVoxelCount));
}

double IntensityHistogram::calcDistance( IntensityEntry* pEntry, IntensityFeature* pFeature )
{
	return fabs(pEntry->size - pFeature->maxSize) + (pEntry->baryCenter - pFeature->baryCenter).norm() + fabs(pEntry->variance - pFeature->variance);
}

void IntensityHistogram::calcOpacityForSelectedFeatures()
{
	//通过计算重要性对特征进行排序,并赋予不透明度

	//计算特征的重要性
	for each(int i in m_vSelected)
	{
		measureFeature(m_vFeature[i]);
	}

// 	//根据特征的重要性对特征进行排序
// 	IntensityFeature * swapFeature = NULL;
// 	int listSize = m_vSelected.size();
// 	for (int i = 0; i < listSize; i++)
// 	{
// 		for (int j = i + 1; j < listSize; ++j)
// 		{
// 			if (m_vFeature[i]->weight < m_vFeature[j]->weight)
// 			{
// 				swapFeature = m_vFeature[i];
// 				m_vFeature[i] = m_vFeature[j];
// 				m_vFeature[j] = swapFeature;
// 			}
// 		}
// 	}

	//
	for each(int i in m_vSelected)
	{
		m_vFeature[i]->color.a = (unsigned char)(pow(m_vFeature[i]->weight, m_tao) + 0.5);
	}

	for each(std::pair<int, int> i in m_vUserSpecified)
	{
		// 如果特征被选择了，才使用用户指定的不透明度
		if (std::find(m_vSelected.begin(), m_vSelected.end(), i.first) != m_vSelected.end())
		{
			m_vFeature[i.first]->color.a = i.second;
		}
	}
}

void IntensityHistogram::measureFeature( IntensityFeature* pFeature )
{
	float nCount = float(pFeature->voxelCount - m_iMinVoxelCount) / (m_iMaxVoxelCount - m_iMinVoxelCount);
	float nSize  = float(pFeature->maxSize - m_minEntrySize) / (m_maxEntrySize - m_minEntrySize);
	float weight = nSize / nCount /** m_alpha*/ /** m_beta*/;
	pFeature->weight = weight;
}

void IntensityHistogram::addUserSpecifiedOpacity( int label, int opacity )
{
	if (label < 0 || label >= m_vFeature.size())
		return;

	std::vector<std::pair<int, int>>::iterator it = m_vUserSpecified.begin();
	for (; it != m_vUserSpecified.end(); ++ it)
	{
		if (it->first == label)
		{
			if (it->second != opacity)
			{
				it->second = opacity;
			}
			return;
		}
	}

	m_vUserSpecified.push_back(std::make_pair(label, opacity));
}

void IntensityHistogram::setTao( double t )
{
	m_tao = t;
}
