#include "ColorVolumeRender.h"
#include "dm/GradientCalculator.h"
#include "imdebug/imdebuggl.h"
#include <minmax.h>
#include <QKeyEvent>

ColorVolumeRender::ColorVolumeRender(QWidget *parent)
	: VolumeLightingRender(parent), m_clusterTex(0), m_clusterVolume(0), m_renderMode(3), m_shaderCluster(0), m_shaderClusterTriLinear(0), m_clusterVolTex(0), m_shaderLighting(0), m_shaderGradOpacityLighting(0), m_gradientMode(RCMG), m_gm(0)
{
//	tx = ty = tz = 0;
}

ColorVolumeRender::~ColorVolumeRender(void)
{
	SAFE_DELETE(m_clusterVolTex);
	SAFE_DELETE(m_clusterTex);
	SAFE_DELETE_ARRAY(m_clusterVolume);
	SAFE_DELETE(m_shaderCluster);
	SAFE_DELETE(m_shaderClusterTriLinear);
	SAFE_DELETE(m_shaderLighting);
	SAFE_DELETE(m_shaderGradOpacityLighting);
}

void ColorVolumeRender::paintGL()
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
	else if (m_renderMode == 3)
	{
		renderByRGBVolumeLighting();
	}
	else if (m_renderMode == 4)
	{
		renderByRGBVolumeGradientOpacityLighting();
	}
	
	//imdebugPixelsf(0, 0, width(), height(), GL_RGBA);
	//////////////////////////////////////////////////////////////////////////
	glDisable(GL_DEPTH_TEST);
	m_camera.setOrthoProjection();
	glMultMatrixd(m_camera.getRotateMatrix());
	drawCoord();
	m_camera.setOrthoProjection();
	m_camera.drawArc();
	glEnable(GL_DEPTH_TEST);
}

void ColorVolumeRender::initTextures()
{
	VolumeLightingRender::initTextures();

	m_tfTex->setFilter(Texture::NEAREST);

	m_volTex->setFormat(GL_RGB);
	m_volTex->setInternalFormat(GL_RGB8);
	m_volTex->setFilter(Texture::NEAREST);
//	m_volTex->setDataType(GL_UNSIGNED_BYTE);
//	m_volTex->setWrapping(Texture::CLAMP);

	m_clusterVolTex = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, Texture::NEAREST, false);
	m_clusterVolTex->setWrapping(Texture::CLAMP);

	m_clusterTex = new Texture((GLubyte*)0, Vector3i(256,256,256), GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, Texture::NEAREST, false);
	m_clusterTex->setWrapping(Texture::CLAMP);

	m_gradMagTex = new Texture((GLubyte*)0, Vector3i(0,0,0), GL_LUMINANCE, GL_LUMINANCE, GL_UNSIGNED_BYTE, Texture::LINEAR, false);
	m_gradMagTex->setWrapping(Texture::CLAMP);

	m_gmTFTex = new Texture(Vector3i(256,1,1), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, Texture::LINEAR, false);
	m_gmTFTex->setWrapping(Texture::CLAMP);
}

void ColorVolumeRender::initShader()
{
	m_shaderPosition = new ShaderProgram;
	m_shaderRaycast = new ShaderProgram;
	m_shaderCluster = new ShaderProgram;
	m_shaderClusterTriLinear = new ShaderProgram;
	m_shaderPosition->loadShader("", "shader/rayposition.frag");
	m_shaderRaycast->loadShader("", "shader/color_volume_raycast.frag");
	m_shaderCluster->loadShader("", "shader/color_cluster_volume_raycast.frag");
	m_shaderClusterTriLinear->loadShader("", "shader/color_cluster_volume_raycast_tri_linear.frag");
	
	m_shaderLighting = new ShaderProgram;
	m_shaderLighting->loadShader("", "shader/color_volume_raycast_lighting.frag");
	m_shaderGradOpacityLighting = new ShaderProgram;
	m_shaderGradOpacityLighting->loadShader("", "shader/color_volume_raycast_grad_opacity_lighting.frag");
}

