#ifndef VOLUMEMETA_H
#define VOLUMEMETA_H

#include "util/Vector3.h"

/**
 * ���ڼ�¼�洢����������
 * ��һ��Byte��ʾ���Ӹ�λ����λ�ֱ�Ϊ��
 *	1λ��ʾ�Ƿ��з��ţ�0Ϊ�޷��ţ�1Ϊ�з���
 *	2λ��ʾ�������ͣ�00Ϊ���ͣ�10Ϊ������
 *	5λ��ʾ���ݳ��ȣ���5λ����ֵ��ʾÿ��������ռ��Byte��
 */
typedef enum {
	DATA_TYPE_UINT8 = 0x01,
	DATA_TYPE_UINT16 = 0x02,
	DATA_TYPE_UINT32 = 0x04,
	DATA_TYPE_FLOAT32 = 0x44
}DataType;

typedef struct //�����ڱ��桢��ȡ��ʶ�����ݵ���С��λ
{
	Vector3i dim;		// ���ݳߴ�
	Vector3f spacing;	// �������
	int component_count;// ÿ����������Ԫ�ظ���
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