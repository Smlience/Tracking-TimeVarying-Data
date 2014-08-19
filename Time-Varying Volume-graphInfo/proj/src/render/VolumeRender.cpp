#include "VolumeRender.h"
#include "imdebug/imdebuggl.h"
#include <minmax.h>
#include <iostream>
using namespace std;


extern void loadTestData(VolumeRender* render);

VolumeRender::VolumeRender(QWidget *parent)
	: QGLCanvas(parent), m_fbo(0), m_shaderPosition(0), m_shaderRaycast(0), m_bboxDim(1.0, 1.0, 1.0)
	, m_volTex(0), m_tfTex(0), m_startPosTex(0), m_endPosTex(0), m_depthTex(0), m_postInitProc(0)
{
	m_postInitProc = &loadTestData;//???
}

VolumeRender::~VolumeRender(void)
{
	SAFE_DELETE(m_volTex);
	SAFE_DELETE(m_tfTex);
	SAFE_DELETE(m_startPosTex);
	SAFE_DELETE(m_endPosTex);
	SAFE_DELETE(m_depthTex);
	SAFE_DELETE(m_shaderRaycast);
	SAFE_DELETE(m_shaderPosition);
	SAFE_DELETE(m_fbo);
}

void VolumeRender::initializeGL()
{
	QGLCanvas::initializeGL();
	initTextures();
	initShader();
	initFBO();
	m_camera.setSceneBBox(Vector3d(0.0, 0.0, 0.0), Vector3d(1.0, 1.0, 1.0));

	if (m_postInitProc != NULL)
	{
		m_postInitProc(this);
	}
}

void VolumeRender::paintGL()
{
	if (!m_volume.loaded())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// 		Render::paint();
		// 		glTranslated(-0.5, -0.5, -0.5);
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
	m_shaderRaycast->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderRaycast->setUniform("step", m_step);
	m_shaderRaycast->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderRaycast->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderRaycast->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderRaycast->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_volTex->getId());

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

void VolumeRender::updateGL()
{
	QGLCanvas::updateGL();
}

void VolumeRender::resizeGL( int w, int h )
{
	QGLCanvas::resizeGL(w, h);
	resizeTextures(w, h);
}

void VolumeRender::loadData( const char* filename )
{
	m_volume.loadVolume(filename);
	//cout<<"?????????????????"<<endl<<endl;
	unsigned char *data = m_volume.getVolume();

	m_bboxDim = m_volume.getNormalizedDim();
	m_camera.setSceneBBox(Vector3d(0, 0, 0), Vector3d(m_bboxDim.x, m_bboxDim.y, m_bboxDim.z));
	Vector3i dim = m_volume.getDimension();
// 	for(int i = 0;i < dim.x;i++){
// 		for(int j = 0;j < dim.y;j++){
// 			for(int k = 0;k < dim.z;k++){
// 				//cout<<(int)*(data+i*dim.x+j*dim.y+k)<<" ";
// 				if(*(data+i*dim.y*dim.z+j*dim.z+k) < THRESHHOLD_VALUE)
// 					*(data+i*dim.y*dim.z+j*dim.z+k)=0;
// 			}
// 		}
// 	}
	m_step = 0.75 / max(dim.x, max(dim.y, dim.z));

	emit dataLoaded(&m_volume);
	emit sendFileDirName(m_volume.getFileDirName());
}



void VolumeRender::initTextures()
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

	m_tfTex = new Texture(Vector3i(256,1,1), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, Texture::NEAREST, false);
	m_tfTex->setWrapping(Texture::CLAMP);
	//////////////////////////////////////////////////////////////////////////
	// For test
	// 	GLubyte* m_TF1DTable = new GLubyte[1024];
	// 	for(size_t i = 0; i < 256; ++i)
	// 		m_TF1DTable[i * 4 + 0] = m_TF1DTable[i * 4 + 1] = m_TF1DTable[i * 4 + 2] = m_TF1DTable[i * 4 + 3] = i;
	//	m_tfTex->setPixelData((GLubyte*)(m_TF1DTable));
	//	m_tfTex->uploadTexture();
}

void VolumeRender::resizeTextures(int w, int h)
{
	m_startPosTex->setDimensions(Vector3i(w, h, 1));
	m_startPosTex->uploadTexture();

	m_endPosTex->setDimensions(Vector3i(w, h, 1));
	m_endPosTex->uploadTexture();

	m_depthTex->setDimensions(Vector3i(w, h, 1));
	m_depthTex->uploadTexture();
}

void VolumeRender::initShader()
{
	m_shaderPosition = new ShaderProgram;
	m_shaderRaycast = new ShaderProgram;
	m_shaderPosition->loadShader("", "shader/rayposition.frag");
	m_shaderRaycast->loadShader("", "shader/raycast.frag");
}

void VolumeRender::initFBO()
{
	if (m_fbo == NULL)
	{
		m_fbo = new FramebufferObject;
	}
	m_fbo->Bind();
	m_fbo->AttachTexture(GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId(), GL_COLOR_ATTACHMENT0_EXT);
	m_fbo->AttachTexture(GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId(),   GL_COLOR_ATTACHMENT1_EXT);
	m_fbo->AttachTexture(GL_TEXTURE_RECTANGLE_ARB, m_depthTex->getId(),    GL_DEPTH_ATTACHMENT_EXT);
	m_fbo->IsValid();
	FramebufferObject::Disable();
}

