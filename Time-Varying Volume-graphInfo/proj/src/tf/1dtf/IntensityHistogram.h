#ifndef INTENSITYHISTOGRAM_H
#define INTENSITYHISTOGRAM_H

#include <vector>

class Volume;

class IntensityHistogramEntry
{
public:
	
};

class IntensityHistogram
{
public:
	IntensityHistogram();
	IntensityHistogram(const Volume* pVolumes);
	~IntensityHistogram();
	size_t getBucketCount() const;
	void calcHistogram(const Volume* pVolume);
	int getValue(int i);
	float getNormalized(int i);
	float getLogNormalized(int i);
	void setValue(int i, int v);
	int* getAllValues();

private:
	void clear();

private:
	int m_max;
	int m_min;
	int* m_entry;
};

#endif // INTENSITYHISTOGRAM_H