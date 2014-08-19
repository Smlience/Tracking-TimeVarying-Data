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
		Color4 color;//������
		Vector3f lab;
		unsigned char clusterID; //�������Ӧ��ID
		size_t voxelCount; //��������
	}Sample;

	unsigned int m_histogram[256][256][256];//64MB

	// 16MB, data value to cluster��ÿ��ֵ��ʾ��Ӧλ�õ���������Ӧ��cluster ID(0~256)
	// ����ID��Ϊm_tf���������Ҷ�ӦCluster����ɫ�Ͳ�͸���ȣ�����Cluster IDӦΪ����֮��Ĵ���
	unsigned char m_clusterVolume[256][256][256];

	ColorSample* m_clusterInfo; //�������Ϣ
	//Sample* m_sample;
	unsigned char* m_tf; //1024B, cluster color & opacity
	const Volume* m_volume;
	int color_sample_num; //�� 32λ int ����
	Sample *color_sample;
};

#endif // CLUSTEREDCOLOR2DTF_H
