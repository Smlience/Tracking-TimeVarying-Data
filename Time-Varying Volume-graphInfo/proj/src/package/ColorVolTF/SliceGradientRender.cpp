#include "SliceGradientRender.h"
#include "dm/Volume.h"
#include <minmax.h>
#include <QAction>
#include <QMenu>
#include <QFileDialog>
#include <QLabel>
#define  _USE_MATH_DEFINES
#include <math.h>
//#include "SliceGradientRenderControl.h"

SliceGradientRender::SliceGradientRender( QWidget *parent /*= NULL*/ )
	: QWidget(parent), m_gradient(0), m_gradientMag(0), m_volume(0), m_slice(0), m_dir(YOZ), m_pix(0), m_gradMap(0)
	, m_gradArrowLength(20), m_elementIdx(1, 2), m_lineWidth(2), m_gradFilter(0.1, 1.0), m_gmpix(0), m_showGM(Qt::Unchecked), m_showImage(Qt::Checked), m_showVector(Qt::Checked)
{
	m_actSaveImg = new QAction("Save Image", this);
	m_actScreenshot = new QAction("Screenshot", this);
	setMinimumWidth(256);
	connect(m_actSaveImg, SIGNAL(triggered()), this, SLOT(saveImage()));
	connect(m_actScreenshot, SIGNAL(triggered()), this, SLOT(screenshot()));


// 	m_sbSlice = new QSpinBox(this);
// 	m_sbSlice->move(5, 5);
//	m_sbSlice->setMinimum(0);
// 	m_sbSlice->setFixedWidth(40);
// 	
// 	m_cbDir = new QComboBox(this);
// 	m_cbDir->setFixedWidth(75);
// 	m_cbDir->move(m_sbSlice->pos().x() + m_sbSlice->width() + 4, 5);
// 	m_cbDir->addItems(QStringList() << "Slice-YZ" << "Slice-XZ" << "Slice-XY");
// 
// 	QLabel* labLineWidth = new QLabel("Line Width:", this);
// 	labLineWidth->setFixedWidth(65);
// 	labLineWidth->move(m_cbDir->pos().x() + m_cbDir->width() + 4, 8);
// 	
// 	m_sbLineWidth = new QSpinBox(this);
// 	m_sbLineWidth->setFixedWidth(30);
// 	m_sbLineWidth->setValue(m_lineWidth);
// 	m_sbLineWidth->setRange(1, 5);
// 	m_sbLineWidth->move(labLineWidth->pos().x() + labLineWidth->width() + 4, 5);
// 	
// 	QLabel* labLineLength = new QLabel("Line Length:", this);
// 	labLineLength->setFixedWidth(65);
// 	labLineLength->move(m_sbLineWidth->pos().x() + m_sbLineWidth->width() + 4, 8);
// 
// 	m_sbLineLength = new QSpinBox(this);
// 	m_sbLineLength->setFixedWidth(30);
// 	m_sbLineLength->setValue(m_gradArrowLength);
// 	m_sbLineLength->setRange(1, 100);
// 	m_sbLineLength->move(labLineLength->pos().x() + labLineLength->width() + 4, 5);
// 
// 	QLabel* labGradFilter = new QLabel("Grad Filter:", this);
// 	labGradFilter->setFixedWidth(70);
// 	labGradFilter->move(m_sbLineLength->pos().x() + m_sbLineLength->width() + 4, 8);
// 
// 	m_sbGradFilter_min = new QDoubleSpinBox(this);
// 	m_sbGradFilter_min->setMinimum(0.001);
// 	m_sbGradFilter_min->setMaximum(1.0);
// 	m_sbGradFilter_min->setDecimals(3);
// 	m_sbGradFilter_min->setSingleStep(0.1);
// 	m_sbGradFilter_min->setValue(m_gradFilter.x);
// 	m_sbGradFilter_min->setFixedWidth(60);
// 	m_sbGradFilter_min->move(labGradFilter->pos().x() + labGradFilter->width() + 4, 5);
// 
// 	m_sbGradFilter_max = new QDoubleSpinBox(this);
// 	m_sbGradFilter_max->setMinimum(0.001);
// 	m_sbGradFilter_max->setMaximum(1.0);
// 	m_sbGradFilter_max->setDecimals(3);
// 	m_sbGradFilter_max->setSingleStep(0.1);
// 	m_sbGradFilter_max->setValue(m_gradFilter.y);
// 	m_sbGradFilter_max->setFixedWidth(60);
// 	m_sbGradFilter_max->move(m_sbGradFilter_min->pos().x() + m_sbGradFilter_min->width() + 4, 5);
// 
// 	m_cbShowGM = new QCheckBox("Magnitude", this);
// 	m_cbShowGM->setChecked(false);
// 	m_cbShowGM->move(m_sbGradFilter_max->pos().x() + m_sbGradFilter_max->width() + 4, 5);
// 
// 	m_cbShowImage = new QCheckBox("Image", this);
// 	m_cbShowImage->setChecked(true);
// 	m_cbShowImage->move(m_cbShowGM->pos().x() + m_cbShowGM->width() + 4, 5);
// 
// 	m_cbShowVector = new QCheckBox("Vector", this);
// 	m_cbShowVector->setChecked(true);
// 	m_cbShowVector->move(m_cbShowImage->pos().x() + m_cbShowImage->width() + 4, 5);
//
// 	connect(m_cbShowGM, SIGNAL(stateChanged(int)), this, SLOT(on_cbShowGM_stateChanged(int)));
// 	connect(m_cbShowImage, SIGNAL(stateChanged(int)), this, SLOT(on_cbShowImage_stateChanged(int)));
// 	connect(m_cbShowVector, SIGNAL(stateChanged(int)), this, SLOT(on_cbShowVector_stateChanged(int)));
// 	connect(m_sbGradFilter_min, SIGNAL(valueChanged(double)), this, SLOT(on_sbGradFilter_min_valueChanged(double)));
// 	connect(m_sbGradFilter_max, SIGNAL(valueChanged(double)), this, SLOT(on_sbGradFilter_max_valueChanged(double)));
// 	connect(m_sbLineLength, SIGNAL(valueChanged(int)), this, SLOT(on_sbLineLength_valueChanged(int)));
// 	connect(m_sbLineWidth, SIGNAL(valueChanged(int)), this, SLOT(on_sbLineWidth_valueChanged(int)));
// 	connect(m_sbSlice, SIGNAL(valueChanged(int)), this, SLOT(on_sbSlice_valueChanged(int)));
// 	connect(m_cbDir, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbDir_currentIndexChanged(int)));

	m_control = new QWidget(this);
	ui.setupUi(m_control);
	m_control->setObjectName("SliceGradientRenderControl");
	m_control->setAutoFillBackground(true);
	//m_control->setStyleSheet("QWidget#SliceGradientRenderControl{border-radius:5px;}");
	//control->move(10, 10);
	ui.sbLineWidth->setValue(m_lineWidth);
	ui.sbLineLength->setValue(m_gradArrowLength);
	ui.sbGradFilter_min->setValue(m_gradFilter.x);
	ui.sbGradFilter_max->setValue(m_gradFilter.y);
	connect(ui.cbShowGM, SIGNAL(stateChanged(int)), this, SLOT(on_cbShowGM_stateChanged(int)));
	connect(ui.cbShowImage, SIGNAL(stateChanged(int)), this, SLOT(on_cbShowImage_stateChanged(int)));
	connect(ui.cbShowVector, SIGNAL(stateChanged(int)), this, SLOT(on_cbShowVector_stateChanged(int)));
	connect(ui.sbGradFilter_min, SIGNAL(valueChanged(double)), this, SLOT(on_sbGradFilter_min_valueChanged(double)));
	connect(ui.sbGradFilter_max, SIGNAL(valueChanged(double)), this, SLOT(on_sbGradFilter_max_valueChanged(double)));
	connect(ui.sbLineLength, SIGNAL(valueChanged(int)), this, SLOT(on_sbLineLength_valueChanged(int)));
	connect(ui.sbLineWidth, SIGNAL(valueChanged(int)), this, SLOT(on_sbLineWidth_valueChanged(int)));
	connect(ui.sbSlice, SIGNAL(valueChanged(int)), this, SLOT(on_sbSlice_valueChanged(int)));
	connect(ui.cbDir, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbDir_currentIndexChanged(int)));
	//control->move(width() - control->width() - 10, height() - control->height() - 10);
}

