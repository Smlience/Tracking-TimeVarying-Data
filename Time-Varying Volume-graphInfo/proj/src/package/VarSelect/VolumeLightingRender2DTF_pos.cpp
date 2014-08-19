#include "VolumeLightingRender2DTF_pos.h"
#include "util/Functions.h"
#include "imdebug/imdebuggl.h"
#include "dm/PosCalculator.h"
#include <iostream>
using namespace std;
VolumeLightingRender2DTF_pos::VolumeLightingRender2DTF_pos(QWidget *parent)
	: VolumeRender(parent), m_ka(0.3), m_ks(0.7), m_kd(1.0), m_shininess(32)
	, m_lightPos(-1.0, 1.0, 1.0), m_pos(NULL), m_2dtf(NULL)
{
	connect(this, SIGNAL(dataLoaded(const Volume*)), this, SLOT(onDataLoaded(const Volume*)));
}

VolumeLightingRender2DTF_pos::~VolumeLightingRender2DTF_pos(void)
{
	SAFE_DELETE(m_posTex);
	SAFE_DELETE_ARRAY(m_pos);
}

void VolumeLightingRender2DTF_pos::initTextures()
{
	m_volTex = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, Texture::LINEAR, false);
	m_volTex->setWrapping(Texture::CLAMP);
	m_startPosTex = new Texture(Vector3i(width(),height(),1), GL_RGB, GL_RGB32F_ARB, GL_FLOAT, Texture::NEAREST, true);
	m_startPosTex->setWrapping(Texture::CLAMP);
	m_startPosTex->uploadTexture();
	m_endPosTex = new Texture(Vector3i(width(),height(),1), GL_RGB, GL_RGB32F_ARB, GL_FLOAT, Texture::NEAREST, true);
	m_endPosTex->setWrapping(Texture::CLAMP);
	m_endPosTex->uploadTexture();
	m_depthTex = new Texture(Vector3i(width(),height(),1), GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, Texture::LINEAR, true);
	m_depthTex->uploadTexture();

	//m_tfTex = new Texture((GLubyte*)0, Vector3i(256, 256, 1), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, Texture::NEAREST, true);
	m_tfTex = new Texture(Vector3i(256, 256, 1), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, Texture::NEAREST, true);
	m_tfTex->setWrapping(Texture::CLAMP);

	m_posTex = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_RGB, GL_RGB8, GL_UNSIGNED_BYTE, Texture::LINEAR, false);
	m_posTex->setWrapping(Texture::CLAMP);

	m_texV1 = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, Texture::LINEAR, false);
	m_texV1->setWrapping(Texture::CLAMP);
	m_texV2 = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, Texture::LINEAR, false);
	m_texV2->setWrapping(Texture::CLAMP);
}

void VolumeLightingRender2DTF_pos::paintGL()
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

	//把TF的纹理保存出来用于测试
	// 	GLubyte* d = m_tfTex->downloadTextureToBuffer();
	// 	QImage* timg = new QImage(d, 256, 256, 1024, QImage::Format_ARGB32);
	// 	timg->save("t.png","PNG");
	// 	delete timg;
	// 	delete d;
	//	imdebugTexImage(GL_TEXTURE_RECTANGLE_ARB, m_tfTex->getId(), GL_RGBA);
	//	imdebugTexImagef(GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId(), GL_RGB);
	//////////////////////////////////////////////////////////////////////////
	m_shaderRaycast->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderRaycast->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderRaycast->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderRaycast->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_RECTANGLE_ARB, m_tfTex->getId());
	m_shaderRaycast->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_volTex->getId());
	m_shaderRaycast->setUniformSampler("texGradient", GL_TEXTURE4, GL_TEXTURE_3D, m_posTex->getId());
	m_shaderRaycast->setUniformSampler("texV1", GL_TEXTURE5, GL_TEXTURE_3D, m_texV1->getId());
	m_shaderRaycast->setUniformSampler("texV2", GL_TEXTURE6, GL_TEXTURE_3D, m_texV2->getId());
	m_shaderRaycast->setUniform("step", m_step);
	m_shaderRaycast->setUniform("ka", m_ka);
	m_shaderRaycast->setUniform("kd", m_kd);
	m_shaderRaycast->setUniform("ks", m_ks);
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

void VolumeLightingRender2DTF_pos::initShader()
{
	m_shaderPosition = new ShaderProgram;
	m_shaderRaycast = new ShaderProgram;
	m_shaderPosition->loadShader("", "shader/rayposition.frag");
	cout<<"shading?"<<endl;
	m_shaderRaycast->loadShader("", "shader/raycast_lightling_2DTF_pos.frag");
}

void VolumeLightingRender2DTF_pos::setKa( double v )
{
	m_ka = v;
	update();
}

void VolumeLightingRender2DTF_pos::setKs( double v )
{
	m_ks = v;
	update();
}

void VolumeLightingRender2DTF_pos::setKd( double v )
{
	m_kd = v;
	update();
}

void VolumeLightingRender2DTF_pos::setShininess( double v )
{
	m_shininess = v;
	update();
}

void VolumeLightingRender2DTF_pos::setLightPos( Vector3d pos )
{
	m_lightPos.set(pos.x, pos.y, pos.z);
	update();
}





void VolumeLightingRender2DTF_pos::onDataLoaded( Volume* v )
{
	cout <<"loading data"<<endl;
	Vector3i dim = v->getDimension();
	int len = dim.x * dim.y * dim.z;
	SAFE_DELETE_ARRAY(m_pos);
	float norm_;

	// 直接计算出梯度体
	m_pos = new unsigned char[len];
	memset(m_pos, 0, len * sizeof(unsigned char));
	unsigned char* pos_value = new unsigned char[len];
	cout<<"data load 0"<<endl<<endl;
	unsigned char num_of_features;

	calcFeature(pos_value, v->data<unsigned char>(), dim  ,num_of_features);
	//v->set_feature_num(num_of_features);
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
	//	gradientF2UC(pos_value, m_pos, dim);
	cout<<"data load 1"<<endl<<endl;
	posF2UC(pos_value, m_pos, dim, num_of_features);
	delete []pos_value;
	cout<<"data load 2"<<endl<<endl;
	//////////////////////////////////////////////////////////////////////////
	m_posTex->setDimensions(dim);
	m_posTex->setPixelData((GLubyte*)(m_pos));
	m_posTex->uploadTexture();
	cout<<"data load finished"<<endl<<endl;
}

void VolumeLightingRender2DTF_pos::set2DTF(unsigned char* data, Vector2i dim)
{
	Vector3i d = m_tfTex->getDimensions();
	if (dim.x != d.x || dim.y != d.y)
	{
		m_tfTex->setDimensions(Vector3i(dim.x, dim.y, 1));
	}
	// 	QImage m(data, dim.x, dim.y, 4 * dim.x, QImage::Format_ARGB32);
	// 	m.save("b.png", "PNG");
	m_tfTex->setPixelData(data);
	m_tfTex->uploadTexture();
	update();
}

void VolumeLightingRender2DTF_pos::setV1V2(const unsigned char *v1, std::pair<int,int> v1Range, const unsigned char*v2, std::pair<int,int> v2Range, Vector3i dim)
{
	//cout <<"setV1V2"<<endl;
	m_texV1->setDimensions(dim);
	m_texV1->setPixelData((GLubyte*)(v1));
	m_texV1->uploadTexture();

	m_texV2->setDimensions(dim);
	m_texV2->setPixelData((GLubyte*)(v2));
	m_texV2->uploadTexture();
}
