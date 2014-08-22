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

	//match_flag12 �Ǵ�ǰ����ƥ���flag��match_flag21 �ǴӺ�ǰƥ���flag
	
	//0 û��match���߱�ʾ��ʧ
	//�������������߷��� ������Ƶ�iλ��1���ʾ���Ӧ���������i������
	//-i��ʾ�ϲ�����һ��ʱ�䲽�ı��i������
	long long match_flag12;

	//0 û��match���߱�ʾ��������
	//�����Ǻϲ� ������Ƶ�iλ��1���ʾ���Ӧ���������i����ǰ��
	//-i��ʾǰһ��ʱ�䲽�ı��i���������Ѳ������������仯��Ӧ��
	long long match_flag21;
};

class feature_list_node{
public:
	int address;
	feature_list_node *next;
};