void SliceGradientRender::setVolume(const Volume* v )
{
	m_volume = v;
	m_vNormDim = m_volume->getNormalizedDim();
	float m = min(min(m_vNormDim.x, m_vNormDim.y), m_vNormDim.z);
	m_vNormDim /= m;
	m_sNormDim.set(m_vNormDim.y, m_vNormDim.z);
	Vector3i dim = m_volume->getDimension();
	m_sDim.set(dim.y, dim.z);
	ui.sbSlice->setMaximum(dim.x);
	showSlice(m_dir, m_slice);
	update();
}

void SliceGradientRender::setGradient(unsigned char *g, unsigned char* gm)
{
	m_gradient = g;
	m_gradientMag = gm;
//	m_maxgm = max_gm;
	showSlice(m_dir, m_slice);
	update();
}

void SliceGradientRender::paintEvent(QPaintEvent *e)
{
	if (m_volume != NULL && m_pix != NULL && !m_pix->isNull())
	{
		draw(this);
		//QPainter p(this);
		//p.fillRect(QRect(0, 0, width(), 35), Qt::lightGray);
	}
}

void SliceGradientRender::draw( QPaintDevice * device )
{
	QPainter p(device);
	QRect r(rect());
	Vector2i padding(10, m_control->height() + 5);
	p.fillRect(r, Qt::white);
	r.setWidth(r.width() - 2 * padding.x);
	r.setHeight(r.height() - 2 * padding.y);
	float ratio = float(m_pix->width()) / m_pix->height();
	if (r.width() < r.height() * ratio)
		r.setHeight(r.width() / ratio);
	else
		r.setWidth(r.height() * ratio);
	padding.set((this->width() - r.width()) / 2, padding.y);
	r.moveTo(padding.x, padding.y);
// 	if (m_showGM == Qt::Unchecked)
// 		p.drawImage(r, *m_pix);
// 	else if(m_showGM == Qt::Checked)
		
	QPen pen(Qt::black, 1);
	p.setPen(pen);
	p.drawRect(r);

	if (m_showImage == Qt::Checked)
	{
		p.drawImage(r, *m_pix);
	}
	if(m_showGM == Qt::Checked)
	{
		p.drawImage(r, *m_gmpix);
	}

	//绘制梯度
	if (m_showVector == Qt::Checked)
	{
		p.setRenderHint(QPainter::Antialiasing);
		QPointF p1,p2;
		int idx = 0;
		int w = m_pix->width();
		int h = m_pix->height();
		float ratio_w = r.width() / float(w);
		float ratio_h = r.height() / float(h);
		float gm = 0.0;

		Vector2f gdir;
		for (int i = 0; i < h; i += 5)
		{
			for (int j = 0; j < w; j += 5)
			{
				idx = i * w + j;
				gdir = m_gradMap[idx];

				gm = gdir.norm();
				//if (g.norm() > 0.5 && (gdir.x != 0 || gdir.y != 0))
				if(gm >= m_gradFilter.x && gm <= m_gradFilter.y)
				{
					gdir = gdir.normalize();
					p1.setX(j * ratio_w + padding.x); //映射到屏幕尺寸
					p1.setY(i * ratio_h + padding.y);
					p2.setX(j * ratio_w + padding.x + gdir.x * m_gradArrowLength);
					p2.setY(i * ratio_h + padding.y + gdir.y * m_gradArrowLength);
					//printf("%f,%f -> %f,%f\n", p1.x(), p1.y(), p2.x(), p2.y());
					//p.rotate();
					//p.setPen(QColor(0, 60, 196, 128));
					drawArrow(&p, p1, p2);
				}
			}
		}
	}
}

