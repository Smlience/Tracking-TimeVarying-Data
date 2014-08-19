#include "ColorVolumeLightingRender.h"
#include "imdebug/imdebuggl.h"

ColorVolumeLightingRender::ColorVolumeLightingRender(QWidget *parent)
: VolumeLightingRender(parent), m_clusterTex(0), m_clusterVolume(0), m_renderMode(2), m_shaderCluster(0), m_shaderClusterTriLinear(0)
{}

ColorVolumeLightingRender::~ColorVolumeLightingRender(void)
{
	SAFE_DELETE(m_clusterTex);
	SAFE_DELETE_ARRAY(m_clusterVolume);
	SAFE_DELETE(m_shaderCluster);
	SAFE_DELETE(m_shaderClusterTriLinear);
}

void ColorVolumeLightingRender::paintGL()
{
	if (!m_volume.loaded())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}
	m_fbo->Bind();
	//////////////////////////////////////////////////////////////////////////

	m_shaderPosition->activate();
	m_camera.setOrthoProjection();
	m_camera.setupModelMatrix();
	// front faces
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawBBox();

	// back face
	glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
	glClear(GL_COLOR_BUFFER_BIT);
	glDepthFunc(GL_GREATER);
	drawBBox();
	glDepthFunc(GL_LESS);

	// disable position fragment program and FBO
	m_shaderPosition->deactivate();
	m_fbo->IsValid();
	FramebufferObject::Disable();

// 	imdebugTexImagef(GL_TEXTURE_RECTANGLE_ARB, m_endPosTexID, GL_RGB);
// 	imdebugTexImagef(GL_TEXTURE_RECTANGLE_ARB, m_startPosTexID, GL_RGB);
	//////////////////////////////////////////////////////////////////////////
	// 根据不同的绘制模式选择不同的绘制方法
	if (m_renderMode == 0)
	{
		renderByRGBVolume();
	}
	else if (m_renderMode == 1)
	{
		renderByClusterVolume();
	}
	else if (m_renderMode == 2)
	{
		renderByClusterVolumeTriLinear();
	}
	//////////////////////////////////////////////////////////////////////////
	m_camera.setOrthoProjection();
	glDisable(GL_DEPTH_TEST);
	m_camera.drawArc();
	glEnable(GL_DEPTH_TEST);
}

void ColorVolumeLightingRender::updateGL()
{
	QGLCanvas::updateGL();
}

void ColorVolumeLightingRender::resizeGL( int w, int h )
{
	QGLCanvas::resizeGL(w, h);
	resizeTextures(w, h);
}

void ColorVolumeLightingRender::initTextures()
{
	VolumeRender::initTextures();

	m_tfTex->setFilter(Texture::NEAREST);

	m_clusterTex = new Texture((GLubyte*)0, Vector3i(256,256,256), GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, Texture::NEAREST, false);
	m_colorVolTex = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE, Texture::NEAREST, false);
	m_colorVolTex->setWrapping(Texture::CLAMP);
	m_volTex->setFilter(Texture::NEAREST);
	m_volTex->setWrapping(Texture::CLAMP);
}

void ColorVolumeLightingRender::initShader()
{
	m_shaderPosition = new ShaderProgram;
	m_shaderRaycast = new ShaderProgram;
	m_shaderCluster = new ShaderProgram;
	m_shaderClusterTriLinear = new ShaderProgram;
	m_shaderPosition->loadShader("", "shader/rayposition.frag");
	m_shaderRaycast->loadShader("", "shader/color_volume_raycast.frag");
	m_shaderCluster->loadShader("", "shader/color_cluster_volume_raycast.frag");
	m_shaderClusterTriLinear->loadShader("", "shader/color_cluster_volume_raycast_tri_linear.frag");
}

void ColorVolumeLightingRender::loadData( const char* filename )
{
	m_volume.loadVolume(filename);
	m_bboxDim = m_volume.getNormalizedDim();
	m_camera.setSceneBBox(Vector3d(0, 0, 0), Vector3d(m_bboxDim.x, m_bboxDim.y, m_bboxDim.z));
	Vector3i dim = m_volume.getDimension();
	m_step = 0.5 / max(dim.x, max(dim.y, dim.z));
	m_colorVolTex->setDimensions(dim);
	m_colorVolTex->setPixelData(m_volume.getVolume());
	m_colorVolTex->uploadTexture();
//	setRenderMode(m_renderMode);
// 	QImage img(m_volume.getVolume(), dim.x, dim.y, QImage::Format_RGB888);
// 	img.save("./data/test.png", "PNG");
	emit dataLoaded(&m_volume);
}

/**
 * ColorTFWidget的signal连接到此slot，在ColorTFCanvas中的ClusteredColorTF计算完
 * 聚类之后发出signal，将聚类结果传递至此slot
 */
