#ifndef COLORSORTER_H
#define COLORSORTER_H

#include "ColorCluster.h"

class ColorSorter
{
public:
	void sort(const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum = 256);
	void sortByNearestLab(const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum = 256);
//	void sortByVoronoi(const Cluster* clusters, int* minPath, int* idxMap, int clusterNum = 256);
//	void minVoronoiPath();
	void sortHierarchical(const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum = 256);
	void sortBySSM(const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum = 256);
	void sortHierarchical_2(const ColorSample* clusters, int* minPath, int* idxMap, int clusterNum = 256);

	void sortNearestTo(int center, const ColorSample* clusters, int* idxMap, int* mask, int& sidx, int& start, int& span, float threshold = 10.0, int clusterNum = 256);

private:
};

#endif // COLORSORTER_H