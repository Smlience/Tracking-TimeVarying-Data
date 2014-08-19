#include "ScatterPlotWidget.h"
#include "ScatterPlot.h"
#include "CanvasBrushPicker.h"
#include "CanvasPolygonPicker.h"
#include "util\func.math.h"
#include "util\util.h"
#include <QPainter>
#include <QMouseEvent>
#include <limits>

ScatterPlotWidget::ScatterPlotWidget( QWidget *parent )
	: QWidget(parent)//, m_xRange(0, 256), m_yRange(0, 256), m_partitions(8, 8), m_samples(NULL)//, m_histogram(NULL), m_histoImg(NULL), m_imgType(0)
	, m_tfData(NULL)
{
	this->setMinimumSize(260, 260);

	m_plot = new ScatterPlot(this);
// 	m_picker = new CanvasBrushPicker(m_plot);
	m_picker = new CanvasPolygonPicker(m_plot);
	m_picker->createToolbar(this);

	connect(m_picker, SIGNAL(canvasChanged()), this, SLOT(emitTFData()));
}

ScatterPlotWidget::~ScatterPlotWidget()
{
	SAFE_DELETE_ARRAY(m_tfData);
}

void ScatterPlotWidget::resizeEvent( QResizeEvent *e )
{
	m_plot->resize(e->size());
	m_picker->resize(m_plot->plotSize());
	m_picker->move(m_plot->margins().left(), m_plot->margins().top());
	m_picker->set_default_tf(40);
	emitTFData();
}

void ScatterPlotWidget::setData( const unsigned char *v1, std::pair<int,int> v1Range, const unsigned char*v2, std::pair<int,int> v2Range, Vector3i dim )
{
	m_plot->setData(v1, v1Range, v2, v2Range, dim);

	SAFE_DELETE_ARRAY(m_tfData);
	Vector2i dataDim = m_plot->dataDimension();
	m_tfData = new unsigned char[dataDim.x *  dataDim.y * 4];
	memset(m_tfData, 0, dataDim.x *  dataDim.y * sizeof(unsigned int));
	update();
}

void ScatterPlotWidget::emitTFData()
{
	Vector2i dim = m_plot->dataDimension();
	QImage tf = m_picker->result().scaled(dim.x, dim.y);
//	tf.save("a.png", "PNG");
	if (m_tfData != NULL)
	{
		// 从交互图像产生TF数据
		int idx = 0;
		QRgb c;
		for (int j = 0;  j < dim.y;++j)
		{
			for (int i = 0; i < dim.x; ++i)
			{
				c = tf.pixel(i, dim.y - j - 1);
				//c = tf.pixel(i, j);
				m_tfData[idx++] = qRed(c);
				m_tfData[idx++] = qGreen(c);
				m_tfData[idx++] = qBlue(c);
				m_tfData[idx++] = qAlpha(c);
			}
		}
		emit tfChanged(m_tfData, dim);
	}
}