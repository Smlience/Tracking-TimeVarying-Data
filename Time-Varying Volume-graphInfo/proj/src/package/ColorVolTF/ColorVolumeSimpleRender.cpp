#include "ColorVolumeSimpleRender.h"
#include <minmax.h>
#include <QKeyEvent>
#include "dm\GradientCalculator.h"

ColorVolumeSimpleRender::ColorVolumeSimpleRender( QWidget *parent /*= NULL*/ )
	: VolumeRender(parent), m_gradient(0), m_gm(0), m_renderMode(1), m_gradientTex(0), m_gradMagTex(0), m_exponent(0.7)
{
	connect(this, SIGNAL(dataLoaded(const Volume*)), this, SLOT(onDataLoaded(const Volume*)));
}

ColorVolumeSimpleRender::~ColorVolumeSimpleRender( void )
{
	SAFE_DELETE(m_gradientTex);
	SAFE_DELETE(m_gradMagTex);
	SAFE_DELETE_ARRAY(m_gradient);
	SAFE_DELETE_ARRAY(m_gm);
	SAFE_DELETE(m_shaderComp);
}

void ColorVolumeSimpleRender::paintGL()
{
	if (!m_volume.loaded())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	m_fbo->Bind();
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

	if (m_renderMode == 0)
	{
		render();
	}
	else if (m_renderMode == 1)
	{
		renderCompare_CDGM();
	}
	//////////////////////////////////////////////////////////////////////////
	glDisable(GL_DEPTH_TEST);
	m_camera.setOrthoProjection();
	glMultMatrixd(m_camera.getRotateMatrix());
	drawCoord();
	m_camera.setOrthoProjection();
	m_camera.drawArc();
	glEnable(GL_DEPTH_TEST);
}

void ColorVolumeSimpleRender::initTextures()
{
	m_volTex = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE, Texture::NEAREST, false);
	m_volTex->setWrapping(Texture::CLAMP);
	m_startPosTex = new Texture(Vector3i(width(),height(),1), GL_RGB, GL_RGB32F_ARB, GL_FLOAT, Texture::NEAREST, true);
	m_startPosTex->setWrapping(Texture::CLAMP);
	m_startPosTex->uploadTexture();
	m_endPosTex = new Texture(Vector3i(width(),height(),1), GL_RGB, GL_RGB32F_ARB, GL_FLOAT, Texture::NEAREST, true);
	m_endPosTex->setWrapping(Texture::CLAMP);
	m_endPosTex->uploadTexture();
	m_depthTex = new Texture(Vector3i(width(),height(),1), GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, Texture::LINEAR, true);
	m_depthTex->uploadTexture();
	m_gradientTex = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE, Texture::NEAREST, false);
	m_gradientTex->setWrapping(Texture::CLAMP);
	m_gradMagTex = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, Texture::LINEAR, false);
	m_gradMagTex->setWrapping(Texture::CLAMP);
}

void ColorVolumeSimpleRender::initShader()
{
	m_shaderPosition = new ShaderProgram;
	m_shaderRaycast = new ShaderProgram;
	m_shaderPosition->loadShader("", "shader/rayposition.frag");
	m_shaderRaycast->loadShader("", "shader/color_volume_simple_raycast.frag");

	m_shaderComp = new ShaderProgram;
	m_shaderComp->loadShader("", "shader/color_volume_compare.frag");
}

void ColorVolumeSimpleRender::loadData( const char* filename )
{
	m_volume.loadVolume(filename);
	m_bboxDim = m_volume.getNormalizedDim();
	m_camera.setSceneBBox(Vector3d(0, 0, 0), Vector3d(m_bboxDim.x, m_bboxDim.y, m_bboxDim.z));
	Vector3i dim = m_volume.getDimension();
	m_step = 0.5 / max(dim.x, max(dim.y, dim.z));

	m_volTex->setDimensions(dim);
	m_volTex->setPixelData(m_volume.getVolume());
	m_volTex->uploadTexture();

	emit dataLoaded(&m_volume);
}

void ColorVolumeSimpleRender::onDataLoaded( const Volume* v )
{
	// do nothing
	calcGradient();
}

