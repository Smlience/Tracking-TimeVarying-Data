#pragma once
#include <string.h>
#include "util/Vector3.h"
#define MAXIMUM_FEATURE_NUM 50
#define dif 30

class feature_match
{
public:
	feature_match(void);
	~feature_match(void);
	void add_node(int feature_value,int address);
	void construct_match_list(Vector3i dim,unsigned char *pos_data);

	feature_list *get_feature_list(int feature_value);

	int feature_same_len(feature_list* a,feature_list *b);
	float cal_match_dp(int same[MAXIMUM_FEATURE_NUM],int pre_len,int same_sum,int dif_sum,int ind);
	void cal_match();
	void cal_match_from_back();
private:
	int num_of_features;
	feature_list* features[MAXIMUM_FEATURE_NUM];
	feature_match *next_time_step;
};


class feature_list{
public:
	feature_list_node* feature_head;
	feature_list_node* feature_tail;
	int feature_lenth;

	//match_flag12 是从前到后匹配的flag，match_flag21 是从后到前匹配的flag
	
	//0 没被match或者表示消失
	//正数是连续或者分裂 其二进制第i位是1则表示其对应的特征编号i是其后继
	//-i表示合并到下一个时间步的编号i个特征
	long long match_flag12;

	//0 没被match或者表示新生特征
	//正数是合并 其二进制第i位是1则表示其对应的特征编号i是其前趋
	//-i表示前一个时间步的编号i的特征分裂产生或者连续变化对应它
	long long match_flag21;
};

class feature_list_node{
public:
	int address;
	feature_list_node *next;
};

