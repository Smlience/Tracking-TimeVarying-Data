#include "IntensityHistogram.h"
#include "dm/Volume.h"
#include <ctime>
#include <algorithm>
#define BUCKET_COUNT 256

IntensityHistogram::IntensityHistogram()
{
	m_entry = new int[BUCKET_COUNT];
	memset(m_entry, 0, BUCKET_COUNT * sizeof(int));
	m_max = 0;
	m_min = 999999;
}

IntensityHistogram::IntensityHistogram( const Volume* pVolumes )
{
	m_entry = new int[BUCKET_COUNT];
	calcHistogram(pVolumes);
}

IntensityHistogram::~IntensityHistogram()
{
	delete []m_entry;
}

void IntensityHistogram::clear()
{
	memset(m_entry, 0, BUCKET_COUNT * sizeof(int));
	m_max = 0;
	m_min = 999999;
}

void IntensityHistogram::calcHistogram( const Volume* pVolume )
{
	if (pVolume == NULL)
		return;

	clear();

	Vector3i dim = pVolume->getDimension();
	int dataLen = dim.x * dim.y * dim.z;
	int i,j,k,index;
	unsigned char sv; //scalar value
	unsigned char* pVolumeData = pVolume->data<unsigned char>();
	for(i = 0, index = 0; i < dim.z; ++i)
	{
		for(j = 0; j < dim.y; ++j)
		{
			for(k = 0; k < dim.x; ++k)
			{
				sv = pVolumeData[index];
				m_entry[sv] ++;
				index ++;
			}
		}
	}

	for (i = 0; i < 256; ++i)
	{
		if (m_entry[i] > m_max)
		{
			m_max = m_entry[i];
		}
		else if (m_entry[i] < m_min)
		{
			m_min = m_entry[i];
		}
	}
}

float IntensityHistogram::getNormalized( int i )
{
	return (static_cast<float>(m_entry[i]) / static_cast<float>(m_max));
}

float IntensityHistogram::getLogNormalized( int i )
{
	return logf(static_cast<float>(1 + m_entry[i])) / log( static_cast<float>(1+m_max));
}

int IntensityHistogram::getValue( int i )
{
	return m_entry[i];
}

void IntensityHistogram::setValue( int i, int v )
{
	m_entry[i] = v;
	if (v > m_max)
	{
		m_max = v;
	}
	else if (v < m_min)
	{
		m_min = v;
	}	
}

int* IntensityHistogram::getAllValues()
{
	return m_entry;
}
