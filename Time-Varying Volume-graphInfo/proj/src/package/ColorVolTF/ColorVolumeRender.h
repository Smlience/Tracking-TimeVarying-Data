#ifndef ColorVolumeRender_H
#define ColorVolumeRender_H

#include "render/VolumeLightingRender.h"

class ColorVolumeRender : public VolumeLightingRender
{
	Q_OBJECT
public:
	ColorVolumeRender(QWidget *parent = NULL);
	~ColorVolumeRender(void);
	virtual void paintGL();
	virtual void initTextures();
	virtual void initShader();

signals:
	void gradientChanged(unsigned char*, unsigned char*); //梯度，梯度模，最大梯度模
	void sendGradMag(const Volume*);

public slots:
	virtual void loadData(const char* filename);
	virtual void onDataLoaded(const Volume* v);
	void calcGradient();
	void setClusterInfo(unsigned char* data);
	void setGMTF(unsigned char* data);
	/**
	 * @brief 设置绘制的模式,执行模式转换时Texture和Shader等的变换
	 * @param [in] mode int 绘制模式编号，0为使用原始RGB体数据绘制，1为使用Cluster体数据绘制
	 * @date 2013/01/25
	 */
	void setRenderMode(int mode);

	void setGradientMode(int mode);

private:
	void genClusterVolume(unsigned char* data);
	void renderByClusterVolume();
	void renderByClusterVolumeTriLinear();
	void renderByRGBVolume();
	void renderByRGBVolumeLighting();
	void renderByRGBVolumeGradientOpacityLighting();
	//////////////////////////////////////////////////////////////////////////
	// For test
	void keyPressEvent(QKeyEvent *e);
	bool loadGradient(const char* filename);
	void saveGradient(const char* filename);
	void saveGradMag(const char* filename);
	void saveGradientUC(const char* filename);
	void loadGradMag(const char* filename);
	void loadGradientUC(const char* filename);
	//////////////////////////////////////////////////////////////////////////

private:
	Texture* m_clusterTex;
	Texture* m_clusterVolTex;
	Texture* m_gradMagTex;
	Texture* m_gmTFTex;
	unsigned char* m_clusterVolume;		// Cluster Volume Data
	const unsigned char* m_clusterData;	// Cluster Data 256^3
	int m_renderMode;
	enum GradientMode{
		RCMG = 0,
		LabCD = 1,
		IntensityCD = 2,
		CDG = 3, // ColorDistanceGradient
		Sobel = 4
	} m_gradientMode;
	ShaderProgram* m_shaderCluster;
	ShaderProgram* m_shaderClusterTriLinear;
	ShaderProgram* m_shaderLighting;
	ShaderProgram* m_shaderGradOpacityLighting;
	Volume m_gmVol;
	unsigned char* m_gm;//byte类型的梯度模
	//unsigned char* m_g;
// 	float *m_g;			//float类型的梯度
// 	float *m_gradMag;	//float类型的梯度模
	float m_maxGradMag;	//梯度模的最大值
};

#endif // ColorVolumeRender_H