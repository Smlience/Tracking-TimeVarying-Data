#include "ColorBar.h"
#include "util/util.h"
#include <QPainter>

ColorBar::ColorBar(QWidget * parent)
	: QWidget(parent), m_color(NULL), m_idxMap(NULL), m_colorNum(0), m_paddingH(0), m_paddingV(2), m_binW(5), m_binH(25)
{
 	setMouseTracking(true);
}

ColorBar::~ColorBar()
{
	SAFE_DELETE_ARRAY(m_color);
	SAFE_DELETE_ARRAY(m_idxMap);
}

void ColorBar::setColor( unsigned char* data, int num, int BPP )
{
	SAFE_DELETE_ARRAY(m_color);
	SAFE_DELETE_ARRAY(m_idxMap);
	m_colorNum = num;
	m_color = new QColor[num];
	m_idxMap = new int[num];
	for (int i = 0; i < num; ++i)
	{
		m_color[i].setRgb(data[i * BPP], data[i * BPP + 1], data[i * BPP + 2]);
		m_idxMap[i] = i;
	}
}

void ColorBar::paintEvent( QPaintEvent * event )
{
	event->accept();
	if (m_color != NULL)
	{
		QImage colorBar(m_colorNum * m_binW, m_binH, QImage::Format_RGB32);

		QPainter p(&colorBar);
		p.setPen(Qt::NoPen);
		if(m_idxMap != NULL)
		{
			for (int i = 0; i < m_colorNum; ++i)
			{
				p.fillRect(i * m_binW, 0, m_binW, m_binH, m_color[m_idxMap[i]]);
			}
		}
		p.end();

		QPainter painter(this);
		painter.drawImage(rect(), colorBar);
		painter.setPen(Qt::white);
		for (int i = 0; i < m_selected.size(); ++i)
		{
			int idx = m_selected[i].first;
			int span = m_selected[i].second;
			painter.drawRect(float(idx) / m_colorNum * width(), 0, float(span * width()) / m_colorNum, m_binH - 1);
		}
	}
}

void ColorBar::mousePressEvent( QMouseEvent * e )
{
	emit mousePressedAt(e->x());
	setSelected(colorIndexAt(e->x()));
	update();
}

int ColorBar::colorIndexAt( int x )
{
	int idx = int((float(x) / width()) * 255 + 0.5);
	return idx;
}

int ColorBar::colorMappedIndexAt( int x )
{
	int idx = int((float(x) / width()) * 255 + 0.5);
	if (m_idxMap != NULL)
	{
		idx = m_idxMap[idx];
	}
	return idx;
}

void ColorBar::setSelected( int idx, int span /*= 1*/ )
{
	m_selected.clear();
	m_selected.push_back(std::make_pair(idx, span));
}

void ColorBar::setIndexMap( const int* idxMap )
{
	 for (int i = 0; i < m_colorNum; ++i)
	 {
		 m_idxMap[i] = idxMap[i];
	 }
}

const int* ColorBar::getIndexMap()
{
	return m_idxMap;
}