void VolumeRender::drawBBox()
{
	// 	float clip[6] = {0.f, 0.0f, 0.f, 0.0f, 0.f, 0.f};
	// 	float clip_tex[6] = {0.f, 0.0f, 0.f, 0.0f, 0.f, 0.f};
	// 	Vector3f texCoord[8] = {	// volume vertex edge coords
	// 		Vector3f(    clip_tex[0],     clip_tex[2],     clip_tex[4]), 
	// 		Vector3f(1 - clip_tex[1],     clip_tex[2],     clip_tex[4]),
	// 		Vector3f(    clip_tex[0], 1 - clip_tex[3],     clip_tex[4]), 
	// 		Vector3f(1 - clip_tex[1], 1 - clip_tex[3],     clip_tex[4]), 
	// 		Vector3f(    clip_tex[0],     clip_tex[2], 1 - clip_tex[5]),
	// 		Vector3f(1 - clip_tex[1],     clip_tex[2], 1 - clip_tex[5]),
	// 		Vector3f(    clip_tex[0], 1 - clip_tex[3], 1 - clip_tex[5]),
	// 		Vector3f(1 - clip_tex[1], 1 - clip_tex[3], 1 - clip_tex[5])
	// 	};
	// 	Vector3f vertex[8] = {	// volume vertex edge coords
	// 		Vector3f(              clip[0],               clip[2],               clip[4]),
	// 		Vector3f(m_bboxDim.x - clip[1],               clip[2],               clip[4]),
	// 		Vector3f(              clip[0], m_bboxDim.y - clip[3],               clip[4]),
	// 		Vector3f(m_bboxDim.x - clip[1], m_bboxDim.y - clip[3],               clip[4]), 
	// 		Vector3f(              clip[0],               clip[2], m_bboxDim.z - clip[5]),
	// 		Vector3f(m_bboxDim.x - clip[1],               clip[2], m_bboxDim.z - clip[5] ),
	// 		Vector3f(              clip[0], m_bboxDim.y - clip[3], m_bboxDim.z - clip[5]),
	// 		Vector3f(m_bboxDim.x - clip[1], m_bboxDim.y - clip[3], m_bboxDim.z - clip[5])
	// 	};
	Vector3f texCoord[8] = {	// volume vertex edge coords
		Vector3f(0, 0, 0 ), Vector3f(1, 0, 0 ), Vector3f(0, 1, 0), Vector3f(1, 1, 0), 
		Vector3f(0, 0, 1 ), Vector3f(1, 0, 1 ), Vector3f(0, 1, 1), Vector3f(1, 1, 1)
	};
	Vector3f vertex[8] = {	// volume vertex edge coords
		Vector3f(0, 0, 0 ), Vector3f(m_bboxDim.x, 0, 0 ), Vector3f(0, m_bboxDim.y, 0), Vector3f(m_bboxDim.x, m_bboxDim.y, 0), 
		Vector3f(0, 0, m_bboxDim.z ), Vector3f(m_bboxDim.x, 0, m_bboxDim.z ), Vector3f(0, m_bboxDim.y, m_bboxDim.z), Vector3f(m_bboxDim.x, m_bboxDim.y, m_bboxDim.z)
	};

	int faceID[] = { 1, 3, 7, 5, 0, 4, 6, 2, 2, 6, 7, 3, 0, 1, 5, 4, 4, 5, 7, 6, 0, 2, 3, 1 };

	glBegin(GL_QUADS);
	for(int i = 0; i < 24; i += 4) {
		glTexCoord3f(texCoord[faceID[i]].x, texCoord[faceID[i]].y, texCoord[faceID[i]].z);
		glVertex3f(vertex[faceID[i]].x, vertex[faceID[i]].y, vertex[faceID[i]].z);

		glTexCoord3f(texCoord[faceID[i + 1]].x, texCoord[faceID[i + 1]].y, texCoord[faceID[i + 1]].z);
		glVertex3f(vertex[faceID[i + 1]].x, vertex[faceID[i + 1]].y, vertex[faceID[i + 1]].z);

		glTexCoord3f(texCoord[faceID[i + 2]].x, texCoord[faceID[i + 2]].y, texCoord[faceID[i + 2]].z);
		glVertex3f(vertex[faceID[i + 2]].x, vertex[faceID[i + 2]].y, vertex[faceID[i + 2]].z);

		glTexCoord3f(texCoord[faceID[i + 3]].x, texCoord[faceID[i + 3]].y, texCoord[faceID[i + 3]].z);
		glVertex3f(vertex[faceID[i + 3]].x, vertex[faceID[i + 3]].y, vertex[faceID[i + 3]].z);
	}
	glEnd();
}

void VolumeRender::set1DTF( unsigned char* data )
{
	m_tfTex->setPixelData(data);
	Vector3i dim= m_tfTex->getDimensions();
	GLubyte *b = m_tfTex->pixels_;
	for(int i = 0;i < dim.x;i++){
		for(int j = 0;j < dim.y;j++){
			for(int k = 0;k < dim.z;k++){
				//cout<<(int)*(data+i*dim.x+j*dim.y+k)<<" ";
				if(*(b+i*dim.y*dim.z+j*dim.z+k) < THRESHHOLD_VALUE)
					cout << *(b+i*dim.y*dim.z+j*dim.z+k);
			}
		}
	}
	m_tfTex->uploadTexture();
	update();

	//////////////////////////////////////////////////////////////////////////
	//  ‰≥ˆ≤‚ ‘
	// 	int i,k;
	// 	for (i = k = 0; k < 256; k++)
	// 	{
	// 		i = k * 4;
	// 		printf("TF[%3d]:(%d, %d, %d, %d)\n", k, data[i], data[i + 1], data[i + 2], data[i + 3]);
	// 	}
}

void VolumeRender::onDataLoaded( const Volume* v )
{}
