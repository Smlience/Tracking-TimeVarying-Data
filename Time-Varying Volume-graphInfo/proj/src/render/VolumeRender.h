#ifndef VOLUMERENDER_H
#define VOLUMERENDER_H

#include "ui/QGLCanvas.h"
#include "dm/Volume.h"
#include "util/Texture.h"
#include "util/ShaderProgram.h"
#include "util/framebufferObject.h"


class VolumeRender : public QGLCanvas
{
	typedef void (*GLPostInitProc)(VolumeRender*);

	Q_OBJECT
public:
	VolumeRender(QWidget *parent = NULL);
	~VolumeRender(void);
	virtual void initializeGL();
	virtual void paintGL();
	virtual void updateGL();
	virtual void resizeGL(int w, int h);

	virtual void initTextures();
	virtual void resizeTextures(int w, int h);
	virtual void initShader();
	virtual void initFBO();

signals:
	void dataLoaded(const Volume*);
	void dataLoaded();
	void sendFileDirName( char* fileDirName );
public slots:
	virtual void loadData(const char* filename);
	virtual void onDataLoaded(const Volume* v);
	void set1DTF(unsigned char* data);

protected:
	void drawBBox();
	void drawPosition();

protected:
	Volume m_volume;
	ShaderProgram* m_shaderRaycast;
	ShaderProgram* m_shaderPosition;
	FramebufferObject* m_fbo;
	Texture* m_volTex;
	Texture* m_tfTex;
	Texture* m_startPosTex;
	Texture* m_endPosTex;
	Texture* m_depthTex;
	Vector3f m_bboxDim;
	float m_step;
	GLPostInitProc m_postInitProc;
};

#endif // VOLUMERENDER_H