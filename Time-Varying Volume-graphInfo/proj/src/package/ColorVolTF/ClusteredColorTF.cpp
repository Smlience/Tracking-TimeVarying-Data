#include "ClusteredColorTF.h"
#include "util\Functions.h"
#include "util\util.h"
#include "tf\1dtf\IntensityHistogram.h"
#include <vector>
#include <time.h>

ClusteredColorTF::ClusteredColorTF() : m_volume(NULL), color_sample(NULL), m_clusterCount(INTENSITY)
{
    m_clusterInfo = new ColorSample[INTENSITY];
    m_tf = new unsigned char[INTENSITY * ELEMENT];
    m_clusterHistogram = new IntensityHistogram;
	color_sample_num=0;
	//m_clusterCount = 4; /// ----------------> 测试用数据，分2类
    clear();
}

ClusteredColorTF::~ClusteredColorTF()
{
    m_volume = NULL;
    SAFE_DELETE(m_clusterHistogram);
    SAFE_DELETE_ARRAY(color_sample);
    SAFE_DELETE_ARRAY(m_clusterInfo);
    SAFE_DELETE_ARRAY(m_tf);
}

void ClusteredColorTF::clear()
{
    memset(m_histogram, 0, 256 * 256 * 256 * sizeof(unsigned int));
    for (int i = 0; i < 256; ++i)
    {
        m_sortMask[i] = -1;
        m_idxMap[i] = i;
    }
}

unsigned int* ClusteredColorTF::histogram()
{
    return &(m_histogram[0][0][0]);
}

unsigned char* ClusteredColorTF::clusters()
{
    return &(m_clusterVolume[0][0][0]);
}

unsigned char* ClusteredColorTF::tfData()
{
    return m_tf;
}

void ClusteredColorTF::saveHistogram(FILE* fp)
{
    printf("Save Histogram to file.\n");
    //保存直方图
    fwrite(m_histogram, sizeof(unsigned int), 256 * 256 * 256, fp);
    printf("Histogram Saved.\n");
}

bool ClusteredColorTF::loadHistogram( FILE* fp )
{
    printf("Load Histogram from file......");
    //读取直方图
	size_t ret = fread(m_histogram, sizeof(unsigned int), 256 * 256 * 256, fp);
	printf("Histogram Loaded..\n");
	if (ret == 256 * 256 * 256)
		return true;
	return false;
}

void ClusteredColorTF::saveClusterResult( FILE* fp )
{
	printf("Save Cluster to file.\n");
	//保存聚类结果
	fwrite(&color_sample_num, sizeof(int), 1, fp);
	fwrite(color_sample, sizeof(ColorSample), color_sample_num, fp);
	fwrite(&m_clusterCount, sizeof(int), 1, fp);
	fwrite(m_clusterInfo, sizeof(ColorSample), m_clusterCount, fp);
	printf("Cluster Saved.\n");
}

void ClusteredColorTF::loadClusterResult( FILE* fp )
{
	printf("Load Cluster from file......");
	//读取聚类结果
	fread(&color_sample_num, sizeof(int), 1, fp);
	SAFE_DELETE_ARRAY(color_sample);
	color_sample = new ColorSample[color_sample_num];
	fread(color_sample, sizeof(ColorSample), color_sample_num, fp);
	fread(&m_clusterCount, sizeof(int), 1, fp);
	memset(m_clusterInfo, 0, sizeof(ColorSample) * INTENSITY);
	fread(m_clusterInfo, sizeof(ColorSample), m_clusterCount, fp);
	printf("Cluster Loaded..\n");
}

