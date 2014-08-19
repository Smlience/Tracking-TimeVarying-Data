#ifndef INTENSITYHISTOGRAM_H
#define INTENSITYHISTOGRAM_H

#include "util/Vector3.h"
#include "util/Vector4.h"
#include "dm/Volume.h"
#include <vector>

typedef struct IntensityEntry
{
	int featureLabel;
	int scalar;
	int voxelCount;
	Vector3d baryCenter;
	double variance;
	double size;
	double weight;//在聚类时用于存放entry与Feature之间的差异
	bool isSeed;
} IntensityEntry;

typedef struct IntensityFeature
{
	int label;
	int entryCount;
	int voxelCount;
	Vector3d baryCenter;
	double variance;
	double maxSize;
	double weight;//用于Feature的排序
	Color4 color;
	std::vector<int> vEntry;
} IntensityFeature;

class IntensityHistogram
{
public:
	IntensityHistogram();
	~IntensityHistogram();
	void calcHistogram(Volume* pVolume);
	void setThreshold(double delta);
	void colorAt(int sv, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);
	std::vector<IntensityFeature*> getFeatureList();
	void setFeatureSelected(int label);
	void setFeatureSelected(std::vector<int> vLabel);
	float getNormalized(int entry);
	float getLogNormalized(int entry);
	void setSortWeight( float a, float b, float c );
	void addUserSpecifiedOpacity(int label, int opacity);
	void removeUserSpecifiedOpacity(int label);
	void setTao(double t);

private:
	void clear();
	void clearEntry(IntensityEntry* pEntry);
	void cluster();
	void clusterRG();
	void addToEntry(int sv, Vector3d& pos);
	double calcDistance(IntensityEntry* pEntry, IntensityFeature* pFeature);
	void calcOpacityForSelectedFeatures();
	void measureFeature(IntensityFeature* pFeature);

private:
	IntensityEntry* m_entry;
	std::vector<IntensityFeature*> m_vFeature;
	std::vector<int> m_vSelected;
	std::vector<std::pair<int, int>> m_vUserSpecified;
	double m_threshold;
	int m_iMaxVoxelCount;
	int m_iMinVoxelCount;
	double m_maxEntrySize;
	double m_minEntrySize;
	Volume* m_pVolume;
	int m_growSpan;
	float m_alpha;
	float m_beta;
	float m_gamma;
	double m_tao;
};

#endif // INTENSITYHISTOGRAM_H