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
	 * @brief 设置TF当前使用的体数据。
	 * 
	 * 如果volume与正在使用的体不同，则替换之并重新计算直方图及聚类
	 *
	 * @param [in] volume const Volume * 彩色体数据
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
	 * @brief 获取输入索引位置的颜色所在的颜色段
	 * @param [in] cidx int 输入索引
	 * @param [out] start int & 起始索引
	 * @param [out] span int & 跨越的颜色索引个数
	 */
	void getRange(int cidx, int& start, int& span);

	/**
	 * @brief 根据选中颜色进行排序，并获取排序后的用户选中索引，及阈值内的索引区段
	 * @param [in] cidx int 选中的颜色索引
	 * @param [out] sidx int & 选中颜色在排序后的索引
	 * @param [out] start int & 排序后选中区段的起始索引
	 * @param [out] span int & 排序后选中区段的颜色个数
	 * @param [out] idxMap int* 原始index到排序后的index的映射
	 * @date 2013/04/11
	 */
	void sortNearestTo(int cidx, int& sidx, int& start, int& span);

	const int* getIndexMap();

private:
	/**
	 * @brief 计算直方图
	 * @date 2013/01/03
	 */
	void calcHistogram(unsigned char* data, int dim_x, int dim_y, int dim_z);

	/**
	 * @brief 对直方图进行聚类
	 * 
	 * 聚为256类，聚类结果存入m_cluster,每个类对应的颜色存入m_tf,类标号对应数组下标
	 *
	 * @date 2013/01/03
	 */
	void calcHistoCluster(const unsigned int* histo);

	void convertRGB2Lab(const unsigned char* src, unsigned char* dest, int dim_x, int dim_y, int dim_z);

private:
	float distance(const Color4& c1, const Color4& c2);
	float distance(const ColorSample& s, const ColorSample& c);

	/**
	 * @brief 对聚类结果进行排序
	 * 
	 * 根据聚类结果(m_clusterInfo)生成Voronoi图，生成带权无向图，然后找一条最短Hamilton回路
	 * 将该回路经过顶点顺序作为排序结果
	 *
	 * @date 2013/01/07
	 */
	void sortCluster();

	/**
	 * @brief 根据聚类信息生成Cluster体
	 * @date 2013/01/20
	 */
	void genClusterVolume();

	/**
	 * @brief 根据排序后的聚类结果生成TF的颜色
	 * @date 2013/01/07
	 */
	void updateTFColor();

//	void sortByNearestLab(const Cluster* clusters, int* minPath, int* idxMap, int clusterNum = 256);

	/**
	 * @brief 排序策略，对给定的Cluster，根据其颜色值进行排序
	 * @param [out] minPath int * 排序算法给出的最短路，minPath[i] == j表示第i个聚类的ID为j
	 * @param [out] idxMap int * 旧ID到新ID的映射。排序后Cluster位置改变，相应地ID发生改变，
	 *							 idxMap[i] == j表示原来ID为i的Cluster排序后ID为j
	 * @param [in] clusterNum int 聚类数量，同时表示minPath和idxMap数组的大小
	 * @date 2013/01/28
	 */
//	void sortByVoronoi(const Cluster* clusters, int* minPath, int* idxMap, int clusterNum = 255);

	bool loadHistogram(FILE* fp);
	void saveHistogram(FILE* fp);

	void loadClusterResult(FILE* fp);
	void saveClusterResult(FILE* fp);

private:
	unsigned int m_histogram[256][256][256];//64MB

	// 16MB, data value to cluster，每个值表示对应位置的体素所对应的cluster ID(0~256)
	// 聚类ID作为m_tf的索引查找对应Cluster的颜色和不透明度，所以Cluster ID应为排序之后的次序
	unsigned char m_clusterVolume[256][256][256];

	ColorSample* m_clusterInfo; //聚类的信息
	int m_clusterCount;
	//Sample* m_sample;
	unsigned char* m_tf; //1024B, cluster color & opacity
	const Volume* m_volume;
	int color_sample_num; //按 32位 int 来记
	ColorSample *color_sample;
	std::map<unsigned int, unsigned char> ccmap;
	IntensityHistogram* m_clusterHistogram; //存储每个聚类中颜色数量的直方图
	ColorSorter m_sorter;
	int m_idxMap[256];
	int m_sortMask[256];
};

#endif // CLUSTEREDCOLORTF_H