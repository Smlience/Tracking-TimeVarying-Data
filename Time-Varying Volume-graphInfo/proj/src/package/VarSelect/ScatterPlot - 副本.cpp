#include "ScatterPlot.h"
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QColorDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include "util\func.math.h"
#include "CanvasPicker.h"
#include "util\util.h"
#include <limits>

ScatterPlot::ScatterPlot( QWidget *parent )
	: m_xRange(0, 256), m_yRange(0, 256), m_partitions(8, 8), m_samples(NULL), m_histogram(NULL), m_histoImg(NULL), m_imgType(0)
	, m_tfData(NULL)
{
	this->setMinimumSize(260, 260);
	m_picker = new CanvasPicker(this);
	connect(m_picker, SIGNAL(tfChanged()), this, SLOT(emitTFData()));

	//////////////////////////////////////////////////////////////////////////
	toolbar = new QWidget(this);

	btnColor = new QPushButton(toolbar);
	QIcon icon_color(QStringLiteral(":/MainWin/default.png"));
	btnColor->setIcon(icon_color);
	btnColor->setIconSize(QSize(16, 16));
	btnColor->setFixedSize(20, 20);
	btnColor->setToolTip("Select Brush Color");
	connect(btnColor, SIGNAL(clicked()), this, SLOT(selectBrushColor()));

	btnFill = new QPushButton(toolbar);
	QIcon icon_brush(QStringLiteral(":/MainWin/default.png"));
	btnFill->setIcon(icon_brush);
	btnFill->setIconSize(QSize(16, 16));
	btnFill->setFixedSize(20, 20);
	btnFill->setToolTip("Brush");
	connect(btnFill, SIGNAL(clicked()), this, SLOT(changeBrushToFill()));

	btnOpacityPlus = new QPushButton(toolbar);
	QIcon icon_opacityplus(QStringLiteral(":/MainWin/default.png"));
	btnOpacityPlus->setIcon(icon_opacityplus);
	btnOpacityPlus->setIconSize(QSize(16, 16));
	btnOpacityPlus->setFixedSize(20, 20);
	btnOpacityPlus->setToolTip("Increase Opacity");
	connect(btnOpacityPlus, SIGNAL(clicked()), this, SLOT(changeBrushToOpactiyIncrease()));

	btnOpacitySub = new QPushButton(toolbar);
	QIcon icon_opacitysub(QStringLiteral(":/MainWin/default.png"));
	btnOpacitySub->setIcon(icon_opacitysub);
	btnOpacitySub->setIconSize(QSize(16, 16));
	btnOpacitySub->setFixedSize(20, 20);
	btnOpacitySub->setToolTip("Decrease Opacity");
	connect(btnOpacitySub, SIGNAL(clicked()), this, SLOT(changeBrushToOpactiyDecrease()));

	btnErase = new QPushButton(toolbar);
	QIcon icon_erase(QStringLiteral(":/MainWin/default.png"));
	btnErase->setIcon(icon_erase);
	btnErase->setIconSize(QSize(16, 16));
	btnErase->setFixedSize(20, 20);
	btnErase->setToolTip("Erase");
	connect(btnErase, SIGNAL(clicked()), this, SLOT(changeBrushToErase()));

	btnClear = new QPushButton(toolbar);
	QIcon icon_clear(QStringLiteral(":/MainWin/default.png"));
	btnClear->setIcon(icon_clear);
	btnClear->setIconSize(QSize(16, 16));
	btnClear->setFixedSize(20, 20);
	btnClear->setToolTip("Clear");
	connect(btnClear, SIGNAL(clicked()), this, SLOT(clearCanvas()));

	QVBoxLayout *vLayout = new QVBoxLayout(toolbar);
	vLayout->setSpacing(2);
	vLayout->setContentsMargins(0, 0, 0, 0);
	vLayout->addWidget(btnColor);
	vLayout->addWidget(btnFill);
	vLayout->addWidget(btnOpacityPlus);
	vLayout->addWidget(btnOpacitySub);
	vLayout->addWidget(btnErase);
	vLayout->addWidget(btnClear);
	toolbar->resize(20, vLayout->count() * (20 + vLayout->spacing()) - vLayout->spacing());
	//////////////////////////////////////////////////////////////////////////

	//m_picker->setWindowOpacity(0.);
	m_xLabel = tr("Scalar");
	m_yLabel = tr("Gradient");
	calcCanvasRect();
}

ScatterPlot::~ScatterPlot()
{
	SAFE_DELETE_ARRAY(m_histogram);
	SAFE_DELETE_ARRAY(m_tfData);
	SAFE_DELETE(m_histoImg);
}