void ClusteredColorTF::setVolume( const Volume* volume )
{
    if (volume != NULL)
    {
        m_volume = volume;
        unsigned char* data = volume->data<unsigned char>();
        Vector3i dim = volume->getDimension();

        //检查直方图及聚类结果是否已经保存
        char fn_his[256];
		char fn_cluster[256];
        memset(fn_his, 0, 256);
		memset(fn_cluster, 0, 256);

        sprintf(fn_his, "%s.his", volume->getFileName());
		sprintf(fn_cluster, "%s.cls", volume->getFileName());
		//////////////////////////////////////////////////////////////////////////
		// 加载直方图
        FILE* fp_his = fopen(fn_his, "rb");
        if(fp_his != NULL && loadHistogram(fp_his))//已保存过，并读取成功
        {
			fclose(fp_his);
        }
		else if(fp_his != NULL)
		{
			fclose(fp_his);
		}
        else
        {
            //没有保存，则计算直方图并进行聚类
			calcHistogram(data, dim.x, dim.y, dim.z);
            //计算结束，保存结果
            fp_his = fopen(fn_his, "wb");
            saveHistogram(fp_his);
        }

		//////////////////////////////////////////////////////////////////////////
		// 加载聚类信息
	    FILE* fp_cluster = fopen(fn_cluster, "rb");
		if (fp_cluster != NULL)
		{
			loadClusterResult(fp_cluster);
			std::map<unsigned int, unsigned char>::iterator it;
			for (int i = 0; i < color_sample_num; ++i)
			{
				//		ccmap.insert(std::make_pair(color2uint(color_sample[i].color), color_sample[i].clusterID));
				ccmap.insert(std::make_pair(color2uint(color_sample[i].color), color_sample[i].clusterID));
			}
		}
		else
		{
			m_clusterCount = INTENSITY;
			calcHistoCluster(&m_histogram[0][0][0]);
			fp_cluster = fopen(fn_cluster, "wb");
			saveClusterResult(fp_cluster);

			//对聚类结果进行排序
			sortCluster();
		}
	    fclose(fp_cluster);

        //printf("1 ==> Color_sample[0]:%d,%d,%d -> count:%d, cid:%d\n", color_sample[0].color.r, color_sample[0].color.g, color_sample[0].color.b, color_sample[0].voxelCount, color_sample[0].clusterID);
        //printf("2 ==> Color_sample[0]:%d,%d,%d -> count:%d, cid:%d\n", color_sample[0].color.r, color_sample[0].color.g, color_sample[0].color.b, color_sample[0].voxelCount, color_sample[0].clusterID);
        genClusterVolume();
        updateTFColor();
    }
    //printf("Black:[%d]\n",m_histogram[0][0][0]);
}

void ClusteredColorTF::calcHistogram( unsigned char* data, int dim_x, int dim_y, int dim_z )
{
    clear();
    int len = dim_x * dim_y * dim_z * 3;
    for(int i = 0; i < len; i+=3)
    {
        m_histogram[data[i]][data[i+1]][data[i+2]] ++;
    }

    printf("Voxel Count : %d\n", len / 3);

    //////////////////////////////////////////////////////////////////////////
    //将直方图写入文件
// 	char fname[100];
// 	sprintf(fname, "%s.chis", m_volume->getFileName());
// 	FILE* file = fopen(fname, "wb");
// 	if (file != NULL)
// 	{
// 		fwrite(m_histogram, 256*256*256*sizeof(unsigned int), 1, file);
// 		fclose(file);
// 	}
    //////////////////////////////////////////////////////////////////////////
    //验证直方图统计是否正确
// 	unsigned int c = 0;
// 	for (int i = 0; i < 256; ++i)
// 		for (int j = 0; j < 256; ++j)
// 			for (int k = 0; k < 256; ++k)
// 				//c += m_histogram[i][j][k];
// 				if(m_histogram[i][j][k] > 0)c++;
// 	printf("%u/%d\n", c, dim_x * dim_y * dim_z);
    //////////////////////////////////////////////////////////////////////////
}