void ColorVolumeRender::loadData( const char* filename )
{
	m_volume.loadVolume(filename);
	m_bboxDim = m_volume.getNormalizedDim();
	m_camera.setSceneBBox(Vector3d(0, 0, 0), Vector3d(m_bboxDim.x, m_bboxDim.y, m_bboxDim.z));
	Vector3i dim = m_volume.getDimension();
	m_step = 0.5 / max(dim.x, max(dim.y, dim.z));

	m_volTex->setDimensions(dim);
	m_volTex->setPixelData(m_volume.getVolume());
	m_volTex->uploadTexture();
// 	m_colorVolTex->setDimensions(dim);
// 	m_colorVolTex->setPixelData(m_volume.getVolume());
// 	m_colorVolTex->uploadTexture();
//	setRenderMode(m_renderMode);
// 	QImage img(m_volume.getVolume(), dim.x, dim.y, QImage::Format_RGB888);
// 	img.save("./data/test.png", "PNG");
	emit dataLoaded(&m_volume);
}

/**
 * ColorTFWidget的signal连接到此slot，在ColorTFCanvas中的ClusteredColorTF计算完
 * 聚类之后发出signal，将聚类结果传递至此slot
 */
void ColorVolumeRender::setClusterInfo( unsigned char* data )
{
	m_clusterData = data;
	m_clusterTex->setPixelData(data);
	m_clusterTex->uploadTexture();

	genClusterVolume(data);
	m_clusterVolTex->setDimensions(m_volume.getDimension());
	m_clusterVolTex->setPixelData(m_clusterVolume);
	m_clusterVolTex->uploadTexture();
// 	m_volTex->setDimensions(m_volume.getDimension());
// 	m_volTex->setPixelData(m_clusterVolume);
// 	m_volTex->uploadTexture();
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

void ColorVolumeRender::setRenderMode(int mode)
{
	if (mode >= 0 && mode < 5)
	{
		m_renderMode = mode;
	}
	else 
	{
		printf("[ColorVolumeRender] Error render mode.\n");
	}
	update();
}

void ColorVolumeRender::setGradientMode( int mode )
{
	if (mode >= RCMG && mode <= Sobel)
	{
		m_gradientMode = (GradientMode)mode;
		calcGradient();
		printf("Gradient recalculated.. > %d\n", mode);
		update();
	}
	else 
	{
		printf("[ColorVolumeRender] Error render mode.\n");
	}
}

void ColorVolumeRender::genClusterVolume( unsigned char* data )
{
	Vector3i dim = m_volume.getDimension();
	int len = dim.x * dim.y * dim.z;
	unsigned char* vol = m_volume.getVolume();
	int idx;

	if (m_clusterVolume != NULL)
		SAFE_DELETE_ARRAY(m_clusterVolume);
	m_clusterVolume = new unsigned char[len];
	memset(m_clusterVolume, 0, len);
	for (int i = 0; i < len; ++i)
	{
		idx = i * 3;
		m_clusterVolume[i] = data[vol[idx] * 256 * 256 + vol[idx + 1] * 256 + vol[idx + 2]];
	}

	//////////////////////////////////////////////////////////////////////////
	// 输出测试
	size_t cluster[256];
	memset(cluster, 0, 256 * sizeof(unsigned int));
	for (int i = 0; i < len; ++i)
	{
		cluster[m_clusterVolume[i]] ++;
	}
	for (int i = 0; i < 256; ++i)
	{
		if(cluster[i] != 0)
			printf("[%s:%d(%s)] > [%d] : %u\n", __FILE__, __LINE__, __FUNCTION__, i, cluster[i]);
	}
}

//使用cluster volume来查询传输函数调整不透明度，使用原体数据的颜色
void ColorVolumeRender::renderByRGBVolume()
{
	m_shaderRaycast->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderRaycast->setUniform("step", m_step);
	m_shaderRaycast->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderRaycast->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderRaycast->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderRaycast->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_volTex->getId());
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
void ColorVolumeRender::renderByClusterVolume()
{
	m_shaderCluster->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderCluster->setUniform("step", m_step);
	m_shaderCluster->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderCluster->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderCluster->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderCluster->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_clusterVolTex->getId());
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
void ColorVolumeRender::renderByClusterVolumeTriLinear()
{
	m_shaderClusterTriLinear->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderClusterTriLinear->setUniform("step", m_step);
	m_shaderClusterTriLinear->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderClusterTriLinear->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderClusterTriLinear->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderClusterTriLinear->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_volTex->getId());
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

//计算梯度，用于Shading
void ColorVolumeRender::onDataLoaded( const Volume* v )
{
	calcGradient();
}

void ColorVolumeRender::calcGradient()
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
	int SLTest = 2; // 0, 正常计算。 1, 写入文件。 2,从文件读
	//const char* fname = "z:/ColorCube2";
	//const char* fname = "z:/leg_256_256_282_white_salt_pepper_0.1";
	//const char* fname = "z:/leg_256_256_282";
	char fname[100];
	//const char* fname = "z:/torso_512_256_256";
	//const char* fname = "z:/leg_256_256_282_RCMG_Lab_3x3x3_01";
	//const char* fname = "z:/leg_256_256_256_RCMG_Lab_3x3x3_01";
	//const char* fname = "z:/head_128_128_128_RCMG_Lab_3x3x3_01";
	//const char* fname = "z:/head_256_256_256_RCMG_Lab_3x3x3_01";
	//const char* fname = "z:/ihead_128_128_128_RCMG_Lab_3x3x3_01";
	//const char* fname = "z:/ihead_256_256_256_RCMG_Lab_3x3x3_01";
	//const char* fname = "z:/ihead_512_512_256_RCMG_Lab_3x3x3_01";
// 	int SLTest = 2;
// 	//const char* fname = "z:/ColorCube_RCMG_Lab_3x3x3_01";
// 	const char* fname = "z:/TestSphere_RCMG_Lab_3x3x3_01";

	//////////////////////////////////////////////////////////////////////////
	// 计算梯度
	char *mode_str[] = {"RCMG", "LuvCD", "IntensityCD", "CDG", "Sobel"};
	sprintf(fname, "%s_%s.grad", m_volume.getFileName(), mode_str[m_gradientMode]);
	//m_gradientMode = ColorVolumeRender::IntensityCD; ///////////////////// 测试用
	if(!loadGradient(fname)) // 从文件加载，失败则重新计算
	{
		float *m_g = new float[len3];
		memset(m_g, 0, sizeof(float) * len3);

		float *m_gradMag = new float[len];
		memset(m_gradMag, 0, sizeof(float) * len);
		//////////////////////////////////////////////////////////////////////////
		switch (m_gradientMode)
		{
		case ColorVolumeRender::RCMG:	//使用RCMG方法计算彩色梯度
			calcColorLuvGradientRCMG(m_g, m_volume.data<unsigned char>(), dim, 3);
			break;
		case ColorVolumeRender::LabCD:	//直接用彩色数据计算梯度(Lab/Luv)
			calcColorLabGradientCentralDiff(m_g, m_volume.data<unsigned char>(), dim);
			break;
		case ColorVolumeRender::IntensityCD://彩色数据转为标量数据后再计算梯度
			calcColorScalarGradientCentralDiff(m_g, m_volume.data<unsigned char>(), dim);
			break;
		case ColorVolumeRender::CDG:
			calcColorDistanceGradient(m_g, m_volume.data<unsigned char>(), dim);
			break;
		case ColorVolumeRender::Sobel:
			calcLUVSobelGradient(m_g, m_volume.data<unsigned char>(), dim);
			break;
		default:
			break;
		}

		//////////////////////////////////////////////////////////////////////////
		//计算梯度模并得到其最大值,同时将梯度转换为uint8类型
		int i;
		float *ptrf;
		unsigned char *ptruc;
		for (i =0, m_maxGradMag = 0, ptrf = m_gradMag; i < len3; i += 3, ++ptrf)
		{
			Vector3f tmp_g(m_g[i], m_g[i + 1], m_g[i + 2]);
			*ptrf = tmp_g.norm();

			if (*ptrf > DOUBLE_EPSILON)
			{
				m_gradient[i    ] = (unsigned char)((m_g[i    ] / (*ptrf) + 1.0) / 2.0 * 255 + 0.5);
				m_gradient[i + 1] = (unsigned char)((m_g[i + 1] / (*ptrf) + 1.0) / 2.0 * 255 + 0.5);
				m_gradient[i + 2] = (unsigned char)((m_g[i + 2] / (*ptrf) + 1.0) / 2.0 * 255 + 0.5);
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

		//将梯度模归一化并转为uint8
		for (i = 0, ptrf = m_gradMag, ptruc = m_gm; i < len; ++i, ++ptrf, ++ptruc)
		{
			(*ptrf) /= m_maxGradMag;
			(*ptruc) = (unsigned char)((*ptrf) * 255 + 0.5);
		}

		SAFE_DELETE_ARRAY(m_g);
		SAFE_DELETE_ARRAY(m_gradMag);
		// 将梯度归一化并转为unsigned char
		//nonNormlizedGradientF2UC(m_g, m_gradient, dim);

		//////////////////////////////////////////////////////////////////////////
		// 保存梯度到文件
		saveGradient(fname);
	}

// 	if (m_gradientMode == RCMG)//使用RCMG方法计算彩色梯度
// 	{
// 		printf("RCMG Gradient ... ... ");
// 
// 		//calcColorLuvGradientRCMG(m_g, m_volume.data<unsigned char>(), dim, 3);
// 		if (SLTest != 2)
// 		{
// 			//calcColorLuvGradientRCMG(m_g, m_volume.data<unsigned char>(), dim, 3);
// 			calcColorLabGradientRCMG(m_g, m_volume.data<unsigned char>(), dim, 3);
// 			//calcColorLuvGradientRCMG(m_g, m_volume.data<unsigned char>(), dim, 5);
// 			//loadGradient("leg_RCMG_5x5x5_10_Lab_BruteForce.grad", true);
// 			//loadGradient("leg_RCMG_3x3x3_01_Lab_BruteForce.grad", true);
// 			//loadGradient("leg_RCMG_3x3x3_03_Lab_BruteForce.grad", true);
// 			//loadGradient("leg_RCMG_3x3x3_03_Luv_BruteForce.grad", true);
// 			//loadGradient("leg_RCMG_3x3x3_01_Luv_BruteForce.grad", true);
// 		}
// 	}
// 	else if (m_gradientMode == LabCD) //直接用彩色数据计算梯度(Lab/Luv)
// 	{
// 		printf("Lab Central Difference Gradient ... ... ");
// 
// 		calcColorLabGradientCentralDiff(m_g, m_volume.data<unsigned char>(), dim);
// 		//calcColorLUVGradientCentralDiff(m_g, m_volume.data<unsigned char>(), dim);
// 	}
// 	else if (m_gradientMode == IntensityCD)//彩色数据转为标量数据后再计算梯度
// 	{
// 		printf("Intensity Central Difference Gradient ... ... ");
// 		calcColorScalarGradientCentralDiff(m_g, m_volume.data<unsigned char>(), dim);
// 	}

// 	//////////////////////////////////////////////////////////////////////////
// 	// 计算梯度模
// 	if(m_gradientMode != RCMG || SLTest != 2)
// 	{
// 		Vector3f p;
// 		float norm_;
// 		m_maxGradMag = 0;
// 		int idx = 0;
// 		//计算梯度模并得到其最大值
// 		for (int i = 0; i < len3; i += 3, ++idx)
// 		{
// 			p.set(m_g[i], m_g[i + 1], m_g[i + 2]);
// 			norm_ = p.norm();
// 			m_gradMag[idx] = norm_;
// 			if (norm_ > m_maxGradMag)
// 				m_maxGradMag = norm_;
// 		}
// 		//printf("Max Gradient Magnitude = %f\n", m_maxGradMag);
// 		//归一化
// 		for (int i = 0; i < len; ++i)
// 		{
// 			m_gradMag[i] /= m_maxGradMag;
// 			m_gm[i] = (unsigned char)(m_gradMag[i] * 255 + 0.5);
// 		}
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		// 将梯度归一化并转为unsigned char
// //	 	gradientNormalize(m_g, dim);
// // 		gradientF2UC(m_g, m_gradient, dim);
// 		nonNormlizedGradientF2UC(m_g, m_gradient, dim);
// 		//////////////////////////////////////////////////////////////////////////
// 	}

// 	int idx = 0;
// 	//计算梯度模并得到其最大值
// 	for (int i = 0; i < len3; i += 3, ++idx)
// 	{
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	// Save / Load ##### FOR TEST ######
// 	if (m_gradientMode == RCMG && SLTest == 1)
// 	{
// 		char fileName[50];
// 		sprintf(fileName, "%s.gf", fname);
// 		saveGradient(fileName);
// 		sprintf(fileName, "%s.gm", fname);
// 		saveGradMag(fileName);
// 		sprintf(fileName, "%s.guc", fname);
// 		saveGradientUC(fileName);
// 	}
// 	else if(m_gradientMode == RCMG && SLTest == 2)
// 	{
// 		char fileName[50];
// 		sprintf(fileName, "%s.gf", fname);
// 		loadGradient(fileName);
// 		sprintf(fileName, "%s.gm", fname);
// 		loadGradMag(fileName);
// 		sprintf(fileName, "%s.guc", fname);
// 		loadGradientUC(fileName);
// 	}

	//////////////////////////////////////////////////////////////////////////
	// 更新纹理
	m_gradientTex->setDimensions(dim);
	m_gradientTex->setPixelData((GLubyte*)(m_gradient));
	m_gradientTex->uploadTexture();

	m_gradMagTex->setDimensions(dim);
	m_gradMagTex->setPixelData((GLubyte*)(m_gm));
	m_gradMagTex->uploadTexture();

	emit gradientChanged(m_gradient, m_gm);

	m_gmVol.setDimension(dim);
	m_gmVol.setData(m_gm);
	emit sendGradMag(&m_gmVol);
}

void ColorVolumeRender::renderByRGBVolumeLighting()
{
	m_shaderLighting->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderLighting->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderLighting->setUniformSampler("texEndPos", GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderLighting->setUniformSampler("texTransfunc", GL_TEXTURE2, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderLighting->setUniformSampler("texVolume", GL_TEXTURE3, GL_TEXTURE_3D, m_volTex->getId());
	m_shaderLighting->setUniformSampler("texCluster", GL_TEXTURE4, GL_TEXTURE_3D, m_clusterTex->getId());
	m_shaderLighting->setUniformSampler("texGradient", GL_TEXTURE5, GL_TEXTURE_3D, m_gradientTex->getId());
	m_shaderLighting->setUniform("step", m_step);
	m_shaderLighting->setUniform("ka", m_ka);
	m_shaderLighting->setUniform("ks", m_ks);
	m_shaderLighting->setUniform("kd", m_kd);
	m_shaderLighting->setUniform("shininess", m_shininess);

	//m_shaderLighting->setUniform("lightPos", m_lightPos.x, m_lightPos.y, m_lightPos.z);
	Vector3f viewDir = m_camera.viewDirection();
// // 	printf("-----------------------------------\n");
// // 	printf("%f %f %f\n", viewDir.x, viewDir.y, viewDir.z);
// // 	printf("%f %f %f\n", mv[2], mv[6], mv[10]);
// // 	printf("-----------------------------------\n");
// 	float mv[16];
// 	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
// 	Vector3f viewDir(-mv[2], -mv[6], -mv[10]);
 	if(viewDir.normSquared() != 0) viewDir = viewDir.normalize();
	m_shaderLighting->setUniform("lightPos", viewDir.x, viewDir.y, viewDir.z);
	m_shaderLighting->setUniform("viewDir", viewDir.x, viewDir.y, viewDir.z); 

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(0, 0);
	glTexCoord2f(0.0, height());
	glVertex2f(0, height());  
	glTexCoord2f(width(), height());
	glVertex2f(width(), height());
	glTexCoord2f(width(), 0.0);
	glVertex2f(width(), 0);
	glEnd();

	m_shaderLighting->deactivate();
}

void ColorVolumeRender::renderByRGBVolumeGradientOpacityLighting()
{
	m_shaderGradOpacityLighting->activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera.setOrtho2DProjection();

	m_shaderGradOpacityLighting->setUniformSampler("texStartPos", GL_TEXTURE0, GL_TEXTURE_RECTANGLE_ARB, m_startPosTex->getId());
	m_shaderGradOpacityLighting->setUniformSampler("texEndPos",   GL_TEXTURE1, GL_TEXTURE_RECTANGLE_ARB, m_endPosTex->getId());
	m_shaderGradOpacityLighting->setUniformSampler("texVolume",   GL_TEXTURE2, GL_TEXTURE_3D, m_volTex->getId());
	m_shaderGradOpacityLighting->setUniformSampler("texGradient", GL_TEXTURE3, GL_TEXTURE_3D, m_gradientTex->getId());
	m_shaderGradOpacityLighting->setUniformSampler("texGradMag",  GL_TEXTURE4, GL_TEXTURE_3D, m_gradMagTex->getId());
	m_shaderGradOpacityLighting->setUniformSampler("colorTF", GL_TEXTURE5, GL_TEXTURE_1D, m_tfTex->getId());
	m_shaderGradOpacityLighting->setUniformSampler("gradMTF", GL_TEXTURE6, GL_TEXTURE_1D, m_gmTFTex->getId());
	m_shaderGradOpacityLighting->setUniformSampler("texCluster", GL_TEXTURE7, GL_TEXTURE_3D, m_clusterTex->getId());
	m_shaderGradOpacityLighting->setUniform("step", m_step);
	m_shaderGradOpacityLighting->setUniform("ka", m_ka);
	m_shaderGradOpacityLighting->setUniform("ks", m_ks);
	m_shaderGradOpacityLighting->setUniform("kd", m_kd);
	m_shaderGradOpacityLighting->setUniform("shininess", m_shininess);

// 	float mv[16];
// 	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
// 	Vector3f viewDir(-mv[2], -mv[6], -mv[10]);
// 	viewDir = viewDir.normalize();
 	Vector3f viewDir = m_camera.viewDirection();
	m_shaderGradOpacityLighting->setUniform("lightPos", viewDir.x, viewDir.y, viewDir.z);
	m_shaderGradOpacityLighting->setUniform("viewDir", viewDir.x, viewDir.y, viewDir.z); 

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

	m_shaderGradOpacityLighting->deactivate();
}

//////////////////////////////////////////////////////////////////////////
// for test
int tx = 3;
int ty = 3;
int tz = 3;
int cur_sel = 14;
void ColorVolumeRender::keyPressEvent( QKeyEvent *e )
{
	int candidate[27][3] = {
		{0, 0, 0}, {0, 0, 3}, {0, 0, 5}, {0, 3, 0}, {0, 3, 3}, {0, 3, 5}, {0, 5, 0}, {0, 5, 3}, {0, 5, 5},
		{3, 0, 0}, {3, 0, 3}, {3, 0, 5}, {3, 3, 0}, {3, 3, 3}, {3, 3, 5}, {3, 5, 0}, {3, 5, 3}, {3, 5, 5},
		{5, 0, 0}, {5, 0, 3}, {5, 0, 5}, {5, 3, 0}, {5, 3, 3}, {5, 3, 5}, {5, 5, 0}, {5, 5, 3}, {5, 5, 5},
	};
	bool recalc = false;
	if (e->key() == Qt::Key_Left)
	{
		recalc = true;
// 		if (tx > 0 || ty > 0 || tz > 0)
// 		{
// 			if (tz > 0)
// 			{
// 				tz--;
// 			}
// 			else
// 			{
// 				if (ty > 0)
// 				{
// 					ty --;
// 					tz = 5;
// 				}
// 				else
// 				{
// 					if (tx > 0)
// 					{
// 						ty = 5;
// 						tz = 5;
// 						tx --;
// 					}
// 				}
// 			}
// 		}
		cur_sel = (cur_sel + 26) % 27;		
	}
	else if (e->key() == Qt::Key_Right)
	{
// 		if (tx < 5 || ty < 5 || tz < 5)
// 		{
// 			recalc = true;
// 			if (tz < 5)
// 			{
// 				tz++;
// 			}
// 			else
// 			{
// 				if (ty < 5)
// 				{
// 					ty ++;
// 					tz = 0;
// 				}
// 				else
// 				{
// 					if (tx < 5)
// 					{
// 						ty = 0;
// 						tz = 0;
// 						tx ++;
// 					}
// 				}
// 			}
// 		}
		recalc = true;
		cur_sel = (cur_sel + 1) % 27;
	}
	tx = candidate[cur_sel][0];
	ty = candidate[cur_sel][1];
	tz = candidate[cur_sel][2];
	printf("[%d]tx = %d, ty = %d, tz = %d\n", cur_sel, tx, ty, tz);
	if (recalc)
	{
		calcGradient();
		update();
		printf("Reloaded >> %d, %d, %d\n", tx, ty, tz);
	}
}

bool ColorVolumeRender::loadGradient( const char* filename )
{
	bool ret = false;
	//从文件中读取梯度
	FILE* f = fopen(filename, "rb+");
	if (f != NULL)
	{
		Vector3i dim = m_volume.getDimension();
// 		if (fromBruteForce)
// 		{
// 			float block[18];
// 			int len3 = dim.x * dim.y * dim.z * 3;
// 			for (int i = 0; i < len3; i += 3)
// 			{
// 				fread(block, sizeof(float), 18, f);
// 				m_g[i + 0] = block[ 0 + tx];
// 				m_g[i + 1] = block[ 6 + ty];
// 				m_g[i + 2] = block[12 + tz];
// 			}
// 		} 
// 		else
// 		{
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
//		}

		fclose(f);
	}
	return ret;
}

void ColorVolumeRender::saveGradient( const char* filename )
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

void ColorVolumeRender::saveGradMag( const char* filename )
{
// 	FILE* f = fopen(filename, "wb+");
// 	if (f != NULL)
// 	{
// 		Vector3i dim = m_volume.getDimension();
// 		fwrite(&m_maxGradMag, sizeof(float), 1, f);
// 		fwrite(m_gradMag, sizeof(float), dim.x * dim.y * dim.z, f);
// 	}
// 	fclose(f);
}

void ColorVolumeRender::saveGradientUC( const char* filename )
{
	FILE* f = fopen(filename, "wb+");
	if (f != NULL)
	{
		Vector3i dim = m_volume.getDimension();
		fwrite(m_gradient, sizeof(unsigned char), dim.x * dim.y * dim.z * 3, f);
	}
	fclose(f);
}

void ColorVolumeRender::loadGradMag( const char* filename )
{
// 	FILE* f = fopen(filename, "rb+");
// 	if (f != NULL)
// 	{
// 		Vector3i dim = m_volume.getDimension();
// 		fread(&m_maxGradMag, sizeof(float), 1, f);
// 		fread(m_gradMag, sizeof(float), dim.x * dim.y * dim.z, f);
// 	}
// 	fclose(f);
}

void ColorVolumeRender::loadGradientUC( const char* filename )
{
	FILE* f = fopen(filename, "rb+");
	if (f != NULL)
	{
		Vector3i dim = m_volume.getDimension();
		fread(m_gradient, sizeof(unsigned char), dim.x * dim.y * dim.z * 3, f);
	}
	fclose(f);
}

void ColorVolumeRender::setGMTF( unsigned char* data )
{
	m_gmTFTex->setPixelData(data);
	m_gmTFTex->uploadTexture();
	update();
	//////////////////////////////////////////////////////////////////////////
	// 输出测试
// 	int i,k;
// 	for (i = k = 0; k < 256; k++)
// 	{
// 		i = k * 4;
// 		printf("TF[%3d]:(%d, %d, %d, %d)\n", k, data[i], data[i + 1], data[i + 2], data[i + 3]);
// 	}
}
