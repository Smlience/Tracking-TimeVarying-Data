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
	 * @brief 设置绘制的模式,执行模式转换时Texture和Shader等的变换
	 * @param [in] mode int 绘制模式编号，0为使用原始RGB体数据绘制，1为使用Cluster体数据绘制
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