void ClusteredColorTF::calcHistoCluster( const unsigned int* histo )
{
    if(histo == NULL)
        return;

    //////////////////////////////////////////////////////////////////////////
    // 选取种子点
// 	std::vector<Vector3i> points;
// 	// 获取所有非0点
// 	for (int i = 0; i < 256; ++i)
// 		for (int j = 0; j < 256; ++j)
// 			for (int k = 0; k < 256; ++k)
// 				if(m_histogram[i][j][k] > 0)
// 					points.push_back(Vector3i(i, j ,k));
    //////////////////////////////////////////////////////////////////////////

// 	{
// 		//测试用，随机生成256个聚类的颜色
// 		srand((unsigned)time(0));
// 		unsigned char r,g,b;
// 		for(int i = 0; i < INTENSITY; ++i)
// 		{
// 			r = rand() % 256;
// 			g = rand() % 256;
// 			b = rand() % 256;
// 			m_clusterInfo[i].color.set(r, g, b, 255);
// 		}
// 		return;
// 	}

    float tmp_L, tmp_a, tmp_b;
    int idx = 256 * 256 * 256;
    unsigned int* ptr = &(m_histogram[0][0][0]);
	color_sample_num = 0;
    for(int i = 0; i < idx; ++i, ++ptr)
    {
        if (*ptr != 0)
        {
            color_sample_num++;
        }
    }

    color_sample = new ColorSample[color_sample_num];
    ColorSample* cur_sample = &(color_sample[0]);
    //整理待聚类数据
    for (int i=0; i<256; i++)
    {
        for (int j=0; j<256; j++)
        {
            for (int k=0; k<256; k++)
            {
                if (m_histogram[i][j][k]!=0)
                {
                    RGB2Lab3f(i, j, k, tmp_L, tmp_a, tmp_b);
                    cur_sample->color.set(i, j, k, 0);
                    cur_sample->lab.set(tmp_L, tmp_a, tmp_b);
                    cur_sample->clusterID = 0;
                    cur_sample->voxelCount = m_histogram[i][j][k];
                    ++cur_sample;
                }
            }
        }
    }
//printf("1 ==> Color_sample[0]:%d,%d,%d -> count:%d, cid:%d\n", color_sample[0].color.r, color_sample[0].color.g, color_sample[0].color.b, color_sample[0].voxelCount, color_sample[0].clusterID);

    //////////////////////////////////////////////////////////////////////////
    // 选取种子点
    srand(time(NULL));
    /////////////////////////////////////
    // 方法一:
    // 随机256聚类中心点的产生
// 	for (int i=0;i<256;)
// 	{
// 		unsigned char R=rand()%256;
// 		unsigned char G=rand()%256;
// 		unsigned char B=rand()%256;
// 		Color4 random_cluster(R,G,B,0);
//
// 		if(!find(i,random_cluster))
// 		{
// 			RGB2Lab3f(R, G, B, tmp_L, tmp_a, tmp_b);
// 			m_clusterInfo[i].color=random_cluster;
// 			m_clusterInfo[i].lab.set(tmp_L, tmp_a, tmp_b);
// 			m_clusterInfo[i].entryCount=0;
// 			m_clusterInfo[i++].voxelCount=0;
// 		}
// 	}

    /////////////////////////////////////
    // 方法二:
    // 从所有数据中随机取256个
    // 添加黑色作为第一个种子点

    RGB2Lab3f(0, 0, 0, tmp_L, tmp_a, tmp_b);
    m_clusterInfo[0].color.set(0, 0, 0, 0);
    m_clusterInfo[0].lab.set(tmp_L, tmp_a, tmp_b);
    m_clusterInfo[0].entryCount=0;
    m_clusterInfo[0].voxelCount=0;
	int findNext = 0;
	if (color_sample_num <= m_clusterCount)
	{
		for (int i = 0; i< color_sample_num;)
		{
			color_sample[i].clusterID = i;
			m_clusterInfo[i].color = color_sample[i].color;
			m_clusterInfo[i].lab   = color_sample[i].lab;
			m_clusterInfo[i].entryCount = 1;
			m_clusterInfo[i++].voxelCount=0;
		}
		m_clusterCount = color_sample_num;
	}
	else
	{
		for (int i = 1; i< m_clusterCount;)
		{
			idx = (rand() / float(RAND_MAX)) * color_sample_num;
			Color4 random_cluster = color_sample[idx].color;

			if(find(i,random_cluster))
			{
	// 			findNext ++;
	// 			if (findNext == 3) //重复三次找到相同的点，说明已经没有点可以选作种子点了(原始数据分类数量少)
	// 			{
	// 				m_clusterCount = i;
	// 				break;
	// 			}
			}
			else
			{
				//findNext = 0;
				RGB2Lab3f(random_cluster.r, random_cluster.g, random_cluster.b, tmp_L, tmp_a, tmp_b);
				m_clusterInfo[i].color=random_cluster;
				m_clusterInfo[i].lab.set(tmp_L, tmp_a, tmp_b);
				m_clusterInfo[i].entryCount=0;
				m_clusterInfo[i++].voxelCount=0;
			}
		}
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //聚类
    //cout<<"color num"<<tmp<<endl;//查看有多少个有效颜色样本点
		int min_tag;
		float min_distance,tmp_distance;
		int entry_count,voxel_count;
		float sumX,sumY,sumZ;
		Vector3f sum;
		ColorSample cluster_pre;
		int variance;
		int ter_condition = 10;//调整终止条件值 控制迭代精度
		//int count_w=0;
		do
		{
			variance=0;
			//count_w++;
			//计算每个采样点的最近Cluster
			for(int j=0; j<color_sample_num; j++)
			{
				min_tag=0;
				min_distance = distance(color_sample[j], m_clusterInfo[0]);

				for(int k = 1; k < INTENSITY; k++)
				{
					tmp_distance = distance(color_sample[j], m_clusterInfo[k]);
					if(tmp_distance < min_distance)
					{
						min_tag = k;
						min_distance = tmp_distance;
					}
				}
				color_sample[j].clusterID = min_tag; //改变color_sample的Cluster ID
			}

			//recompute the cluster center;
			for(int k = 0; k < m_clusterCount; k++)
			{
				entry_count = 0;
				sumX = 0;
				sumY = 0;
				sumZ = 0;
				sum.set(0.0, 0.0, 0.0);
				for(int j = 0; j < color_sample_num; j++)
				{
					if(color_sample[j].clusterID == k)
					{
						entry_count++;
						sum += color_sample[j].lab;
					}
				}

				// 改变clusterInfo的颜色和entry_count
				cluster_pre = m_clusterInfo[k];
				if(entry_count > 0)
				{
					Color4* pColor = &(m_clusterInfo[k].color);
					Vector3f* pLab = &(m_clusterInfo[k].lab);
					*(pLab) = sum / entry_count;
					Lab3f2RGB3ub(pLab->x, pLab->y, pLab->z, pColor->r, pColor->g, pColor->b);
				}
				m_clusterInfo[k].entryCount = entry_count;

				if(variance < ter_condition)
				{
					variance += distance(cluster_pre.color, m_clusterInfo[k].color);
				}
			}
		}
		while(variance > ter_condition);  //调整
	}

    //最后计算每个聚类中含有的体素数
    for(int k = 0; k < m_clusterCount; k++)
    {
        int voxel_count = 0;
        for(int j = 0; j < color_sample_num; ++j)
        {
            if(color_sample[j].clusterID == k)
            {
                Color4 rgb = color_sample[j].color;
                voxel_count += m_histogram[rgb.r][rgb.g][rgb.b];
            }
        }
        m_clusterInfo[k].voxelCount = voxel_count;
    }

//	printf("2 ==> Color_sample[0]:%d,%d,%d -> count:%d, cid:%d\n", color_sample[0].color.r, color_sample[0].color.g, color_sample[0].color.b, color_sample[0].voxelCount, color_sample[0].clusterID);
// 	////输出测试
// 	//cout<<ter_dondition<<' '<<count_w<<endl;
// 	int non_zero = 0;
// 	for(int j=0;j<INTENSITY;j++)
// 	{
// 		cout<<"(";
// 		cout<<(int)m_clusterInfo[j].color.x<<" "<<(int)m_clusterInfo[j].color.y<<" "<<(int)m_clusterInfo[j].color.z;
// 		cout<<") "<<m_clusterInfo[j].entryCount<<"  "<<m_clusterInfo[j].voxelCount<<endl;
// 		if (m_clusterInfo[j].entryCount != 0)
// 		{
// 			non_zero ++;
// 		}
// 	}
// 	printf("Non-zero Cluster:%d\n", non_zero);
// 	cout<<endl;
}