void ScatterPlot::paintEvent( QPaintEvent *e )
{
	QPainter p(this);
	QFontMetrics metrics = p.fontMetrics();
	int textHeight = metrics.height();
	float deltaX = m_size.width() / float(m_partitions.first); // x坐标上每分的宽度
	float deltaY = m_size.height() / float(m_partitions.second); // y坐标上每分的宽度

	// 移动坐标系
	p.translate(m_margins.left(), m_margins.top() + m_size.height());

	// 绘制外框
	p.drawRect(0, 0, m_size.width(), -m_size.height());
	if(true) // 绘制坐标轴及刻度
	{
		// 画横坐标
		//p.drawLine(0, 0, w, 0);
		//p.drawText(deltaX * spanX + metrics.width(xLabel) / 2, textHeight, xLabel);
		p.drawText((m_size.width() - metrics.width(m_xLabel)) / 2, textHeight * 2.25, m_xLabel);
		for (int i = 1; i <= m_partitions.first; ++i)
		{
			// 绘制坐标刻度
			p.drawLine(deltaX * i, 0, deltaX * i, 4);

			// 绘制坐标刻度值
			QString mark = QString::number(i * m_xRange.second / m_partitions.first);
			int stringWidth = metrics.width(mark);
			p.drawText(deltaX * i - stringWidth / 2, textHeight * 1.25, mark);
		}

		// 画纵坐标
		//p.drawLine(0, 0, 0, -h);
		//p.drawText(-metrics.width(yLabel) / 2, -(deltaY * spanY + textHeight), yLabel);
		p.rotate(-90);
		p.drawText((m_size.height() - metrics.width(m_yLabel)) / 2, -textHeight * 2.25, m_yLabel);
		p.rotate(90);
		for (int i = 1; i <= m_partitions.second; ++i)
		{
			// 绘制坐标刻度
			p.drawLine(-4, -i * deltaY, 0, -i * deltaY);

			// 绘制坐标值
			QString mark = QString::number(i * m_yRange.second / m_partitions.second);
			int stringWidth = metrics.width(mark);
			p.drawText(-stringWidth - 4, -(deltaY * i + textHeight / 2 - metrics.ascent()), mark);
		}
	}
	else
	{
		p.drawText((m_size.width() - metrics.width(m_xLabel)) / 2, textHeight * 1.25, m_xLabel);
		p.rotate(-90);
		p.drawText((m_size.height() - metrics.width(m_yLabel)) / 2, -textHeight * 0.5, m_yLabel);
		p.rotate(90);
	}

// 	if (m_samples != NULL)
// 	{
// 		p.scale(1.0, -1.0);
// 		for (QVector<QPointF>::const_iterator i = m_samples->begin(); i != m_samples->end(); i++)
// 		{
// 			p.drawPoint(i->x(), i->y() + m_margins.bottom());
// 		}
// 	}
	if (m_histoImg != NULL)
	{
		QRect target(0, -m_size.height(), m_size.width(), m_size.height());
		QRect source(0, 0, -1, -1);
// 		QImage m_canvas = *m_histoImg;//(m_size.width(), m_size.height(), QImage::Format_ARGB32);
// 		QRgb c;
// 		for(int i = 0; i < m_canvas.width(); ++i)
// 		{
// 			for (int j = 0; j < m_canvas.height(); ++j)
// 			{
// 				c = m_picker->colorAt(i, j);
// 				//将TF颜色与直方图混合
// 				int h = qRed(m_canvas.pixel(i, j));
// 				int a = qAlpha(c);
// 				int r = qRed(c) * a + (1 - a) * h;
// 				int g = qGreen(c) * a + (1 - a) * h;
// 				int b = qBlue(c) * a + (1 - a) * h;
// 				m_canvas.setPixel(i, j, qRgb(r, g, b));
// 			}
// 		}
// 		p.drawImage(target, m_canvas, source);
		p.setCompositionMode(QPainter::CompositionMode_Multiply);
		p.drawImage(target, *m_histoImg, source);
		p.drawImage(target, m_picker->mask(), source);
	}
}

void ScatterPlot::mouseMoveEvent( QMouseEvent *e )
{
	int x = e->pos().x();
	int y = e->pos().y();
	if (x >= m_margins.left() && x <= width() - m_margins.right()
		&& y >= m_margins.bottom() && y <= height() - m_margins.top())
	{
		x = (e->pos().x() - m_margins.left()) / float(m_size.width()) * (m_xRange.second - m_xRange.first) + m_xRange.first;
		y = (m_size.height() + m_margins.bottom() - e->pos().y()) / float(m_size.height()) * (m_yRange.second - m_yRange.first) + m_yRange.first;
		QToolTip::showText(e->globalPos(), QString::number(x) + ", " +	QString::number(y), this, rect());
	}
	QWidget::mouseMoveEvent(e);
}

void ScatterPlot::setSamples( const QVector<QPointF> &samples )
{
	m_samples = &samples;
}

QPointF ScatterPlot::transform(const QPointF& in)
{
	QPointF r;
	return in;
}

QPointF ScatterPlot::invTransform(const QPointF& in)
{
	QPointF r;
	return r;
}

void ScatterPlot::resizeEvent( QResizeEvent *e )
{
	calcCanvasRect();
}

