#ifndef _PLOT_H_
#define _PLOT_H_ 1

#include <qwt_plot.h>

class QwtPlotCurve;
class QwtSymbol;
class CanvasPicker;

class Plot : public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget *parent = NULL );

    void setSymbol( QwtSymbol * );

public slots:
    void setSamples( const QVector<QPointF> &samples );
	void pointSelected(const QPointF &p);

protected:
//	void paintEvent(QPaintEvent *e);
	void resizeEvent( QResizeEvent *e );
	void insertCurve();

private:
	QwtPlotCurve *d_curve;
	QwtPlotCurve *d_curveSelected;
	QVector<QPointF> samples;
	CanvasPicker* m_canvasPicker;
};

#endif // _PLOT_H_
