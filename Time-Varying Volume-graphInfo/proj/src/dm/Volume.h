#ifndef VOLUME_H
#define VOLUME_H

#include "util/Vector3.h"
#include "dm/VolumeMeta.h"
#include "dm/PosCalculator.h"
#include "dm/GradientCalculator.h"
class Volume 
{
public:
	Volume();
	~Volume();
	// clear volume data
	void clear();

	bool loadVolume(const char* filename);
	bool loadVolume_process(const char* filename);
	bool reloadVolume(int time);
	bool reloadVolume_all(int time);
	bool loaded();

	const char* getFileName() const {return m_pFileName;}

	inline Vector3i getAlignedDim() const { return m_alignedDim; }
	inline Vector3f getNormalizedDim() const { return m_normalizedDim; }
	// Slices number
	inline Vector3i getDimension() const { return meta.dim; }
	inline Vector3f getSpacing() const { return meta.spacing; }
	inline char * getFileDirName(){return m_FileDir;}
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
	unsigned char* grad()const{
		return grad_value;
	}
	unsigned char* pos()const{
		return pos_value;
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

private:
	const char* m_pFileName;
	//const char* m_pFileDir;  //outputGradient  ´æ´¢ÎÄ¼þÄ¿Â¼
	char m_FileDir[1024];

	VolumeMeta meta;
	unsigned char *pos_value;
	unsigned char *grad_value;

	unsigned char *pos_value_all;
	unsigned char *grad_value_all;
	unsigned char *data_all;

	int num_of_timestep;
	unsigned char maxnum_of_features;
	Vector3i m_alignedDim; // xSize
	Vector3f m_normalizedDim; // xfSize the spacings of each dimension

	int m_timeStep;
	int m_volumeSize;
//	unsigned char num_of_features;
};

#endif // VOLUME_H
