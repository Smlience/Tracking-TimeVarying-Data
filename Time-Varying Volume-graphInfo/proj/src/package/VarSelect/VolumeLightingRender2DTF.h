#ifndef VolumeLightingRender2DTF_H
#define VolumeLightingRender2DTF_H

#include "render/VolumeRender.h"

class VolumeLightingRender2DTF : public VolumeRender
{
	Q_OBJECT
public:
	VolumeLightingRender2DTF(QWidget *parent = NULL);
	~VolumeLightingRender2DTF(void);
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
		void set2DTF(unsigned char* data, Vector2i dim);
		void set1DTF(unsigned char* data);

		void setV1V2(const unsigned char *v1, std::pair<int,int> v1Range, const unsigned char*v2, std::pair<int,int> v2Range, Vector3i dim);
		void setCurTime(int time);

protected:
	float m_ka;
	float m_ks;
	float m_kd;
	float m_shininess;
	Vector3f m_lightPos;
	Texture* m_gradientTex;
	Texture* m_texV2;
	Texture* m_texV1;

	Texture* m_1DtfTex;

	unsigned char* m_gradient;
	unsigned char* m_2dtf;

	GLubyte* m_1dtf;
	//
	long nTimeStart1;      //计时开始
	long nTimeStart2;      //计时开始
	long nTimeStop1;       //计时结束
	long nTimeStop2;       //计时结束
};

#endif // VolumeLightingRender2DTF_H