int ClusteredColorTF::find(int n, Color4 cluster)
{
    for (int i = 0; i < n; ++i)
    {
        if (m_clusterInfo[i].color == cluster)
            return 1;
    }
    return 0;
}

void ClusteredColorTF::sortCluster()
{
    //////////////////////////////////////////////////////////////////////////
    // 生成hsv颜色，用于颜色排序
//     for (int i = 0; i < m_clusterCount; ++i)
//     {
//         RGB2HSL(m_clusterInfo[i].color.r, m_clusterInfo[i].color.g, m_clusterInfo[i].color.b,
//                 m_clusterInfo[i].hsv.x, m_clusterInfo[i].hsv.y, m_clusterInfo[i].hsv.z);
//     }
    //printf("hsv color generated!\n");

    //////////////////////////////////////////////////////////////////////////
    ColorSample clusterInfoCopy[INTENSITY]; //备份当前的聚类信息
    int minPath[INTENSITY];
    int idxMap[INTENSITY];
    memset(idxMap, 0, INTENSITY * sizeof(int));
    for (int i = 0; i < m_clusterCount; ++i)
    {
        clusterInfoCopy[i] = m_clusterInfo[i];
        idxMap[i] = i;
    }

    //////////////////////////////////////////////////////////////////////////
    // 排序策略
    m_sorter.sort(m_clusterInfo, minPath, idxMap, m_clusterCount);
//     printf("idxMap[0]:%d\n", idxMap[0]);
//     printf("minPath[0]:%d minPath[%d]:%d\n", minPath[0], idxMap[0], minPath[idxMap[0]]);
    //////////////////////////////////////////////////////////////////////////
    //生成颜色到Cluster ID的映射
    std::map<unsigned int, unsigned char>::iterator it;
    for (int i = 0; i < color_sample_num; ++i)
    {
//		ccmap.insert(std::make_pair(color2uint(color_sample[i].color), color_sample[i].clusterID));
        ccmap.insert(std::make_pair(color2uint(color_sample[i].color), idxMap[color_sample[i].clusterID]));
    }
// 	//根据排序结果调整Cluster顺序，并更新颜色到Cluster ID的映射
    for (int i = 0; i < m_clusterCount; ++i)
    {
		if (idxMap[i] != i)
		{
			m_clusterInfo[idxMap[i]] = clusterInfoCopy[i];
		}
// 		m_clusterInfo[i] = tmpClusterInfo[minPath[i]];
// 		for(int j = 0; j < color_sample_num; j++)
// 		{
// 			if (color_sample[j].clusterID == minPath[i])
// 			{
// 				it = ccmap.find(color2uint(color_sample[j].color));
// 				if (it != ccmap.end())
// 				{
// 					it->second = i;
// 				}
// 				else
// 					printf("Error:[%d,%d,%d]\n", color_sample[j].color.r, color_sample[j].color.g, color_sample[j].color.b);
// 			}
// 		}
    }
}