void ColorVolumeLightingRender::setClusterInfo( unsigned char* data )
{
	m_clusterData = data;
	m_clusterTex->setPixelData(data);
	m_clusterTex->uploadTexture();

	genClusterVolume(data);
	m_volTex->setDimensions(m_volume.getDimension());
	m_volTex->setPixelData(m_clusterVolume);
	m_volTex->uploadTexture();
	update();
// 	//输出Cluster图像，用于测试
// 	Vector3i dim = m_volume.getDimension();
// 	unsigned char* vol = m_volume.getVolume();
// 	QImage img(dim.x, dim.y, QImage::Format_RGB32);
// 	int idx;
// 	uint v;
// 	for (int i = 0; i < dim.y; ++i)
// 	{
// 		for (int j = 0; j < dim.x; ++j)
// 		{
// 			idx = i * dim.x + j;
// 			idx *= 3;
// 			v = data[vol[idx] * 256 * 256 + vol[idx + 1] * 256 + vol[idx + 2]];
// 			v = ((((0 | v) << 8) | v) << 8) | v;
// 			img.setPixel(j, i, v);
// 		}
// 	}
// 	img.mirrored().save("testimg.png", "PNG");
}

void ColorVolumeLightingRender::setRenderMode(int mode)
{
	if (mode >= 0 && mode < 3)
	{
		m_renderMode = mode;
	}
	else
	{
		printf("[ColorVolumeLightingRender] Error render mode.\n");
	}
	update();
}

void ColorVolumeLightingRender::genClusterVolume( unsigned char* data )
{
	Vector3i dim = m_volume.getDimension();
	int len = dim.x * dim.y * dim.z;
	unsigned char* vol = m_volume.getVolume();
	int idx;

	if (m_clusterVolume == NULL)
	{
		m_clusterVolume = new unsigned char[len];
	}
	memset(m_clusterVolume, 0, len);
	for (int i = 0; i < len; ++i)
	{
		idx = i * 3;
		m_clusterVolume[i] = data[vol[idx] * 256 * 256 + vol[idx + 1] * 256 + vol[idx + 2]];
	}
}

//使用cluster volume来查询传输函数调整不透明度，使用原体数据的颜色
void ColorVolumeLightingRender::renderByRGBVolume()
{
	m_shaderRaycast->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderRaycast->setUniform("step", m_step);
	m_shaderRaycast->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderRaycast->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderRaycast->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderRaycast->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_colorVolTex->getId());
	m_shaderRaycast->setUniformSampler("texCluster", GL_TEXTURE4, GL_TEXTURE_3D, m_clusterTex->getId());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(0, 0);
	glTexCoord2f(0.0, height());
	glVertex2f(0, height());  
	glTexCoord2f(width(), height());
	glVertex2f(width(), height());
	glTexCoord2f(width(), 0.0);
	glVertex2f(width(), 0);
	glEnd();

	m_shaderRaycast->deactivate();
}

//使用cluster volume来查询传输函数会得对应的颜色和不透明度，不使用原始数据
void ColorVolumeLightingRender::renderByClusterVolume()
{
	m_shaderCluster->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderCluster->setUniform("step", m_step);
	m_shaderCluster->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderCluster->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderCluster->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderCluster->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_volTex->getId());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(0, 0);
	glTexCoord2f(0.0, height());
	glVertex2f(0, height());  
	glTexCoord2f(width(), height());
	glVertex2f(width(), height());
	glTexCoord2f(width(), 0.0);
	glVertex2f(width(), 0);
	glEnd();
	m_shaderCluster->deactivate();
}

//使用原始数据中采样点周围的8个点，获取其对应的cluster volume中的cluster id，
//然后查询传输函数获得8个颜色和不透明度，对8个颜色和不透明度进行3线性插值得到采样点颜色
void ColorVolumeLightingRender::renderByClusterVolumeTriLinear()
{
	m_shaderClusterTriLinear->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderClusterTriLinear->setUniform("step", m_step);
	m_shaderClusterTriLinear->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderClusterTriLinear->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderClusterTriLinear->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderClusterTriLinear->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_colorVolTex->getId());
	m_shaderClusterTriLinear->setUniformSampler("texCluster", GL_TEXTURE4, GL_TEXTURE_3D, m_clusterTex->getId());
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2i(0, 0);
	glTexCoord2f(0.0, height());
	glVertex2f(0, height());  
	glTexCoord2f(width(), height());
	glVertex2f(width(), height());
	glTexCoord2f(width(), 0.0);
	glVertex2f(width(), 0);
	glEnd();

	m_shaderClusterTriLinear->deactivate();
}
