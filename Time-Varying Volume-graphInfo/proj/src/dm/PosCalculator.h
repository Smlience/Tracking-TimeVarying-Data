#ifndef POSCALCULATOR_H
#define POSCALCULATOR_H
#define THRESHHOLD_VALUE 130

/**
* @brief ��������������������ֵ
* @param [out] pos_value float *  �����ݵ�����ֵ
* @param [in] data T * ����������
* @param [in] dim Vector3i ���ݵ�ά��
* @date 2014/7/14
*/
#define DIS 1

void dfs_pos(unsigned char* pos_value,unsigned char* data,Vector3i dim,Vector3i pos,bool *tmp_flag,unsigned char cur_pos_value,int &num_global);

void calcFeature(unsigned char* pos_value, unsigned char* data, Vector3i dim,unsigned char &num_of_feature);

inline void posF2UC(unsigned char* src, unsigned char* dest, Vector3i dim,unsigned char num_of_feature);

#endif