void ScatterPlot::calcCanvasRect()
{
//	QPainter p(this);
	QFontMetrics metrics = this->fontMetrics();
	int textHeight = metrics.height();

	m_margins.setTop(textHeight);
	m_margins.setBottom(textHeight * 3);
	m_margins.setLeft(metrics.width(QString::number(m_yRange.second)) + textHeight * 2);
	m_margins.setRight(textHeight + metrics.width(QString::number(m_xRange.second)) / 2);
	m_size.setWidth(this->width() - m_margins.left() - m_margins.right());
	m_size.setHeight(this->height() - m_margins.top() - m_margins.bottom());

	m_picker->resize(m_size);
	m_picker->move(m_margins.left(), m_margins.top());

	m_matrix.reset();
	m_matrix.translate(m_margins.left(), static_cast<float>(height())-1);
	m_matrix.scale(1.f, -1.f);
}

void ScatterPlot::setData( unsigned char *v1, std::pair<int,int> v1Range, unsigned char*v2, std::pair<int,int> v2Range, Vector3i dim )
{
	m_xRange = v1Range;
	m_yRange = v2Range;
	m_dim.x = v1Range.second - v1Range.first + 1;
	m_dim.y = v2Range.second - v2Range.first + 1;
// 	int d1 = v1Range.second - v1Range.first + 1;
// 	int d2 = v2Range.second - v2Range.first + 1;
	SAFE_DELETE_ARRAY(m_histogram);
	m_histogram = new unsigned int[m_dim.x *  m_dim.y];
	SAFE_DELETE_ARRAY(m_tfData);
	m_tfData = new unsigned char[m_dim.x *  m_dim.y * 4];
	memset(m_histogram, 0, m_dim.x *  m_dim.y * sizeof(unsigned int));
	unsigned int len = dim.x * dim.y * dim.z;
	for (unsigned int idx = 0; idx < len; ++idx)
	{
		unsigned char p1 = v1[idx] - v1Range.first;
		unsigned char p2 = v2[idx] - v2Range.first;
		m_histogram[p2 * m_dim.x + p1] ++;
	}
	m_histoMax = (std::numeric_limits<unsigned int>::min)();
	len = m_dim.x * m_dim.y;
	for (unsigned int idx = 0; idx < len; ++idx)
	{
		if (m_histoMax < m_histogram[idx])
		{
			m_histoMax = m_histogram[idx];
		}
	}
	drawHistograme();
	update();
}

void ScatterPlot::drawHistograme()
{
	if (m_histoImg == NULL || m_dim.x != m_histoImg->width() || m_dim.y != m_histoImg->height())
	{
		SAFE_DELETE(m_histoImg);
		m_histoImg = new QImage(m_dim.x, m_dim.y, QImage::Format_ARGB32);
	}
	m_histoImg->fill(Qt::transparent);

	QPainter p(m_histoImg);
	int len = m_dim.x * m_dim.y;
	int i, j, idx;
	for(j = idx = 0; j < m_dim.y; ++j)
	{
		for (i = 0; i < m_dim.x; ++i, ++idx)
		{
			//idx = j * m_dim.x + i
			int opacity;
			if (m_imgType == 1) // Scatter plot, 全白或全黑
			{
				opacity = (m_histogram[idx] > 0) ? 255 : 0;				
			}
			else
			{
				opacity = log(float(m_histogram[idx])) / log(float(m_histoMax)) * 255; //直方图量化
			}
			//QRgb c = m_picker->colorAt(i, d2 - j);
			//m_histoImg->setPixel(i, d2 - j, c);
			m_histoImg->setPixel(i, m_dim.y - j - 1, qRgb(opacity, opacity, opacity));//d2 - j 用于解决QT坐标系原点在左上的问题
		}
	}
}

void ScatterPlot::emitTFData()
{
	QImage tf = m_picker->mask().scaled(m_dim.x, m_dim.y);
//	tf.save("a.png", "PNG");
	if (m_tfData != NULL)
	{
		// 从交互图像产生TF数据
		int idx = 0;
		QRgb c;
		for (int j = 0;  j < m_dim.y;++j)
		{
			for (int i = 0; i < m_dim.x; ++i)
			{
				c = tf.pixel(i, m_dim.y - j - 1);
				//c = tf.pixel(i, j);
				m_tfData[idx++] = qRed(c);
				m_tfData[idx++] = qGreen(c);
				m_tfData[idx++] = qBlue(c);
				m_tfData[idx++] = qAlpha(c);
			}
		}
		emit tfChanged(m_tfData, m_dim);
	}
}

void ScatterPlot::selectBrushColor()
{
	QColor c = QColorDialog::getColor(m_picker->getPenColor(), this, "Select brush color");
	m_picker->setPenColor(c);
}

void ScatterPlot::changeBrushToOpactiyIncrease()
{
	m_picker->setBrushType(CanvasPicker::BrushTypeOpacityIncrease);
}

void ScatterPlot::changeBrushToOpactiyDecrease()
{
	m_picker->setBrushType(CanvasPicker::BrushTypeOpacityDecrease);
}

void ScatterPlot::changeBrushToErase()
{
	m_picker->setBrushType(CanvasPicker::BrushTypeErase);
}

void ScatterPlot::changeBrushToFill()
{
	m_picker->setBrushType(CanvasPicker::BrushTypeFill);
}

void ScatterPlot::clearCanvas()
{
	m_picker->clearCanvas();
}
