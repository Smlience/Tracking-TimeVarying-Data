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
//	void gradientChanged(unsigned char*, unsigned char*); //梯度，梯度模，最大梯度模
//	void sendGradMag(const Volume*);

public slots:
	virtual void loadData(const char* filename);
	virtual void onDataLoaded(const Volume* v);
	void calcGradient();
	/**
	 * @brief 设置绘制的模式,执行模式转换时Texture和Shader等的变换
	 * @param [in] mode int 绘制模式编号，0为使用原始RGB体数据绘制，1为使用对比方法设置TF绘制
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
	unsigned char* m_gm;//byte类型的梯度模
	float m_maxGradMag;	//梯度模的最大值
	int m_renderMode;
	float m_exponent;
};

#endif // ColorVolumeRender_H