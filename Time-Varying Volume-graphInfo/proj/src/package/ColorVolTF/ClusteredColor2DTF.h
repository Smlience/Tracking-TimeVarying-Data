#ifndef CLUSTEREDCOLOR2DTF_H
#define CLUSTEREDCOLOR2DTF_H

#include "ColorCluster.h"

class Volume;

class ClusteredColor2DTF
{
public:
	ClusteredColor2DTF(void);
	~ClusteredColor2DTF(void);
	void setVolume(const Volume* volume);
	unsigned char* clusters();
	unsigned char* tfData();

protected:

private:
	typedef struct Sample_{
		Color4 color;//样本点
		Vector3f lab;
		unsigned char clusterID; //样本点对应的ID
		size_t voxelCount; //体素数量
	}Sample;

	unsigned int m_histogram[256][256][256];//64MB

	// 16MB, data value to cluster，每个值表示对应位置的体素所对应的cluster ID(0~256)
	// 聚类ID作为m_tf的索引查找对应Cluster的颜色和不透明度，所以Cluster ID应为排序之后的次序
	unsigned char m_clusterVolume[256][256][256];

	ColorSample* m_clusterInfo; //聚类的信息
	//Sample* m_sample;
	unsigned char* m_tf; //1024B, cluster color & opacity
	const Volume* m_volume;
	int color_sample_num; //按 32位 int 来记
	Sample *color_sample;
};

#endif // CLUSTEREDCOLOR2DTF_H
