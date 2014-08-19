#ifndef ColorVolumeLightingRender_H
#define ColorVolumeLightingRender_H

#include "render/VolumeLightingRender.h"

class ColorVolumeLightingRender : public VolumeLightingRender
{
	Q_OBJECT
public:
	ColorVolumeLightingRender(QWidget *parent = NULL);
	~ColorVolumeLightingRender(void);
	virtual void paintGL();
	virtual void updateGL();
	virtual void resizeGL(int w, int h);

	virtual void initTextures();
	virtual void initShader();

// signals:
// 	void dataLoaded(const Volume*);

public slots:
	virtual void loadData(const char* filename);
	void setClusterInfo(unsigned char* data);
	/**
	 * @brief ���û��Ƶ�ģʽ,ִ��ģʽת��ʱTexture��Shader�ȵı任
	 * @param [in] mode int ����ģʽ��ţ�0Ϊʹ��ԭʼRGB�����ݻ��ƣ�1Ϊʹ��Cluster�����ݻ���
	 * @date 2013/01/25
	 */
	void setRenderMode(int mode);

private:
	void genClusterVolume(unsigned char* data);
	void renderByClusterVolume();
	void renderByClusterVolumeTriLinear();
	void renderByRGBVolume();

private:
	Volume m_volume;
	Texture* m_clusterTex;
	Texture* m_colorVolTex;
	unsigned char* m_clusterVolume;		// Cluster Volume Data
	const unsigned char* m_clusterData;	// Cluster Data 256^3
	int m_renderMode;
	ShaderProgram* m_shaderCluster;
	ShaderProgram* m_shaderClusterTriLinear;
};

#endif // ColorVolumeLightingRender_H