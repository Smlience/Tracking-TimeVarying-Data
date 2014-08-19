#ifndef CLUSTEREDCOLORTF_H
#define CLUSTEREDCOLORTF_H

#include "dm/Volume.h"
#include "ColorSorter.h"
#include "util/Vector4.h"
#include <map>

class IntensityHistogram;

class ClusteredColorTF
{
public:
	enum TFSize{
		INTENSITY = 256,
		ELEMENT = 4
	};
	ClusteredColorTF();
	~ClusteredColorTF();

	void clear();
	int find(int n,Color4  cluster );

	/**
	 * @brief ����TF��ǰʹ�õ������ݡ�
	 * 
	 * ���volume������ʹ�õ��岻ͬ�����滻֮�����¼���ֱ��ͼ������
	 *
	 * @param [in] volume const Volume * ��ɫ������
	 * @date 2013/01/03
	 */
	void setVolume(const Volume* volume);

	unsigned int* histogram();

	unsigned char* clusters();

	unsigned char* tfData();

	const ColorSample* getClusterInfo();

	int getClusterCount() const;

	IntensityHistogram* getClusterHistogram();

	/**
	 * @brief ��ȡ��������λ�õ���ɫ���ڵ���ɫ��
	 * @param [in] cidx int ��������
	 * @param [out] start int & ��ʼ����
	 * @param [out] span int & ��Խ����ɫ��������
	 */
	void getRange(int cidx, int& start, int& span);

	/**
	 * @brief ����ѡ����ɫ�������򣬲���ȡ�������û�ѡ������������ֵ�ڵ���������
	 * @param [in] cidx int ѡ�е���ɫ����
	 * @param [out] sidx int & ѡ����ɫ������������
	 * @param [out] start int & �����ѡ�����ε���ʼ����
	 * @param [out] span int & �����ѡ�����ε���ɫ����
	 * @param [out] idxMap int* ԭʼindex��������index��ӳ��
	 * @date 2013/04/11
	 */
	void sortNearestTo(int cidx, int& sidx, int& start, int& span);

	const int* getIndexMap();

private:
	/**
	 * @brief ����ֱ��ͼ
	 * @date 2013/01/03
	 */
	void calcHistogram(unsigned char* data, int dim_x, int dim_y, int dim_z);

	/**
	 * @brief ��ֱ��ͼ���о���
	 * 
	 * ��Ϊ256�࣬����������m_cluster,ÿ�����Ӧ����ɫ����m_tf,���Ŷ�Ӧ�����±�
	 *
	 * @date 2013/01/03
	 */
	void calcHistoCluster(const unsigned int* histo);

	void convertRGB2Lab(const unsigned char* src, unsigned char* dest, int dim_x, int dim_y, int dim_z);

private:
	float distance(const Color4& c1, const Color4& c2);
	float distance(const ColorSample& s, const ColorSample& c);

	/**
	 * @brief �Ծ�������������
	 * 
	 * ���ݾ�����(m_clusterInfo)����Voronoiͼ�����ɴ�Ȩ����ͼ��Ȼ����һ�����Hamilton��·
	 * ���û�·��������˳����Ϊ������
	 *
	 * @date 2013/01/07
	 */
	void sortCluster();

	/**
	 * @brief ���ݾ�����Ϣ����Cluster��
	 * @date 2013/01/20
	 */
	void genClusterVolume();

	/**
	 * @brief ���������ľ���������TF����ɫ
	 * @date 2013/01/07
	 */
	void updateTFColor();

//	void sortByNearestLab(const Cluster* clusters, int* minPath, int* idxMap, int clusterNum = 256);

	/**
	 * @brief ������ԣ��Ը�����Cluster����������ɫֵ��������
	 * @param [out] minPath int * �����㷨���������·��minPath[i] == j��ʾ��i�������IDΪj
	 * @param [out] idxMap int * ��ID����ID��ӳ�䡣�����Clusterλ�øı䣬��Ӧ��ID�����ı䣬
	 *							 idxMap[i] == j��ʾԭ��IDΪi��Cluster�����IDΪj
	 * @param [in] clusterNum int ����������ͬʱ��ʾminPath��idxMap����Ĵ�С
	 * @date 2013/01/28
	 */
//	void sortByVoronoi(const Cluster* clusters, int* minPath, int* idxMap, int clusterNum = 255);

	bool loadHistogram(FILE* fp);
	void saveHistogram(FILE* fp);

	void loadClusterResult(FILE* fp);
	void saveClusterResult(FILE* fp);

private:
	unsigned int m_histogram[256][256][256];//64MB

	// 16MB, data value to cluster��ÿ��ֵ��ʾ��Ӧλ�õ���������Ӧ��cluster ID(0~256)
	// ����ID��Ϊm_tf���������Ҷ�ӦCluster����ɫ�Ͳ�͸���ȣ�����Cluster IDӦΪ����֮��Ĵ���
	unsigned char m_clusterVolume[256][256][256];

	ColorSample* m_clusterInfo; //�������Ϣ
	int m_clusterCount;
	//Sample* m_sample;
	unsigned char* m_tf; //1024B, cluster color & opacity
	const Volume* m_volume;
	int color_sample_num; //�� 32λ int ����
	ColorSample *color_sample;
	std::map<unsigned int, unsigned char> ccmap;
	IntensityHistogram* m_clusterHistogram; //�洢ÿ����������ɫ������ֱ��ͼ
	ColorSorter m_sorter;
	int m_idxMap[256];
	int m_sortMask[256];
};

#endif // CLUSTEREDCOLORTF_H