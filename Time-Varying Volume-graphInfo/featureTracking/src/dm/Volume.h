#ifndef VOLUME_H
#define VOLUME_H

#include "src/util/Vector3.h"
#include "src/dm/VolumeMeta.h"
#include "src/dm/PosCalculator.h"
#include "src/dm/dag.h"
#include "src/dm/GradientCalculator.h"

class Volume 
{
public:
	Volume();
	~Volume();
	// clear volume data
	void clear();
	void clearVolume();

	bool graphOutput();
	bool graphInput();


	bool loadVolume(const char* filename);
	bool loadVolume_process();
	bool reloadVolume(int time);

	bool loaded();
	void generateProbs();
	bool initFeatureList();

	const char* getFileName() const {return m_pFileName;}

	inline Vector3i getAlignedDim() const { return m_alignedDim; }
	inline Vector3f getNormalizedDim() const { return m_normalizedDim; }
	// Slices number
	inline Vector3i getDimension() const { return meta.dim; }
	inline Vector3f getSpacing() const { return meta.spacing; }

	/**
	 * @brief Deprecated. Use data<DataType>() instead.
	 * @date 2013/06/14
	 */
	unsigned char* getVolume() const {
		return data<unsigned char>();
	}

	template<typename T>
	T* data() const
	{
		return static_cast<T*>(meta.data);
	};

	unsigned char* pos()const{
		return m_pFeatureIndex;
	}
	inline void setDimension(Vector3i dim)
	{
		meta.dim = dim;
	}

	inline void setData(void* d)
	{
		meta.data = d;
	};
	bool cal_flag;
	bool load_flag;
// 	unsigned char get_feature_num() 
// 	{
// 		return num_of_features;
// 	};
// 
// 	void set_feature_num(unsigned char num){
// 		num_of_features = num;
// 	}
protected:
	// get the power of 2 greater than or equal to size
	int getPow2(int size);

	// trilinear interpolation
	double intpTrilinear(unsigned char* pData, double x, double y, double z);

	// trilinear cubic bspline scalar interpolation
	double cubicIntpValue(double v0, double v1, double v2, double v3, double mu);
	double triCubicIntpValue(unsigned char* pData, double x, double y, double z);

	// nearest neighbor scalar value
	double getValue(unsigned char* pData, double x, double y, double z);

	bool outputFeature(int timestep);
	bool outputGradient(int timestep);

	bool outputNumOfFeature(unsigned char* pFeatureNumArray);

	bool calFeatureAttribute(int timenum,int num_of_features);

	
private:
	const char* m_pFileName;
	const char* m_pFileDir;  //outputGradient

	VolumeMeta meta;
	unsigned char *m_pFeatureIndex;  //单个特征编号体数据
	float *m_pGradient_tmp;  //单个特征编号体数据
	unsigned char * m_pGradient; 

	unsigned char **m_pVolume;  //全部  原始体数据

	unsigned char *data_all;

	int m_timeStep;
	unsigned char maxnum_of_features;
	Vector3i m_alignedDim; // xSize
	Vector3f m_normalizedDim; // xfSize the spacings of each dimension
	int m_volumeSize;

	//有向图 相关
	int m_FeatureSum;
	unsigned char *m_arryNumFeature;

	DAG dag;
//	unsigned char num_of_features;
};

#endif // VOLUME_H
