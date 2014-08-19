#include "ColorVolumeLightingRender.h"
#include "util/Functions.h"
#include "imdebug/imdebuggl.h"
#include "dm/GradientCalculator.h"

ColorVolumeLightingRender::ColorVolumeLightingRender(QWidget *parent)
: VolumeLightingRender(parent)
{}

ColorVolumeLightingRender::~ColorVolumeLightingRender(void)
{}

void ColorVolumeLightingRender::initTextures()
{
	VolumeLightingRender::initTextures();
	m_tfTex->setFilter(Texture::NEAREST);

	m_volTex->setFormat(GL_RGB);
	m_volTex->setInternalFormat(GL_RGB8);
	m_volTex->setFilter(Texture::NEAREST);
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

// 	imdebugTexImagef(GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId(), GL_RGB);
// 	imdebugTexImagef(GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId(), GL_RGB);
	//////////////////////////////////////////////////////////////////////////
	m_shaderRaycast->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderRaycast->setUniform("step", m_step);
	m_shaderRaycast->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderRaycast->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderRaycast->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderRaycast->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_volTex->getId());
	m_shaderRaycast->setUniformSampler("texGradient", GL_TEXTURE4, GL_TEXTURE_3D, m_gradientTex->getId());
	m_shaderRaycast->setUniform("ka", m_ka);
	m_shaderRaycast->setUniform("ks", m_ks);
	m_shaderRaycast->setUniform("kd", m_kd);
	m_shaderRaycast->setUniform("shininess", m_shininess);

	//获取光线方向
// 	GLfloat modelview[16]; 
// 	glGetFloatv(GL_MODELVIEW_MATRIX, modelview); 
// 	m_eyePos.set(modelview[2], modelview[6], modelview[10]);
// 	m_shaderRaycast->setUniform("lightPos", m_lightPos.x, m_lightPos.y, m_lightPos.z);
// 	m_shaderRaycast->setUniform("eyePos", m_eyePos.x, m_eyePos.y, m_eyePos.z);
	Vector3f viewDir = m_camera.viewDirection();
	m_shaderRaycast->setUniform("lightPos", viewDir.x, viewDir.y, viewDir.z);
	m_shaderRaycast->setUniform("viewDir", viewDir.x, viewDir.y, viewDir.z); 

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
	//////////////////////////////////////////////////////////////////////////
	glDisable(GL_DEPTH_TEST);
	m_camera.setOrthoProjection();
	glMultMatrixd(m_camera.getRotateMatrix());
	drawCoord();
	m_camera.setOrthoProjection();
	m_camera.drawArc();
	glEnable(GL_DEPTH_TEST);
}

void ColorVolumeLightingRender::initShader()
{
	m_shaderPosition = new ShaderProgram;
	m_shaderRaycast = new ShaderProgram;
	m_shaderPosition->loadShader("", "shader/rayposition.frag");
	m_shaderRaycast->loadShader("", "shader/raycast_lighting.frag");
}

void ColorVolumeLightingRender::onDataLoaded( const Volume* v )
{
	//计算梯度模
	Vector3i dim = v->getDimension();
	int len = dim.x * dim.y * dim.z * 3;
	SAFE_DELETE_ARRAY(m_gradient);
	float norm_;

	// 直接计算出梯度体
	m_gradient = new unsigned char[len];
	memset(m_gradient, 0, len * sizeof(unsigned char));
 	float* grad = new float[len];
	calcGradientCentralDiff(grad, v->data<unsigned char>(), dim);

//////////////////////////////////////////////////////////////////////////
// 	int xy = dim.x * dim.y;
//  	int i, j, k;
//  	Vector3f gradient;
//  	unsigned char* pData = v->data<unsigned char>();
//  	// 测试
//  	for(i = 0; i < dim.z; ++i) {
//  		for(j = 0; j < dim.y; ++j)  {
//  			for(k = 0; k < dim.x; ++k) {
//  				unsigned int index_p = i * xy + j * dim.x + k;
//  				index_p *= 3;
//  				gradient = calcGradientCentralDiffAt(pData, dim, Vector3i(k, j, i));
//  				grad[index_p + 0] = gradient.x;
//  				grad[index_p + 1] = gradient.y;
//  				grad[index_p + 2] = gradient.z;
//  			}
//  		}
//  	}
 	gradientNormalize(grad, dim);
 	gradientF2UC(grad, m_gradient, dim);
	delete []grad;
//////////////////////////////////////////////////////////////////////////
	m_gradientTex->setDimensions(dim);
	m_gradientTex->setPixelData((GLubyte*)(m_gradient));
	m_gradientTex->uploadTexture();
}
