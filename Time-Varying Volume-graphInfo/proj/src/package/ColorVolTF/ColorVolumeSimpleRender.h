#ifndef ColorVolumeSimpleRender_H
#define ColorVolumeSimpleRender_H

#include "render/VolumeRender.h"

class ColorVolumeSimpleRender : public VolumeRender
{
	Q_OBJECT
public:
	ColorVolumeSimpleRender(QWidget *parent = NULL);
	~ColorVolumeSimpleRender(void);
	virtual void paintGL();
	virtual void initTextures();
	virtual void initShader();

protected:
	void keyPressEvent(QKeyEvent *e);
//signals:
//	void gradientChanged(unsigned char*, unsigned char*); //�ݶȣ��ݶ�ģ������ݶ�ģ
//	void sendGradMag(const Volume*);

public slots:
	virtual void loadData(const char* filename);
	virtual void onDataLoaded(const Volume* v);
	void calcGradient();
	/**
	 * @brief ���û��Ƶ�ģʽ,ִ��ģʽת��ʱTexture��Shader�ȵı任
	 * @param [in] mode int ����ģʽ��ţ�0Ϊʹ��ԭʼRGB�����ݻ��ƣ�1Ϊʹ�öԱȷ�������TF����
	 * @date 2013/01/25
	 */
	void setRenderMode(int mode);
	void setExponent(float exp);

private:
	void render();
	void renderCompare_CDGM();
	void saveGradient( const char* filename );
	bool loadGradient(const char* filename);

private:
	ShaderProgram* m_shaderComp;
	Texture* m_gradientTex;
	Texture* m_gradMagTex;
	unsigned char* m_gradient;
	unsigned char* m_gm;//byte���͵��ݶ�ģ
	float m_maxGradMag;	//�ݶ�ģ�����ֵ
	int m_renderMode;
	float m_exponent;
};

#endif // ColorVolumeRender_H