void ColorVolumeSimpleRender::calcGradient()
{
	Vector3i dim = m_volume.getDimension();
	int len = dim.x * dim.y * dim.z;
	int len3 = len * 3;

	//////////////////////////////////////////////////////////////////////////
	// 初始化用来存梯度、梯度模的数组
	SAFE_DELETE_ARRAY(m_gradient);
	m_gradient = new unsigned char[len3];
	memset(m_gradient, 0, sizeof(unsigned char) * len3);

	SAFE_DELETE_ARRAY(m_gm);
	m_gm = new unsigned char[len];
	memset(m_gm, 0, sizeof(unsigned char) * len);
	//////////////////////////////////////////////////////////////////////////
	char fname[100];
	sprintf(fname, "%s_CDG.grad", m_volume.getFileName());
	if(!loadGradient(fname)) // 从文件加载，失败则重新计算
	{		
		float *l_g = new float[len3];
		memset(l_g, 0, sizeof(float) * len3);
		calcColorDistanceGradient(l_g, m_volume.data<unsigned char>(), dim);

		float *l_gradMag = new float[len];
		memset(l_gradMag, 0, sizeof(float) * len);
		//////////////////////////////////////////////////////////////////////////
		//计算梯度模并得到其最大值,同时将梯度转换为uint8类型
		int i;
		float *ptrf;
		unsigned char *ptruc;
		for (i =0, m_maxGradMag = 0, ptrf = l_gradMag; i < len3; i += 3, ++ptrf)
		{
			Vector3f tmp_g(l_g[i], l_g[i + 1], l_g[i + 2]);
			*ptrf = tmp_g.norm();

			if (*ptrf > DOUBLE_EPSILON)
			{
				m_gradient[i    ] = (unsigned char)((l_g[i    ] / (*ptrf) + 1.0) / 2.0 * 255 + 0.5);
				m_gradient[i + 1] = (unsigned char)((l_g[i + 1] / (*ptrf) + 1.0) / 2.0 * 255 + 0.5);
				m_gradient[i + 2] = (unsigned char)((l_g[i + 2] / (*ptrf) + 1.0) / 2.0 * 255 + 0.5);
			}
			else
			{
				m_gradient[i    ] = 128;
				m_gradient[i + 1] = 128;
				m_gradient[i + 2] = 128;
			}

			if (*ptrf > m_maxGradMag)
				m_maxGradMag = *ptrf;
		}
		SAFE_DELETE_ARRAY(l_g);

		//将梯度模归一化并转为uint8
		for (i = 0, ptrf = l_gradMag, ptruc = m_gm; i < len; ++i, ++ptrf, ++ptruc)
		{
			(*ptrf) /= m_maxGradMag;
			(*ptruc) = (unsigned char)((*ptrf) * 255 + 0.5);
		}
		SAFE_DELETE_ARRAY(l_gradMag);

		//////////////////////////////////////////////////////////////////////////
		// 保存梯度到文件
		saveGradient(fname);
	}

	m_gradientTex->setDimensions(dim);
	m_gradientTex->setPixelData((GLubyte*)(m_gradient));
	m_gradientTex->uploadTexture();

	m_gradMagTex->setDimensions(dim);
	m_gradMagTex->setPixelData((GLubyte*)(m_gm));
	m_gradMagTex->uploadTexture();
}

void ColorVolumeSimpleRender::setRenderMode( int mode )
{
	if (mode >= 0 && mode < 2)
	{
		m_renderMode = mode;
	}
	else 
	{
		printf("[ColorVolumeSimpleRender] Error render mode.\n");
	}
	update();
}

void ColorVolumeSimpleRender::saveGradient(const char* filename)
{
	FILE* f = fopen(filename, "wb+");
	if (f != NULL)
	{
		Vector3i dim = m_volume.getDimension();
		fwrite(m_gradient, sizeof(unsigned char), dim.x * dim.y * dim.z * 3, f);
		fwrite(m_gm, sizeof(unsigned char), dim.x * dim.y * dim.z, f);
		fclose(f);
	}
}

bool ColorVolumeSimpleRender::loadGradient(const char* filename)
{
	bool ret = false;
	//从文件中读取梯度
	FILE* f = fopen(filename, "rb+");
	if (f != NULL)
	{
		Vector3i dim = m_volume.getDimension();

		size_t read_size = fread(m_gradient, sizeof(unsigned char), dim.x * dim.y * dim.z * 3, f);
		if(read_size == dim.x * dim.y * dim.z * 3)
		{
			//读梯度模
			read_size = fread(m_gm, sizeof(unsigned char), dim.x * dim.y * dim.z, f);
			if(read_size = dim.x * dim.y * dim.z)
				ret = true;
			else
				printf("Gradient magnitude in file [%s] is destroyed. Recalculate required...\n", filename);
		}
		else
			printf("Gradient in file [%s] is destroyed. Recalculate required...\n", filename);

		fclose(f);
	}
	return ret;
}

void ColorVolumeSimpleRender::render()
{
	m_shaderRaycast->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderRaycast->setUniform("step", m_step);
	m_shaderRaycast->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderRaycast->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderRaycast->setUniformSampler("texVolume", GL_TEXTURE2, GL_TEXTURE_3D, m_volTex->getId());
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

void ColorVolumeSimpleRender::renderCompare_CDGM()
{
	m_shaderComp->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderComp->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderComp->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderComp->setUniformSampler("texVolume", GL_TEXTURE2, GL_TEXTURE_3D, m_volTex->getId());
	m_shaderComp->setUniformSampler("texGrad", GL_TEXTURE3, GL_TEXTURE_3D, m_gradientTex->getId());
	m_shaderComp->setUniformSampler("texGM", GL_TEXTURE4, GL_TEXTURE_3D, m_gradMagTex->getId());
	m_shaderComp->setUniform("step", m_step);
	m_shaderComp->setUniform("exponent", m_exponent);
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

	m_shaderComp->deactivate();
}

void ColorVolumeSimpleRender::setExponent(float exp)
{
	m_exponent = exp;
	update();
}

void ColorVolumeSimpleRender::keyPressEvent( QKeyEvent *e )
{
	if (e->key() == Qt::Key_Left)
	{
		if(m_exponent > 0.1) m_exponent -= 0.05;
		printf("exponent = %f\n", m_exponent);
		update();
	}
	else if (e->key() == Qt::Key_Right)
	{
		if(m_exponent < 2.0) m_exponent += 0.05;
		printf("exponent = %f\n", m_exponent);
		update();
	}
}