unsigned char lerp(const unsigned char &v0, const unsigned char &v1, float t)
{
	return (unsigned char)(v0 + (v1 - v0) * t);
}

#define GRAD_UC2F(V) (V / 255.0 * 2.0 - 1.0)
void SliceGradientRender::showSlice( int dir, int slice )
{
	if (m_volume == NULL || m_gradient == NULL)
		return;

	Vector3i dim = m_volume->getDimension();
	unsigned char* vdata = m_volume->data<unsigned char>();

	if (m_dir != (SliceDir)dir)
	{
		m_dir = (SliceDir)dir;

		if (m_dir == XOY)
		{
			m_sDim.set(dim.x, dim.y);
			m_sNormDim.set(m_vNormDim.x, m_vNormDim.y);
			m_elementIdx.set(0, 1);
		}
		else if (m_dir == XOZ)
		{
			m_sDim.set(dim.x, dim.z);
			m_sNormDim.set(m_vNormDim.x, m_vNormDim.z);
			m_elementIdx.set(0, 2);
		}
		else if (m_dir == YOZ)
		{
			m_sDim.set(dim.y, dim.z);
			m_sNormDim.set(m_vNormDim.y, m_vNormDim.z);
			m_elementIdx.set(1, 2);
		}
	}
	m_slice = slice;
	//printf("Show slilce [%s, %d]\n", (dir == XOY ? "XOY" : (dir == YOZ ? "YOZ" : "XOZ")), slice);

	int idx = 0;
	int vidx = 0;
	int gmidx = 0;

	unsigned char* data = new unsigned char[m_sDim.x * m_sDim.y * 3];
//	unsigned char* gdata_tmp = new unsigned char[m_sDim.x * m_sDim.y * 3];
	unsigned char* gmdata = new unsigned char[m_sDim.x * m_sDim.y];
	float* gdata = new float[m_sDim.x * m_sDim.y * 3];
//	QImage gmImg(m_sDim.x, m_sDim.y, QImage::Format_ARGB32);
//	unsigned char gm_tmp;

	if (m_dir == XOY)
	{
		memcpy(data, vdata + slice * dim.x * dim.y * 3, m_sDim.x * m_sDim.y * 3);
//		memcpy(gdata, m_gradient + slice * dim.x * dim.y * 3, m_sDim.x * m_sDim.y * 3 * sizeof(unsigned char));
		unsigned char* ptr = m_gradient + slice * dim.x * dim.y *3;
		for (int i = 0; i < m_sDim.y * m_sDim.x * 3; ++i)
		{
			gdata[i] = GRAD_UC2F(ptr[i]);
		}
		//unsigned char* gmdata = m_gradientMag + slice * dim.x * dim.y;//new unsigned char[m_sDim.x * m_sDim.y];
		memcpy(gmdata, m_gradientMag + slice * dim.x * dim.y, m_sDim.x * m_sDim.y * sizeof(unsigned char));
// 		for (int i = 0; i < m_sDim.y; ++i)
// 		{
// 			for (int j = 0; j < m_sDim.x; ++j)
// 			{
// 				vidx = i * m_sDim.x + j;
// 				gm_tmp = gmdata[vidx];
// 				gmImg.setPixel(j, i, qRgb(gm_tmp, gm_tmp, gm_tmp));
// 			}
// 		}
		//delete []gmdata;
	}
	else if (m_dir == XOZ)
	{
		for (int i = 0; i < m_sDim.y; ++i)
		{
			for (int j = 0; j < m_sDim.x; ++j)
			{
				vidx = (i * dim.x * dim.y + slice * dim.x + j) * 3;
				data[idx + 0] = vdata[vidx + 0];
				data[idx + 1] = vdata[vidx + 1];
				data[idx + 2] = vdata[vidx + 2];
				gdata[idx + 0] = GRAD_UC2F(m_gradient[vidx + 0]);
				gdata[idx + 1] = GRAD_UC2F(m_gradient[vidx + 1]);
				gdata[idx + 2] = GRAD_UC2F(m_gradient[vidx + 2]);
				gmdata[gmidx++] = m_gradientMag[vidx / 3];
// 				gm_tmp = m_gradientMag[vidx / 3];
// 				gmImg.setPixel(j, i, qRgb(gm_tmp, gm_tmp, gm_tmp));
				idx += 3;
			}
		}
	}
	else if (m_dir == YOZ)
	{
		for (int i = 0; i < m_sDim.y; ++i)
		{
			for (int j = 0; j < m_sDim.x; ++j)
			{
				vidx = (i * dim.x * dim.y + j * dim.x + slice) * 3;
				data[idx + 0] = vdata[vidx + 0];
				data[idx + 1] = vdata[vidx + 1];
				data[idx + 2] = vdata[vidx + 2];
				gdata[idx + 0] = GRAD_UC2F(m_gradient[vidx + 0]);
				gdata[idx + 1] = GRAD_UC2F(m_gradient[vidx + 1]);
				gdata[idx + 2] = GRAD_UC2F(m_gradient[vidx + 2]);
				gmdata[gmidx++] = m_gradientMag[vidx / 3];
// 				gm_tmp = m_gradientMag[vidx / 3];
// 				gmImg.setPixel(j, i, qRgb(gm_tmp, gm_tmp, gm_tmp));
				idx += 3;
			}
		}
	}

//	//将梯度转回浮点数
// 	for (unsigned char *ptrg = gdata, *pend = gdata + m_sDim.y * m_sDim.x + 1, *ptrgm = gmdata; ptrg < pend; ++ptrg)
// 	{
// 		*ptrg = (*ptrg / 255.0 * 2.0 - 1.0);
// 	}
//*
	int w = m_sDim.x * m_sNormDim.x;
	int h = m_sDim.y * m_sNormDim.y;
// 	int w = m_sDim.x;
// 	int h = m_sDim.y;
	if(m_pix == NULL || m_pix->width() != w || m_pix->height() != h)
	{
		SAFE_DELETE(m_pix);
		m_pix = new QImage(w, h, QImage::Format_ARGB32);
		SAFE_DELETE(m_gmpix);
		m_gmpix = new QImage(w, h, QImage::Format_ARGB32);
		SAFE_DELETE_ARRAY(m_gradMap);
		m_gradMap = new Vector2f[w * h];
	}
	m_pix->fill(Qt::black);
	//绘制体数据Slice
	// 从缩放后的坐标映射到原坐标，插值得到缩放后的值
	Vector2f origPos; //原坐标
	Vector2i ceilPos, floorPos;
	Vector3<unsigned char> d1,d2,d3,d4,rst,r1,r2; //原始图像数据，做插值用
	Vector3f g1,g2,g3,g4,gr1,gr2,grst; //
	Vector2f gdir;
	unsigned char gm1, gm2, gm3, gm4, gmr1, gmr2, gm_rst;
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			origPos.set(j / m_sNormDim.x, i / m_sNormDim.y);
			ceilPos.set(ceil(origPos.x), ceil(origPos.y));
			floorPos.set(floor(origPos.x), floor(origPos.y));
			if (ceilPos.x >= m_sDim.x)
				ceilPos.x = m_sDim.x - 1;
			if (ceilPos.y >= m_sDim.y)
				ceilPos.y = m_sDim.y - 1;

			if (ceilPos.x == floorPos.x && ceilPos.y == floorPos.y) //不需要插值
			{
				idx = (ceilPos.y  * m_sDim.x + ceilPos.x) * 3;
				rst.set(data[idx], data[idx + 1], data[idx + 2]);
				grst.set(gdata[idx], gdata[idx + 1], gdata[idx + 2]);
				gm_rst = gmdata[idx / 3];
			}
			else if (ceilPos.x == floorPos.x) //d1==d2, d3==d4,y方向线性插值
			{
				idx = (ceilPos.y  * m_sDim.x + ceilPos.x) * 3;//上
				r1.set(data[idx], data[idx + 1], data[idx + 2]);
				g1.set(gdata[idx], gdata[idx + 1], gdata[idx + 2]);
				gm1 = gmdata[idx / 3];
				idx = (floorPos.y * m_sDim.x + floorPos.x) * 3;//下
				r2.set(data[idx], data[idx + 1], data[idx + 2]);
				g2.set(gdata[idx], gdata[idx + 1], gdata[idx + 2]);
				gm2 = gmdata[idx / 3];
				rst = lerp(r2, r1, ceilPos.y - origPos.y);
				grst = lerp(g2, g1, ceilPos.y - origPos.y);
				gm_rst = lerp(gm2, gm1, ceilPos.y - origPos.y);
			}
			else if (ceilPos.y == floorPos.y) //d1==d3, d2==d4,x方向线性插值
			{
				idx = (ceilPos.y  * m_sDim.x + ceilPos.x) * 3;//右
				r1.set(data[idx], data[idx + 1], data[idx + 2]);
				g1.set(gdata[idx], gdata[idx + 1], gdata[idx + 2]);
				gm1 = gmdata[idx / 3];
				idx = (floorPos.y * m_sDim.x + floorPos.x) * 3;//左
				r2.set(data[idx], data[idx + 1], data[idx + 2]);
				g2.set(gdata[idx], gdata[idx + 1], gdata[idx + 2]);
				gm2 = gmdata[idx / 3];
				rst = lerp(r2, r1, ceilPos.x - origPos.x);
				grst = lerp(g2, g1, ceilPos.x - origPos.x);
				gm_rst = lerp(gm2, gm1, ceilPos.x - origPos.x);
			}
			else //双线性插值
			{
				idx = (ceilPos.y  * m_sDim.x + floorPos.x) * 3;//左上
				d1.set(data[idx], data[idx + 1], data[idx + 2]);
				g1.set(gdata[idx], gdata[idx + 1], gdata[idx + 2]);
				gm1 = gmdata[idx / 3];
				idx = (ceilPos.y  * m_sDim.x +  ceilPos.x) * 3;//右上
				d2.set(data[idx], data[idx + 1], data[idx + 2]);
				g2.set(gdata[idx], gdata[idx + 1], gdata[idx + 2]);
				gm2 = gmdata[idx / 3];
				idx = (floorPos.y * m_sDim.x + floorPos.x) * 3;//左下
				d3.set(data[idx], data[idx + 1], data[idx + 2]);
				g3.set(gdata[idx], gdata[idx + 1], gdata[idx + 2]);
				gm3 = gmdata[idx / 3];
				idx = (floorPos.y * m_sDim.x +  ceilPos.x) * 3;//右下
				d4.set(data[idx], data[idx + 1], data[idx + 2]);
				g4.set(gdata[idx], gdata[idx + 1], gdata[idx + 2]);
				gm4 = gmdata[idx / 3];
				r1 = lerp(d2, d1, ceilPos.x - origPos.x);
				r2 = lerp(d4, d3, ceilPos.x - origPos.x);
				rst = lerp(r2, r1, ceilPos.y - origPos.y);
				gr1 = lerp(g2, g1, ceilPos.x - origPos.x);
				gr2 = lerp(g4, g3, ceilPos.x - origPos.x);
				grst = lerp(gr2, gr1, ceilPos.y - origPos.y);
				gmr1 = lerp(gm2, gm1, ceilPos.x - origPos.x);
				gmr2 = lerp(gm4, gm3, ceilPos.x - origPos.x);
				gm_rst = lerp(gmr2, gmr1, ceilPos.y - origPos.y);
			}
			m_pix->setPixel(j, i, qRgb(rst.x, rst.y, rst.z));
// 			float gm_tmp = grst.norm() / m_maxgm;
// 			if (gm_tmp > 1.0)
// 			{
// 				printf("[%s][%s][Line:%d]shit!@! > %f\n", __FILE__, __FUNCTION__, __LINE__,gm_tmp);
// 			}
// 			//gm_tmp = log(gm_tmp);
// 			unsigned char gray = gm_tmp* 255;
// 			//gray = log(float(gray));
			m_gmpix->setPixel(j, i, qRgb(gm_rst, gm_rst, gm_rst));

			// 采样得到的梯度从三维映射到二维
			gdir.set(grst[m_elementIdx.x], grst[m_elementIdx.y]);
			if (gdir.x != 0 || gdir.y != 0)
				gdir = gdir.normalize() * gm_rst / 255.0; //投影在Slice平面上的方向*原梯度模
			m_gradMap[i * w + j] = gdir;
		}
	}
	delete[] data;
	delete[] gdata;
	delete[] gmdata;
