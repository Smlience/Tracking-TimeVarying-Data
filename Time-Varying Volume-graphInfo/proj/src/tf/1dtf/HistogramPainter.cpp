#include "HistogramPainter.h"
#include "IntensityHistogram.h"

#include <QPainter>
#include <QPaintEvent>

HistogramPainter::HistogramPainter(QWidget* parent, Vector2f xRange, Vector2f yRange, int leftSpace, int rightSpace,
	int bottomSpace, int padding, int arrowLength)
	: QWidget(parent)
	, m_xRange(xRange)
	, m_yRange(yRange)
	, m_iPadding(padding)
	, m_iLeftSpace(leftSpace)
	, m_iRightSpace(rightSpace)
	, m_iBottomSpace(bottomSpace)
	, m_iArrowLength(arrowLength)
	, m_pHistogram(0)
	, m_pCache(0)
	, m_iHisstyle(0)
{
	setMouseTracking(true);
}

HistogramPainter::~HistogramPainter()
{
	delete m_pCache;
}

void HistogramPainter::setHistogram(IntensityHistogram* histogram)
{
	m_pHistogram = histogram;
	delete m_pCache;
	m_pCache = 0;
}

void HistogramPainter::setXRange(const Vector2f& xRange) {
	if (xRange != m_xRange) {
		m_xRange = xRange;
		delete m_pCache;
		m_pCache = 0;
	}
}

void HistogramPainter::updateHistogram() {
	delete m_pCache;
	m_pCache = 0;
}

void HistogramPainter::paintEvent(QPaintEvent* event) {
	event->accept();

	if (m_pCache == 0 || m_pCache->rect() != rect()) {
		delete m_pCache;
		m_pCache = new QPixmap(rect().size());
		m_pCache->fill(Qt::transparent);

		QPainter paint(m_pCache);

		// put origin in lower lefthand corner
		QMatrix m;
		m.translate(0.0, static_cast<float>(height())-1);
		m.scale(1.f, -1.f);
		paint.setMatrix(m);
		paint.setMatrixEnabled(true);

		if (m_pHistogram) {
			// draw histogram
			paint.setPen(Qt::NoPen);
			paint.setBrush(QColor(139, 162, 228, 200));
			paint.setRenderHint(QPainter::Antialiasing, true);

			int histogramWidth = 256; //static_cast<int>(m_pHistogram->getBucketCount());
			Vector2f p;

			QPointF* points = new QPointF[histogramWidth + 2];
			int count = 0;

			for (int x=0; x < histogramWidth; ++x) {
				float xpos = static_cast<float>(x) / histogramWidth;
				// Do some simple clipping here, as the automatic clipping of drawPolygon()
				// gets very slow if lots of polygons have to be clipped away, e.g. when
				// zooming to small part of the histogram.
				if (xpos >= m_xRange[0] && xpos <= m_xRange[1]) {
					float value;
					if(m_iHisstyle==0)
						value = m_pHistogram->getNormalized(x);
					else if(m_iHisstyle==1) 
						value= m_pHistogram->getLogNormalized(x);
					p = wtos(Vector2f(xpos, value * (m_yRange[1] - m_yRange[0]) + m_yRange[0]));

					// optimization: if the y-coord has not changed from the two last points
					// then just update the last point's x-coord to the current one
					if( (count >= 2 ) && (points[count - 2].ry() == p.y) && (points[count - 1].ry() == p.y) && (count >= 2) ){
						points[count - 1].rx() = p.x;
					} else {
						points[count].rx() = p.x;
						points[count].ry() = p.y;
						count++;
					}
				}
			}

			// Qt can't handle polygons that have more than 65536 points
			// so we have to split the polygon
			bool needSplit = false;
			if (count > 65536 - 2) { // 16 bit dataset
				needSplit = true;
				count = 65536 - 2; // 2 points needed for closing the polygon
			}

			if (count > 0) {
				// move x coordinate of first and last points to prevent vertical holes caused
				// by clipping
				points[0].rx() = wtos(Vector2f(m_xRange[0], 0.f)).x;
				if (count < histogramWidth - 2) // only when last point was actually clipped
					points[count - 1].rx() = wtos(Vector2f(m_xRange[1], 0.f)).x;

				// needed for a closed polygon
				p = wtos(Vector2f(0.f, m_yRange[0]));
				points[count].rx() = points[count - 1].rx();
				points[count].ry() = p.y;
				count++;
				p = wtos(Vector2f(0.f, m_yRange[0]));
				points[count].rx() = points[0].rx();
				points[count].ry() = p.y;
				count++;

				paint.drawPolygon(points, count);
			}

			// draw last points when splitting is needed
			if (needSplit && false) {
				delete[] points;
				points = new QPointF[5];
				count = 0;
				for (int x=histogramWidth - 2; x < histogramWidth; ++x) {
					float xpos = static_cast<float>(x) / histogramWidth;
					if (xpos >= m_xRange[0] && xpos <= m_xRange[1]) {
						float value;
						if(m_iHisstyle==0)
							value = m_pHistogram->getNormalized(x);
						else if(m_iHisstyle==1) 
							value= m_pHistogram->getLogNormalized(x);
						p = wtos(Vector2f(xpos, value * (m_yRange[1] - m_yRange[0]) + m_yRange[0]));
						points[x-histogramWidth+3].rx() = p.x;
						points[x-histogramWidth+3].ry() = p.y;
						count++;
					}
				}
				if (count > 0) {
					// move x coordinate of last point to prevent vertical holes caused by clipping
					points[count - 1].rx() = wtos(Vector2f(m_xRange[1], 0.f)).x;

					// needed for a closed polygon
					p = wtos(Vector2f(0.f, m_yRange[0]));
					points[count].rx() = points[count - 1].rx();
					points[count].ry() = p.y;
					count++;
					p = wtos(Vector2f(0, m_yRange[0]));
					points[count].rx() = points[0].rx();
					points[count].ry() = p.y;
					count++;

					paint.drawPolygon(points, 5);
				}
			}
			delete[] points;
		}
	}

	QPainter paint(this);
	paint.drawPixmap(0, 0, *m_pCache);
}

Vector2f HistogramPainter::wtos(const Vector2f& p) const {
	float sx = (p.x - m_xRange[0]) / (m_xRange[1] - m_xRange[0]) * (static_cast<float>(width()) \
		- m_iRightSpace - 1.5 * m_iArrowLength - m_iLeftSpace) + m_iLeftSpace;
	float sy = (p.y - m_yRange[0]) / (m_yRange[1] - m_yRange[0]) * (static_cast<float>(height()) \
		- m_iPadding - 1.5 * m_iArrowLength - m_iBottomSpace) + m_iBottomSpace;
	return Vector2f(sx, sy);
}
