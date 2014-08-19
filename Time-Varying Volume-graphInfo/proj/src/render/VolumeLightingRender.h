#ifndef VolumeLightingRender_H
#define VolumeLightingRender_H

#include "render/VolumeRender.h"

class VolumeLightingRender : public VolumeRender
{
	Q_OBJECT
public:
	VolumeLightingRender(QWidget *parent = NULL);
	~VolumeLightingRender(void);
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
	virtual void onDataLoaded(const Volume* v);

protected:
	float m_ka;
	float m_ks;
	float m_kd;
	float m_shininess;
	Vector3f m_lightPos;
 	Texture* m_gradientTex;
 	unsigned char* m_gradient;
	//float* m_gradient;
};

#endif // VolumeLightingRender_H