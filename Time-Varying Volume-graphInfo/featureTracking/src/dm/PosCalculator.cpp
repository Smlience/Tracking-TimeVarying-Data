#include "src/util/Vector3.h"
#include "src/util/func.color.h"
#include <stack>
#include "PosCalculator.h"
using namespace std;
/**
* @brief 计算区域增长的特征的值
* @param [out] m_pFeatureIndex float *  体数据的特征值
* @param [in] data T * 标量体数据
* @param [in] dim Vector3i 数据的维度
* @date 2014/7/14
*/


void dfs_pos(unsigned char* m_pFeatureIndex,unsigned char* data,Vector3i dim,Vector3i pos,bool *tmp_flag,unsigned char cur_m_pFeatureIndex,int &num_global)
//   dfs_pos(					m_pFeatureIndex,		   data,		 dim,		  pos,		tmp_flag,			   cur_m_pFeatureIndex,	    num_global);
{
	stack<Vector3i> pos_stack;
	pos_stack.push(pos);
	Vector3i tmp=pos;
	int xy = dim.x*dim.y;
	int tid=tmp.z * xy + tmp.y * dim.x + tmp.x;
	tmp_flag[tid] = 1;
	m_pFeatureIndex[tid] = cur_m_pFeatureIndex;
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
						m_pFeatureIndex[tid] = cur_m_pFeatureIndex;
						pos_stack.push(tmp);
						num_global ++;
					}
				}
			}
		}
	}
}

void calcFeature(unsigned char* m_pFeatureIndex, unsigned char* data, Vector3i dim,unsigned char &num_of_feature)
{
	int num_global = 0;
	int len = dim.x*dim.y*dim.z;
	bool *tmp_flag = new bool[len];
	memset(tmp_flag,0,sizeof(bool)*len);
	assert(m_pFeatureIndex != NULL && data != NULL);
	Vector3i pos,grow;
	int xy = dim.x * dim.y;
	unsigned char cur_m_pFeatureIndex = 0;
	size_t idx = 0;
	memset(m_pFeatureIndex, 0, len * sizeof(unsigned char));
	int num = 0;
	// calculate gradient and the normalized direction
	for(pos.z = 0; pos.z < dim.z; ++pos.z)
	{
		for(pos.y = 0; pos.y < dim.y; ++pos.y)
		{
			for(pos.x = 0; pos.x < dim.x; ++pos.x)
			{
				if(tmp_flag[idx] == 0 && data[idx] >THRESHHOLD_VALUE){
					++cur_m_pFeatureIndex;
					dfs_pos(m_pFeatureIndex,data,dim,pos,tmp_flag,cur_m_pFeatureIndex,num_global);
					//		cout <<"?"<<endl;
				}
				if(data[idx] >THRESHHOLD_VALUE){
					num++;
				}
				else{
						m_pFeatureIndex[idx] = 0;//该点不属于哪个特征
				}
				++idx;
			}
		}
	} 
	printf("\n*num of features:\t%d\t points in features:%d %d\n",(int)cur_m_pFeatureIndex,num,num_global);
	num_of_feature = cur_m_pFeatureIndex;  //特征总个数
	delete []tmp_flag;
	//posF2UC(m_pFeatureIndex,m_pFeatureIndex,dim,40);
}

inline void posF2UC(unsigned char* src, unsigned char* dest, Vector3i dim,unsigned char num_of_feature)
{
	int len = dim.x * dim.y * dim.z;
	for (int i = 0; i < len;i++)
	{
		dest[i] = (unsigned char)(src[i]*255.0/num_of_feature + 0.5);
	}
}