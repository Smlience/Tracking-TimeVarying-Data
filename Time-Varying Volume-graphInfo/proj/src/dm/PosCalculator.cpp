#include "util/Vector3.h"
#include "util/func.color.h"
#include <stack>
#include "PosCalculator.h"
using namespace std;
/**
* @brief 计算区域增长的特征的值
* @param [out] pos_value float *  体数据的特征值
* @param [in] data T * 标量体数据
* @param [in] dim Vector3i 数据的维度
* @date 2014/7/14
*/


void dfs_pos(unsigned char* pos_value,unsigned char* data,Vector3i dim,Vector3i pos,bool *tmp_flag,unsigned char cur_pos_value,int &num_global){
	stack<Vector3i> pos_stack;
	pos_stack.push(pos);
	Vector3i tmp=pos;
	int xy = dim.x*dim.y;
	int tid=tmp.z * xy + tmp.y * dim.x + tmp.x;
	tmp_flag[tid] = 1;
	pos_value[tid] = cur_pos_value;
	num_global ++;
	while(!pos_stack.empty()){
		pos = pos_stack.top();
		pos_stack.pop();
		for(tmp.x =(pos.x-DIS) > 0?(pos.x-DIS):0;tmp.x <=pos.x+DIS && tmp.x < dim.x;tmp.x++){
			for(tmp.y =(pos.y-DIS)> 0?(pos.y-DIS):0;tmp.y <=pos.y+DIS && tmp.y < dim.y;tmp.y++){
				for(tmp.z =(pos.z-DIS)> 0?(pos.z-DIS):0;tmp.z <=pos.z+DIS && tmp.z < dim.z;tmp.z++){
					tid = tmp.z * xy + tmp.y * dim.x + tmp.x;
					if(tmp_flag[tid]== 0 && data[tid] > THRESHHOLD_VALUE){
						tmp_flag[tid] = 1;
						pos_value[tid] = cur_pos_value;
						pos_stack.push(tmp);
						num_global ++;
					}
				}
			}
		}
	}
}

void calcFeature(unsigned char* pos_value, unsigned char* data, Vector3i dim,unsigned char &num_of_feature)
{
	int num_global = 0;
	int len = dim.x*dim.y*dim.z;
	bool *tmp_flag = new bool[len];
	memset(tmp_flag,0,sizeof(bool)*len);
	assert(pos_value != NULL && data != NULL);
	Vector3i pos,grow;
	int xy = dim.x * dim.y;
	unsigned char cur_pos_value = 0;
	size_t idx = 0;
	memset(pos_value, 0, len * sizeof(unsigned char));
	int num = 0;
	// calculate gradient and the normalized direction
	for(pos.z = 0; pos.z < dim.z; ++pos.z)
	{
		for(pos.y = 0; pos.y < dim.y; ++pos.y)
		{
			for(pos.x = 0; pos.x < dim.x; ++pos.x)
			{
				if(tmp_flag[idx] == 0 && data[idx] >THRESHHOLD_VALUE){
					++cur_pos_value;
					dfs_pos(pos_value,data,dim,pos,tmp_flag,cur_pos_value,num_global);
					//		cout <<"?"<<endl;
				}
				if(data[idx] >THRESHHOLD_VALUE){
					num++;
				}
				else{
						pos_value[idx] = 0;
				}
				++idx;
			}
		}
	} 
	printf("\n*num of features:\t%d\t points in features:%d %d\n",(int)cur_pos_value,num,num_global);
	num_of_feature = cur_pos_value;
	delete []tmp_flag;
	posF2UC(pos_value,pos_value,dim,40);
}

inline void posF2UC(unsigned char* src, unsigned char* dest, Vector3i dim,unsigned char num_of_feature)
{
	int len = dim.x * dim.y * dim.z;
	for (int i = 0; i < len;i++)
	{
		dest[i] = (unsigned char)(src[i]*255.0/num_of_feature + 0.5);
	}
}