//*
// 	//绘制梯度
// 	{
// 		QPainter p(m_pix);
// 		p.setRenderHint(QPainter::Antialiasing);
// 		QPointF p1,p2;
// 		idx = 0;
// 		for (int i = 0; i < h; i += 6)
// 		{
// 			for (int j = 0; j < w; j += 6)
// 			{
// 				idx = i * w + j;
// 				gdir = m_gradMap[idx];
// 
// 				//if (g.norm() > 0.5 && (gdir.x != 0 || gdir.y != 0))
// 				if(gdir.norm() > 0.1)
// 				{
// 					p1.setX(j);
// 					p1.setY(i);
// 					p2.setX(j + gdir.x * m_gradArrowLength);
// 					p2.setY(i + gdir.y * m_gradArrowLength);
// 					//printf("%f,%f -> %f,%f\n", p1.x(), p1.y(), p2.x(), p2.y());
// 					//p.setPen(QColor(0, 60, 196, 128));
// 					drawArrow(&p, p1, p2);
// 				}
// 			}
// 		}
// 	}
//*/	

	update();
}

void SliceGradientRender::drawArrow( QPainter *p, const QPointF& p1, const QPointF& p2 )
{
	QPen pen1(Qt::blue, m_lineWidth);
// 	QPen pen2(Qt::red, m_lineWidth * 2);
// 	p->setPen(pen1);
// 	p->drawLine(p1, p2);
// 	p->setPen(pen2);
// 	p->drawPoint(p2);

	QPointF v = p2 - p1;
	float len = m_gradArrowLength;//sqrt(v.x() * v.x() + v.y() * v.y());
	QPointF vn(v.x() / len, v.y() / len);
	p->setPen(pen1);
	p->translate(p1);
	p->rotate((v.y() < 0 ? -1 : 1) * acos(vn.x()) * 180.0 / M_PI);
	p->drawLine(0, 0, len, 0);
	p->drawLine(len, 0, len - 5 * cos(45 * M_PI / 180), 5 * sin(45 * M_PI / 180));
	p->drawLine(len, 0, len - 5 * cos(45 * M_PI / 180), - 5 * sin(45 * M_PI / 180));
	p->resetTransform();
}

