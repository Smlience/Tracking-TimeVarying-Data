#pragma once

#include "render/VolumeRender.h"

class VolumeLightingRender2DTF_pos : public VolumeRender
{
	Q_OBJECT
public:
	VolumeLightingRender2DTF_pos(QWidget *parent = NULL);
	~VolumeLightingRender2DTF_pos(void);
	virtual void initTextures();
	virtual void paintGL();
	virtual void initShader();
	float ka() {return m_ka;};
	float kd() {return m_kd;};
	float ks() {return m_ks;};
	float shininess() {return m_shininess;};
	Vector3f lightPosition() {return m_lightPos;};

	public slots:
		void setKa(double v);
		void setKs(double v);
		void setKd(double v);
		void setShininess(double v);
		void setLightPos(Vector3d pos);
		virtual void onDataLoaded(Volume* v);
		void set2DTF(unsigned char* data, Vector2i dim);
		void setV1V2(const unsigned char *v1, std::pair<int,int> v1Range, const unsigned char*v2, std::pair<int,int> v2Range, Vector3i dim);

protected:
	float m_ka;
	float m_ks;
	float m_kd;
	float m_shininess;
	Vector3f m_lightPos;
	Texture* m_posTex;
	Texture* m_texV2;
	Texture* m_texV1;
	unsigned char* m_pos;
	unsigned char* m_2dtf;
	//float* m_gradient;
};
