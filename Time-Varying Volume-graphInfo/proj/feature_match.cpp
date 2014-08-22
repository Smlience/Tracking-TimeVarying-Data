#include "feature_match.h"
#include "util/Vector3.h"
#include <math.h>

feature_match::feature_match(void)
{
	num_of_features = 0;
	for(int i = 0;i<=MAXIMUM_FEATURE_NUM;i++){
		features[i]->feature_lenth = 0;
		features[i]->match_flag12 = 0;
		features[i]->match_flag21 = 0;
		features[i]->feature_head = new feature_list_node;
		features[i]->feature_tail = features[i]->feature_head;
	}
}


feature_match::~feature_match(void){
	feature_list_node *tmp;
	for(int i = 0;i<=MAXIMUM_FEATURE_NUM;i++){
		while(features[i]->feature_head!=NULL){
			tmp= features[i]->feature_head;
			features[i]->feature_head = features[i]->feature_head->next;
			delete tmp;
		}
	}
}

void feature_match:: add_node(int feature_value,int address){
	features[feature_value]->feature_tail->next = new feature_list_node;
	features[feature_value]->feature_tail= features[feature_value]->feature_tail->next;
	features[feature_value]->feature_tail->address = address;
	features[feature_value]->feature_lenth++;
}

void feature_match::construct_match_list(Vector3i dim,unsigned char *pos_data){
	for(int k = 0; k<dim.x*dim.y*dim.z;k++){
		if(*(pos_data+k)!=0){
			add_node(*(pos_data+k),k);
		}
	}
}

feature_list* feature_match::get_feature_list(int feature_value){
	return features[feature_value];
}

int  feature_match:: feature_same_len(feature_list* a,feature_list *b){
	int sum = 0;
	feature_list_node *ta = a->feature_head->next;
	feature_list_node *tb = b->feature_head->next;
	while(ta!=NULL && tb !=NULL){
		if(ta->address < tb->address)
			ta = ta->next;
		else if(ta->address > tb->address)
			tb = tb->next;
		else
		{
			sum++;
			ta = ta->next;
			tb = tb->next;
		}
	}
	return sum;
}


void feature_match::cal_match(){
	int overlap[MAXIMUM_FEATURE_NUM];
	int overlap_nextstep_id[MAXIMUM_FEATURE_NUM];
	int num_of_overlap;
	for(int i = 1;i <= num_of_features;i++){
		num_of_overlap = 0;
		for(int j = 1;j<=next_time_step->num_of_features;j++){
			if(next_time_step->features[j]->match_flag21 == 0)
				overlap[j] = feature_same_len(features[i],next_time_step->features[j]);
			else overlap[j] = 0;
			if(overlap[j]!= 0)
				overlap_nextstep_id[num_of_overlap++] = j;
		}
		int tid;
		int tmpk;
		int overlap_sum;
		int dif_sum;//下一个时间步与本时间步的这个特征匹配的特征 没有重合部分的面积和
		int min_dif_sum = -1;
		int min_dif_sum_id = -1;
		//所有有重合部分(且没被匹配过)的特征，就是num_of_overlap个，每个特征或者0，或者1，所有的都遍历一遍
		for(int k = 1;k<(int)(pow(2.0,num_of_overlap)-0.5);k++){
			tmpk = k;
			tid = 0;
			overlap_sum = 0;
			dif_sum = 0;
			while(tmpk!=0){
				if(tmpk%2==1){
					overlap_sum += overlap[overlap_nextstep_id[tid]];
					//对应特征的面积减去重合部分，就是剩余部分，记录所有的特征的剩余部分的综合
					dif_sum+=(next_time_step->features[overlap_nextstep_id[tid]]->feature_lenth - overlap[overlap_nextstep_id[tid]]);

				}
				tmpk/=2;
				tid++;
			}
			//比较dif_sum和本时间步被匹配特征剩余的面积，哪个大。
			if(dif_sum < features[i]->feature_lenth - overlap_sum){
				dif_sum = features[i]->feature_lenth - overlap_sum;
			}
			if(min_dif_sum == -1 || min_dif_sum > dif_sum){
				min_dif_sum = dif_sum;
				min_dif_sum_id = k;
			}
		}
		if(min_dif_sum < dif){
			long long tmp_match_flag = 0;
			tid = 0;
			while(min_dif_sum_id != 0){
				if(min_dif_sum_id%2 == 1){
					next_time_step->features[overlap_nextstep_id[tid]]->match_flag21 = -i;
					tmp_match_flag += (long long)(pow(2.0,overlap_nextstep_id[tid])+0.5);
				}
				min_dif_sum_id/=2;
				tid++;
			}
			features[i]->match_flag12 = tmp_match_flag;
		}
	}
	cal_match_from_back();
}


void feature_match::cal_match_from_back(){
	int overlap[MAXIMUM_FEATURE_NUM];
	int overlap_nextstep_id[MAXIMUM_FEATURE_NUM];
	int num_of_overlap;
	for(int i = 1;i<=next_time_step->num_of_features;i++){
		num_of_overlap = 0;
		for(int j = 1;j <= num_of_features;j++){
			if(features[j]->match_flag12 == 0)
				overlap[j] = feature_same_len(features[j],next_time_step->features[i]);
			else overlap[j] = 0;
			if(overlap[j]!= 0)
				overlap_nextstep_id[num_of_overlap++] = j;
		}
		int tid;
		int tmpk;
		int overlap_sum;
		int dif_sum;//下一个时间步与本时间步的这个特征匹配的特征 没有重合部分的面积和
		int min_dif_sum = -1;
		int min_dif_sum_id = -1;
		//所有有重合部分(且没被匹配过)的特征，就是num_of_overlap个，每个特征或者0，或者1，所有的都遍历一遍
		for(int k = 1;k<(int)(pow(2.0,num_of_overlap)-0.5);k++){
			tmpk = k;
			tid = 0;
			overlap_sum = 0;
			dif_sum = 0;
			while(tmpk!=0){
				if(tmpk%2==1){
					overlap_sum += overlap[overlap_nextstep_id[tid]];
					//对应特征的面积减去重合部分，就是剩余部分，记录所有的特征的剩余部分的综合
					dif_sum+=(features[overlap_nextstep_id[tid]]->feature_lenth - overlap[overlap_nextstep_id[tid]]);
				}
				tmpk/=2;
				tid++;
			}
			//比较dif_sum和本时间步被匹配特征剩余的面积，哪个大。
			if(dif_sum < next_time_step->features[i]->feature_lenth - overlap_sum){
				dif_sum = next_time_step->features[i]->feature_lenth - overlap_sum;
			}
			if(min_dif_sum == -1 || min_dif_sum > dif_sum){
				min_dif_sum = dif_sum;
				min_dif_sum_id = k;
			}
		}
		if(min_dif_sum < dif){
			long long tmp_match_flag = 0;
			tid = 0;
			while(min_dif_sum_id != 0){
				if(min_dif_sum_id%2 == 1){
					features[overlap_nextstep_id[tid]]->match_flag12 = -i;
					tmp_match_flag += (long long)(pow(2.0,overlap_nextstep_id[tid])+0.5);
				}
				min_dif_sum_id/=2;
				tid++;
			}
			next_time_step->features[i]->match_flag21 = tmp_match_flag;
		}
	}
}