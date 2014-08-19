#ifndef VOLUMEMETA_H
#define VOLUMEMETA_H

#include "util/Vector3.h"

/**
 * 用于记录存储的数据类型
 * 用一个Byte表示，从高位到低位分别为：
 *	1位表示是否有符号：0为无符号，1为有符号
 *	2位表示数据类型：00为整型，10为浮点型
 *	5位表示数据长度：后5位的数值表示每个体素所占的Byte数
 */
typedef enum {
	DATA_TYPE_UINT8 = 0x01,
	DATA_TYPE_UINT16 = 0x02,
	DATA_TYPE_UINT32 = 0x04,
	DATA_TYPE_FLOAT32 = 0x44
}DataType;

typedef struct //可用于保存、读取、识别数据的最小单位
{
	Vector3i dim;		// 数据尺寸
	Vector3f spacing;	// 采样间隔
	int component_count;// 每个体素数据元素个数
	void* data;
	DataType type;
}VolumeMeta;

inline int bytePerComponent(DataType type)
{
	return (type & 0x1f);
}

inline DataType getDataType( int byteCount, char type )
{
	unsigned char ret = 0;
	if (type == 'f')
	{
		ret |= 0x40;
	}
	ret |= byteCount;
	// 	switch(byteCount)
	// 	{
	// 	case 8:
	// 		ret |= 0x08;
	// 		break;
	// 	case 16:
	// 		ret |= 0x10;
	// 		break;
	// 	case 32:
	// 		ret |= 0x20;
	// 		break;
	// 	}
	return (DataType)ret;
}

#endif // VOLUMEMETA_H