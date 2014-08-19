#ifndef POSCALCULATOR_H
#define POSCALCULATOR_H
#define THRESHHOLD_VALUE 130

/**
* @brief 计算区域增长的特征的值
* @param [out] pos_value float *  体数据的特征值
* @param [in] data T * 标量体数据
* @param [in] dim Vector3i 数据的维度
* @date 2014/7/14
*/
#define DIS 1

void dfs_pos(unsigned char* pos_value,unsigned char* data,Vector3i dim,Vector3i pos,bool *tmp_flag,unsigned char cur_pos_value,int &num_global);

void calcFeature(unsigned char* pos_value, unsigned char* data, Vector3i dim,unsigned char &num_of_feature);

inline void posF2UC(unsigned char* src, unsigned char* dest, Vector3i dim,unsigned char num_of_feature);

#endif