void ClusteredColorTF::genClusterVolume()
{
    //生成体数据对应的Cluster体，每个体素是原数据体素对应的分类ID
    unsigned int color;
    size_t entry_count[INTENSITY];
    memset(entry_count, 0, sizeof(int) * INTENSITY);
    memset(m_clusterVolume, 255, 256 * 256 * 256);

    std::map<unsigned int, unsigned char>::iterator it;
// 	for (int i = 0; i < 256; i++)
// 	{
// 		for (int j = 0; j < 256; j++)
// 		{
// 			for (int k = 0; k < 256; k++)
// 			{
// 				color = rgba2uint(i,j,k,0);
// 				it = ccmap.find(color);
// 				if (it == ccmap.end())
// 					m_clusterVolume[i][j][k] = 255;
// 				else
// 					m_clusterVolume[i][j][k] = it->second;
// 				entry_count[m_clusterVolume[i][j][k]] ++;
// 			}
// 		}
// 	}
    //////////////////////////////////////////////////////////////////////////
    Color4 c;
    for (int i = 0; i < color_sample_num; ++i)
    {
        c = color_sample[i].color;
        it = ccmap.find(color2uint(c));
        if (it == ccmap.end())
        {
            printf("Error Gen:[%d,%d,%d]\n", c.r, c.g, c.b);
            //m_clusterVolume[c.r][c.g][c.b] = 0;
        }
        else
        {
            m_clusterVolume[c.r][c.g][c.b] = it->second;
        }
        //m_clusterVolume[c.r][c.g][c.b] = color_sample[i].clusterID;
    }

    //////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
    // 设置直方图的值
    int count = 0;
    unsigned char tmp_l, tmp_a, tmp_b;
    for(int i = 0; i < m_clusterCount; ++i)
    {
		//RGB2Lab3ub(m_clusterInfo[i].color.r, m_clusterInfo[i].color.g, m_clusterInfo[i].color.b, tmp_l, tmp_a, tmp_b);
		//m_clusterHistogram->setValue(i, entry_count[i]);	//直方图显示entry数量
        //m_clusterHistogram->setValue(i, tmp_l);			//直方图显示亮度
		m_clusterHistogram->setValue(i, m_clusterInfo[i].voxelCount);//直方图显示数据量
    }
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
    // 输出测试
	// 统计聚类直方图(属于某个Cluster的颜色数)
// 	for (int i = 0; i < 256; i++)
// 		for (int j = 0; j < 256; j++)
// 			for (int k = 0; k < 256; k++)
// 				entry_count[m_clusterVolume[i][j][k]] ++;
// 	for(int i = 0; i < INTENSITY; ++i)
// 	{
// 		if (entry_count[i] != 0)
// 		{
// 			printf("%d %u/%u voxel=%u\n", i, m_clusterInfo[i].entryCount, entry_count[i], m_clusterInfo[i].voxelCount);
// 			count ++;
// 		}
// 	}
// 	printf("Non-zero Cluster After Mapping:%d\n", count);
	//////////////////////////////////////////////////////////////////////////
}

