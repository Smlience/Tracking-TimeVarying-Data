#ifndef TF1D_H
#define TF1D_H

#include <vector>
#include "util/Vector4.h"

class TF1D
{
public:
	enum TransfuncSize{
		INTENSITY = 256,
		ELEMENT = 4
	};

	TF1D();
	~TF1D();

	unsigned char *getTFData(){return pData;}

protected:
	void resetTF();

private:
	unsigned char *pData;
};

//========================================================================
//                       Mapping Key
//========================================================================
class TFMappingKey
{
public:
	TFMappingKey(float intensity, const Color4& color);
	TFMappingKey();
	~TFMappingKey();

	bool operator==(const TFMappingKey& key);
	bool operator!=(const TFMappingKey& key);

	void setColor(const Color4& color);
	Color4& getColor();

	void setAlpha(float a);
	float getAlpha();

	float getIntensity();
	void setIntensity(float intensity);

protected:
	float m_intensity;
	Color4 m_color;
};

//========================================================================
//                       Mapping Key Collection
//========================================================================

class TFMappingKeyCollection
{
public:
	TFMappingKeyCollection();
	~TFMappingKeyCollection();
	
	size_t getNumKeys(){return m_keyCollection.size();}
	TFMappingKey* getKey(int i);
	void updateKey(TFMappingKey* key);
	void removeKey(TFMappingKey* key);
	void addKey(TFMappingKey* key);
	std::vector<TFMappingKey*> getKeyCollection() {return m_keyCollection;}
	void setKeyCollection(std::vector<TFMappingKey*> vkey);
	void reset();
	void cleanNoInit();
private:
	std::vector<TFMappingKey*> m_keyCollection;
};

#endif // TF1D_H