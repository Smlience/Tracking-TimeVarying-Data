#include "ColorTF2DCanvas.h"
#include "dm\Volume.h"
#include "ClusteredColorTF.h"
#include "ColorCluster.h"
#include "util\util.h"

ColorTF2DCanvas::ColorTF2DCanvas(QWidget *parent)
	: QWidget(parent),m_clusters(NULL)
{
	setMouseTracking(true);

	m_tf = new ClusteredColorTF();
}

ColorTF2DCanvas::~ColorTF2DCanvas()
{
	SAFE_DELETE(m_tf);
}

QSize ColorTF2DCanvas::minimumSizeHint() const
{
	return QSize(300,100);
}

QSize ColorTF2DCanvas::sizeHint() const
{
	return QSize(300, 100);
}

QSizePolicy ColorTF2DCanvas::sizePolicy() const
{
	return QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void ColorTF2DCanvas::clearTF()
{
	printf("Clear TF\n");
}

void ColorTF2DCanvas::setVolume( const Volume* vol )
{
	printf("TF2D Got Data!\n");
	clearTF();
	m_tf->setVolume(vol);
	m_clusters = m_tf->getClusterInfo();
	segment();
	setupTransfunc();
	emit sendClusterInfo(m_tf->clusters());
}

void ColorTF2DCanvas::paintEvent( QPaintEvent* event )
{
	QPainter p(this);
	float ps = 1.0;
	QPen pen;
	pen.setColor(Qt::red);
	
// 	p.setPen(Qt::NoPen);
// 	QBrush brush(QColor(200, 30,128, 128));
// 	p.setBrush(brush);
// 	std::list<QPoint>::iterator it = m_vPoint.begin();
// 	for (; it != m_vPoint.end(); ++it)
// 	{
// // 		pen.setWidthF(ps);
// // 		p.setPen(pen);
// 		p.drawEllipse(QPointF(*it), ps, ps);
// 		ps += 0.1;
// 	}
	
// 	p.setPen(QPen(Qt::blue,0,Qt::SolidLine));
// 	p.setBrush(QBrush(QColor(122,255,128,126)));
// 	p.drawRect(10,10,20,30);

	if(m_clusters != NULL)
	{
		pen.setWidth(3);
		for (int i = 0; i < 256; ++i)
		{
			pen.setColor(QColor(m_clusters[i].color.r, m_clusters[i].color.g, m_clusters[i].color.b));
			p.setPen(pen);
			p.drawPoint(m_pos[i]);
			//p.drawPoint(m_clusters[i].hsv.x * 360, m_clusters[i].lab.x);
			//p.drawPoint(m_clusters[i].hsv.x * 360, m_clusters[i].lab.z + 128);
			//p.drawPoint(m_clusters[i].lab.x, m_clusters[i].lab.z + 128);
		}
	}
}

void ColorTF2DCanvas::mousePressEvent( QMouseEvent* event )
{

}

void ColorTF2DCanvas::mouseReleaseEvent( QMouseEvent* event )
{

}

void ColorTF2DCanvas::mouseMoveEvent( QMouseEvent* event )
{
	m_vPoint.push_front(event->pos());
	if (m_vPoint.size() > 100)
	{
		m_vPoint.pop_back();
	}
	update();
}

void ColorTF2DCanvas::keyPressEvent( QKeyEvent* event )
{

}

void ColorTF2DCanvas::keyReleaseEvent( QKeyEvent* event )
{

}

bool ColorTF2DCanvas::eventFilter( QObject *obj, QEvent *e )
{
	if (e->type() == QEvent::MouseMove)
	{
		mouseMoveEvent(static_cast<QMouseEvent*>(e));
	}

	// pass the event on to the parent class
	return QWidget::eventFilter(obj, e);
}

void ColorTF2DCanvas::setupTransfunc()
{
	unsigned char *tfData = m_tf->tfData();
	for (int x = 0; x < 256; ++x)
	{
		//tfData[x*4] = temp.r;
		//tfData[x*4+1] = temp.g;
		//tfData[x*4+2] = temp.b;
		tfData[x*4+3] = 2;
	}
	emit sendTF(tfData);
}

void ColorTF2DCanvas::segment()
{
	bool flag[361];
	int idx1, idx2;
	int span;
	memset(flag, 0, sizeof(bool) * 361);
	for (int i = 0; i < 256; ++i)
	{
		//idx1 = int(m_clusters[i].hsv.x * 360 + 0.5);
		idx1 = int(m_clusters[i].lab.x * 360 + 0.5);  ///////////////////////// HSV removed from struct Cluster
		if (!flag[idx1])
		{
			flag[idx1] = true;
		}
	}
	for (int i = 0; i < 361; ++i)
	{
		//printf("%d", flag[i]);
		if (flag[i])
		{
			if(span == 0)//开始一个区段
			{
				idx1 = i;
			}
			idx2 = i;
			++span;		 
		}
		else if (span != 0 || (i == 360 && idx1 != 0)) //结束一个区段
		{
			m_segment.push_back(std::make_pair(idx1, idx2));
			span = 0;
			idx1 = 0;
			idx2 = 0;
		}
	}
//	printf("\n");

	std::list< std::pair<int, int> >::iterator it = m_segment.begin();
// 	for (int i = 0; it != m_segment.end(); ++it, ++i)
// 	{
// 		printf("S[%d]:[%d,%d]\n", i, it->first, it->second);
// 	}
	std::list<int> segSpan;
	padding = 0;
	it = m_segment.begin();
	int preRight = it->second;
	for (++it; it != m_segment.end(); ++it)
	{
		span = it->first - preRight - 1;
		//找到最大的间隔
		if (padding < span)
		{
			padding = span;
			idx1 = preRight;
			idx2 = it->first;
		}

		segSpan.push_back(span);
		preRight = it->second;
	}
	--it;
	span = m_segment.begin()->first + 360 - it->second - 1;
	segSpan.push_back(span);
	if (padding < span)
	{
		padding = 0;
		idx1 = it->second;
		idx2 = m_segment.begin()->first;
	}
//	printf("max span = %d, idx1=%d, idx2=%d\n", padding,idx1,idx2);
	padding = 360 - idx2/* + 1*/;

// 	for (std::list<int>::iterator itSegSpan = segSpan.begin(); itSegSpan != segSpan.end(); ++itSegSpan)
// 	{
// 		printf("span:%d", *itSegSpan);
// 	}
//	printf("padding = %d\n", padding);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 256; ++i)
	{
		//m_pos[i].setX((int(m_clusters[i].hsv.x * 360) + padding) % 360);
		m_pos[i].setX((int(m_clusters[i].lab.x * 360) + padding) % 360); ///////////////// HSV removed!!
		m_pos[i].setY(m_clusters[i].lab.x);
	}
}
