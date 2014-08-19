#ifndef HISTOGRAMPAINTER_H
#define HISTOGRAMPAINTER_H

#include <QWidget>
#include "util/Vector2.h"

class IntensityHistogram;

class vxHistogramPainter:public QWidget
{
	Q_OBJECT
public:
	vxHistogramPainter(QWidget* parent = 0, Vector2f xRange = Vector2f(0.f, 1.f), Vector2f yRange = Vector2f(0.f, 1.f),
		int leftSpace = 0, int rightSpace = 0, int bottomSpace = 0, int padding = 0, int arrowLength = 0);

	~vxHistogramPainter();

	void paintEvent(QPaintEvent* event);
	void setXRange(const Vector2f& xRange);
	void updateHistogram();
	void setHisStyle(int style) { m_iHisstyle = style;}

public slots:
	void setHistogram(IntensityHistogram* histogram);

private:
	inline Vector2f wtos(const Vector2f& p) const;

	Vector2f m_xRange;
	Vector2f m_yRange;
	int m_iLeftSpace;
	int m_iRightSpace;
	int m_iBottomSpace;
	int m_iPadding;
	int m_iArrowLength;

	IntensityHistogram* m_pHistogram;

	QPixmap* m_pCache;

	int m_iHisstyle;

};

#endif //HISTOGRAMPAINTER_H