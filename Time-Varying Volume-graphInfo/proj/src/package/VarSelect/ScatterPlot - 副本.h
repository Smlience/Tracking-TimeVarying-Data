#ifndef SCATTERPLOT_H
#define SCATTERPLOT_H

#include <QWidget>
#include <QPushButton>
#include "util\Vector2.h"
#include "util\Vector3.h"

class CanvasPicker;

class ScatterPlot : public QWidget
{
	Q_OBJECT

public:
	ScatterPlot(QWidget *parent = 0);
	~ScatterPlot();
	QPointF transform(const QPointF& in);
	QPointF invTransform(const QPointF& in);

signals:
	void tfChanged(unsigned char* data, Vector2i dim);

public slots:
	void setSamples(const QVector<QPointF> &samples);
	void setData(unsigned char *v1, std::pair<int,int> v1Range, unsigned char*v2, std::pair<int,int> v2Range, Vector3i dim );
	void emitTFData();
	void selectBrushColor();
	void changeBrushToOpactiyIncrease();
	void changeBrushToOpactiyDecrease();
	void changeBrushToErase();
	void changeBrushToFill();
	void clearCanvas();

protected:
	void paintEvent(QPaintEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void resizeEvent(QResizeEvent *e);

private:
	void calcCanvasRect();
	void drawHistograme();

private:
	QMargins m_margins;
	QSize m_size;
	std::pair<int,int> m_partitions;
	std::pair<int,int> m_xRange;
	std::pair<int,int> m_yRange;
	Vector2i m_dim;
	QString m_xLabel;
	QString m_yLabel;
	const QVector<QPointF>* m_samples;
	CanvasPicker* m_picker;
	QMatrix m_matrix;
	unsigned int *m_histogram;
	unsigned int m_histoMax;
	QImage* m_histoImg;
	int m_imgType;// 0 == hitograme, 1 == scatter plot
	unsigned char* m_tfData;

	QPushButton *btnColor;
	QPushButton *btnFill;
	QPushButton *btnOpacityPlus;
	QPushButton *btnOpacitySub;
	QPushButton *btnErase;
	QPushButton *btnClear;
	QWidget* toolbar;
};

#endif // SCATTERPLOT_H
