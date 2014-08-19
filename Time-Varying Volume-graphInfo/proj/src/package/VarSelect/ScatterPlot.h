#ifndef SCATTERPLOTCANVAS_H
#define SCATTERPLOTCANVAS_H

#include <QWidget>
#include "util\Vector2.h"
#include "util\Vector3.h"

class ScatterPlot : public QWidget
{
	Q_OBJECT

public:
	ScatterPlot(QWidget *parent = 0);
	~ScatterPlot();
	QPointF transform(const QPointF& pos);
	QPointF invTransform(const QPointF& in);
	QMargins margins() const;
	QSize plotSize() const;
	QRect plotRect() const;
	Vector2i dataDimension() const;

	typedef enum{Histogram = 0, Scatter = 1} PlotType;
	void setPlotType(PlotType t);

public slots:
	void setData(const unsigned char *v1, std::pair<int,int> v1Range, const unsigned char*v2, std::pair<int,int> v2Range, Vector3i dim );

protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);

private:
	void calcCanvasRect();
	void drawHistogram();

private:
	QMargins m_margins;
	QSize m_size;
	std::pair<int,int> m_partitions;
	std::pair<int,int> m_xRange;
	std::pair<int,int> m_yRange;
	Vector2i m_dim;
	QString m_xLabel;
	QString m_yLabel;
	QMatrix m_matrix;
	unsigned int *m_histogram;
	unsigned int m_histoMax;
	QImage* m_histoImg;
	PlotType m_imgType;
};

#endif // SCATTERPLOTCANVAS_H
