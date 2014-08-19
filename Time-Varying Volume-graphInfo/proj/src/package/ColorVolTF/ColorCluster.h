#ifndef COLORCLUSTER_H
#define COLORCLUSTER_H

#include "util/Vector3.h"
#include "util/Vector4.h"
#include <stdint.h>

typedef struct{
	Color4 color;
	Vector3f lab;
	union{
		uint32_t entryCount;  //�������ɫ����
		uint32_t clusterID;   //����ID
	};
	uint32_t voxelCount; //��������
}ColorSample;
// typedef struct Sample_{
// 	Color4 color;//������
// 	Vector3f lab;
// 	unsigned char clusterID; //�������Ӧ��ID
// 	size_t voxelCount; //��������
// }Sample;
// 
// typedef struct Cluster_{
// 	Color4 color;
// 	Vector3f lab;
// 	Vector3f hsv;
// 	size_t entryCount; //�������ɫ����
// 	size_t voxelCount; //�������������
// }Cluster;

#endif // COLORCLUSTER_H