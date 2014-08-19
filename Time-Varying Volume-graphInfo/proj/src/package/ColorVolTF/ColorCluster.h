#ifndef COLORCLUSTER_H
#define COLORCLUSTER_H

#include "util/Vector3.h"
#include "util/Vector4.h"
#include <stdint.h>

typedef struct{
	Color4 color;
	Vector3f lab;
	union{
		uint32_t entryCount;  //聚类的颜色数量
		uint32_t clusterID;   //聚类ID
	};
	uint32_t voxelCount; //体素数量
}ColorSample;
// typedef struct Sample_{
// 	Color4 color;//样本点
// 	Vector3f lab;
// 	unsigned char clusterID; //样本点对应的ID
// 	size_t voxelCount; //体素数量
// }Sample;
// 
// typedef struct Cluster_{
// 	Color4 color;
// 	Vector3f lab;
// 	Vector3f hsv;
// 	size_t entryCount; //聚类的颜色数量
// 	size_t voxelCount; //聚类的体素数量
// }Cluster;

#endif // COLORCLUSTER_H