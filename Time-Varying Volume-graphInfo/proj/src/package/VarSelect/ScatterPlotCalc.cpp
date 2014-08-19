#include "ScatterPlotCalc.h"
#include "dm\Curvature.h"
#include "util\func.math.h"
#include "util\util.h"
#include "dm/PosCalculator.h"
#include <time.h>
#include <iostream>
using namespace std;
ScatterPlotCalc::ScatterPlotCalc() : m_data(NULL)
{}

ScatterPlotCalc::~ScatterPlotCalc()
{}

void ScatterPlotCalc::setVolume( const Volume* v )
{
	Vector3i dim = v->getDimension();
	size_t len = dim.x * dim.y * dim.z;
	unsigned char* data = v->data<unsigned char>();

	/*
	直方图按照曲率绘制
	float* result = new float[len];
	calcCurvature(result, data, dim); 
	SAFE_DELETE_ARRAY(m_data);
	m_data = new unsigned char[len];
	for(size_t i = 0; i < len; ++i)
	{
		//m_samples += QPointF(data[i], result[i] * 255);
		m_data[i] = (unsigned char)(result[i] * 255 + 0.5);
	}
	delete []result;
	*/
	unsigned char*pos_value = new unsigned char[len];
	unsigned char num_of_features;
	if(v->cal_flag==false)
		calcFeature(pos_value, v->data<unsigned char>(), dim  ,num_of_features);
	else
		pos_value = v->pos();
// 	for(size_t i = 0; i < len; ++i)
// 	{
// 		pos_value[i] = (unsigned char)(((int)pos_value[i]) * 255.0/((int)40) + 0.5);
// 	}
	//////////////////////////////////////////////////////////////////////////
	// 随机设置一些点，用于测试
	//srand(time(0));
	//for ( int i = 0; i < 10000; i++ )
	//{
	//	const double x = rand() / float(RAND_MAX) * 254.0 + 1.0;
	//	const double y = ::log( 10.0 * ( x - 1.0 ) + 1.0 ) 	* ( rand() / float(RAND_MAX) * 0.5 + 0.9 ) * 10;

	//	m_samples += QPointF( x, y );
	//}
	//emit samples(m_samples);

	//emit dataChanged(data, std::pair<int,int>(0, 255), m_data, std::pair<int,int>(0, 255), dim);
	emit dataChanged(data, std::pair<int,int>(0, 255), pos_value, std::pair<int,int>(0, 255), dim);
}
