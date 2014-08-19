#include <algorithm>
#include "TF1D.h"

TF1D::TF1D()
{
	pData = new unsigned char[INTENSITY * ELEMENT];
	resetTF();
}

TF1D::~TF1D()
{
	delete[] pData;
}

void TF1D::resetTF()
{
	int index;
	for(int i = 0; i < INTENSITY; ++i)
	{
		index = i * ELEMENT;
		pData[index] = pData[index+1] = pData[index+2] = pData[index+3] = i;
	}
}

//========================================================================
//                       Mapping Key
//========================================================================
TFMappingKey::TFMappingKey(float intensity, const Color4& color)
: m_intensity(intensity)
, m_color(color)
{}

TFMappingKey::TFMappingKey()
: m_intensity(0.f)
, m_color(Color4(0))
{}

TFMappingKey::~TFMappingKey() {
}

bool TFMappingKey::operator==(const TFMappingKey& key) {
	return (m_intensity == key.m_intensity) && (m_color== key.m_color);
}

bool TFMappingKey::operator!=(const TFMappingKey& key) {
	return !(*this == key);
}

void TFMappingKey::setColor(const Color4& color) {
	m_color = color;
}

Color4& TFMappingKey::getColor() {
	return m_color;
}

void TFMappingKey::setAlpha(float a) {
	m_color.a = static_cast<unsigned int>(a*255.f);
}

float TFMappingKey::getAlpha() {
	return m_color.a / 255.f;
}

float TFMappingKey::getIntensity() {
	return m_intensity;
}

void TFMappingKey::setIntensity(float i) {
	m_intensity = i;
}

//========================================================================
//                       Mapping Key Collection
//========================================================================

TFMappingKeyCollection::TFMappingKeyCollection()
{
	TFMappingKey* firstKey = new TFMappingKey(0.f, Color4(0));
	TFMappingKey* lastKey = new TFMappingKey(1.f, Color4(255,255,255,255));
	//初始化时，为两个
	m_keyCollection.push_back(firstKey);
	m_keyCollection.push_back(lastKey);
}

TFMappingKeyCollection::~TFMappingKeyCollection()
{
	std::vector<TFMappingKey*>::iterator iter = m_keyCollection.begin();
	for (; iter != m_keyCollection.end(); ++iter)
	{
		delete *iter;
	}
}

TFMappingKey* TFMappingKeyCollection::getKey( int i )
{
	return m_keyCollection[i];
}

bool sortFunction(TFMappingKey* a, TFMappingKey* b) {
	return a->getIntensity() < b->getIntensity();
}

void TFMappingKeyCollection::updateKey( TFMappingKey* key )
{
	std::sort(m_keyCollection.begin(), m_keyCollection.end(), sortFunction);
}

void TFMappingKeyCollection::removeKey( TFMappingKey* key )
{
	std::vector<TFMappingKey *>::iterator keyIterator = find(m_keyCollection.begin(), m_keyCollection.end(), key);
	if (keyIterator != m_keyCollection.end())
		m_keyCollection.erase(keyIterator);
	delete key;
}

void TFMappingKeyCollection::addKey( TFMappingKey* key )
{
	// insert key at appropriate location
	if (m_keyCollection.empty()) {
		m_keyCollection.push_back(key);
		return;
	}
	std::vector<TFMappingKey *>::iterator keyIterator = m_keyCollection.begin();
	// Fast-forward to the correct position
	while ((keyIterator != m_keyCollection.end()) && (key->getIntensity() > (*keyIterator)->getIntensity()))
		keyIterator++;
	m_keyCollection.insert(keyIterator, key);
}

void TFMappingKeyCollection::reset()
{
	std::vector<TFMappingKey*>::iterator iter = m_keyCollection.begin();
	for (; iter != m_keyCollection.end(); ++iter)
	{
		delete *iter;
	}
	m_keyCollection.clear();
	TFMappingKey* firstKey = new TFMappingKey(0.f, Color4(0));
	TFMappingKey* lastKey = new TFMappingKey(1.f, Color4(255,255,255,255));
	//初始化时，为两个
	m_keyCollection.push_back(firstKey);
	m_keyCollection.push_back(lastKey);
}
void TFMappingKeyCollection::cleanNoInit()
{
	std::vector<TFMappingKey*>::iterator iter = m_keyCollection.begin();
	for (; iter != m_keyCollection.end(); ++iter)
	{
		delete *iter;
	}
	m_keyCollection.clear();
}
void TFMappingKeyCollection::setKeyCollection( std::vector<TFMappingKey*> vkey )
{
	if (vkey.size() > 1)
	{
		std::vector<TFMappingKey*>::iterator iter = m_keyCollection.begin();
		for (; iter != m_keyCollection.end(); ++iter)
		{
			delete *iter;
		}
		m_keyCollection.clear();
		m_keyCollection.assign(vkey.begin(), vkey.end());
	}
}