void SliceGradientRender::contextMenuEvent( QContextMenuEvent *e )
{
	QCursor cur=this->cursor();
	QMenu menu(this);
	menu.addAction(m_actSaveImg);
	menu.addAction(m_actScreenshot);
	menu.exec(cur.pos());
}

void SliceGradientRender::saveImage()
{
	QString fn = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled.png", tr("Images (*.png)"));
	m_pix->save(fn, "PNG");
	printf("Saved slice image to [%s]\n", fn.toLocal8Bit().data());
}

void SliceGradientRender::screenshot()
{
	QString fn = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled.png", tr("Images (*.png)"));
	QPixmap pix(width(), height());
	draw(&pix);
	pix.save(fn, "PNG");
	printf("Saved slice image to [%s]\n", fn.toLocal8Bit().data());
}

void SliceGradientRender::on_sbSlice_valueChanged( int i )
{
	showSlice(m_dir, i);
}

void SliceGradientRender::on_cbDir_currentIndexChanged( int i )
{
	int maxv;
	Vector3i dim = m_volume->getDimension();
	if (i == 2) // XY
	{
		maxv = dim.z;
	}
	else if (i == 1) // XZ
	{
		maxv = dim.y;
	}
	else if (i == 0) // YZ
	{
		maxv = dim.x;
	}

	if (ui.sbSlice->value() >= maxv)
	{
		ui.sbSlice->setValue(maxv - 1);
	}
	ui.sbSlice->setMaximum(maxv - 1);
	showSlice(i, ui.sbSlice->value());
}

void SliceGradientRender::on_sbLineWidth_valueChanged( int i )
{
	if (m_lineWidth != i)
	{
		m_lineWidth = i;
		update();
	}
}

void SliceGradientRender::on_sbGradFilter_min_valueChanged( double d )
{
	if (m_gradFilter.x != d)
	{
		m_gradFilter.x = (float)d;
		update();
	}
}

void SliceGradientRender::on_sbGradFilter_max_valueChanged( double d )
{
	if (m_gradFilter.y != d)
	{
		m_gradFilter.y = (float)d;
		update();
	}
}

void SliceGradientRender::on_cbShowGM_stateChanged( int state )
{
	m_showGM = state;
	update();
}

void SliceGradientRender::on_sbLineLength_valueChanged( int i )
{
	if (m_gradArrowLength != i)
	{
		m_gradArrowLength = i;
		update();
	}
}

void SliceGradientRender::on_cbShowImage_stateChanged( int state )
{
	m_showImage = state;
	update();
}

void SliceGradientRender::on_cbShowVector_stateChanged( int state )
{
	m_showVector = state;
	update();
}