void ClusteredColorTF::updateTFColor()
{
	memset(m_tf, 0, INTENSITY * ELEMENT);
    for(int i = 0; i < m_clusterCount; ++i)
    {
        Color4 c = m_clusterInfo[i].color;
        m_tf[i * ELEMENT + 0] = c.r;
        m_tf[i * ELEMENT + 1] = c.g;
        m_tf[i * ELEMENT + 2] = c.b;
    }
}

void ClusteredColorTF::convertRGB2Lab( const unsigned char* src, unsigned char* dest, int dim_x, int dim_y, int dim_z )
{
    int len = dim_x * dim_y * dim_z;
    for (int i = 0; i < len; i += 3)
    {
        RGB2Lab3ub(src[i], src[i+1], src[i+2], dest[i], dest[i+1], dest[i+2]);
    }
}

float ClusteredColorTF::distance( const Color4& c1, const Color4& c2 )
{
    // 采用Lab颜色空间计算两个颜色的距离
    float L1, a1, b1, L2, a2, b2;
    RGB2Lab3f(c1.x, c1.y, c1.z, L1, a1, b1);
    RGB2Lab3f(c2.x, c2.y, c2.z, L2, a2, b2);
    return ::distance(L1, a1, b1, L2, a2, b2);

// 			MinDistance = distance(color_sample[j].color.x, color_sample[j].color.y, color_sample[j].color.z
// 								 , m_clusterInfo[0].color.x, m_clusterInfo[0].color.y, m_clusterInfo[0].color.z);
// 			MinDistance = (color_sample[j].color.x-m_clusterInfo->color.x)*(color_sample[j].color.x-m_clusterInfo->color.x)+
// 				(color_sample[j].color.y-m_clusterInfo->color.y)*(color_sample[j].color.y-m_clusterInfo->color.y)+
// 				(color_sample[j].color.z-m_clusterInfo->color.z)*(color_sample[j].color.z-m_clusterInfo->color.z);
}

float ClusteredColorTF::distance(const ColorSample& s, const ColorSample& c)
{
    return ::distance(s.lab.x, s.lab.y, s.lab.z, c.lab.x, c.lab.y, c.lab.z);
}

IntensityHistogram* ClusteredColorTF::getClusterHistogram()
{
    return m_clusterHistogram;
}

void ClusteredColorTF::getRange( int cidx, int& start, int& span )
{

}

const ColorSample* ClusteredColorTF::getClusterInfo()
{
    return m_clusterInfo;
}

void ClusteredColorTF::sortNearestTo( int cidx, int& sidx, int& start, int& span)
{
//	printf("[%d]->[%d](%d,%d,%d)\n", cidx, m_idxMap[cidx], m_clusterInfo[m_idxMap[cidx]].color.r, m_clusterInfo[m_idxMap[cidx]].color.g, m_clusterInfo[m_idxMap[cidx]].color.b);
    m_sorter.sortNearestTo(cidx, m_clusterInfo, m_idxMap, m_sortMask, sidx, start, span, 5.0f);
//	sidx = m_idxMap[cidx];
//	span = 0;
// 	// 找到区段的起始位置
// 	for(int i = 0; i < INTENSITY; ++i)
// 	{
// 		if (m_sortMask[i] == sidx)
// 		{
// 			start = i;
// 			++span;
// 			break;
// 		}
// 	}
// 	// 计算区段的宽度
// 	for (int i = sidx + 1; i < INTENSITY; ++i)
// 	{
// 		if (m_sortMask[i] == sidx)
// 		{
// 			++span;
// 		}
// 		else
// 		{
// 			break;
// 		}
// 	}
}

const int* ClusteredColorTF::getIndexMap()
{
    return m_idxMap;
}

int ClusteredColorTF::getClusterCount() const
{
	return m_clusterCount;
}
