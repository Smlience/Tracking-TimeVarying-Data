#include "plot.h"
#include "CanvasPicker.h"
#include <QResizeEvent>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_layout.h>
#include <time.h>

//////////////////////////////////////////////////////////////////////////
class PointPicker: public QwtPlotPicker
{
public:
    PointPicker( QWidget *canvas ):
        QwtPlotPicker( canvas )
    {
        setTrackerMode( QwtPicker::ActiveOnly );
        setStateMachine( new QwtPickerClickPointMachine() );
//        setRubberBand( QwtPlotPicker::PolygonRubberBand );
    }

//     virtual QwtText trackerTextF( const QPointF &pos ) const
//     {
//         QwtText text;
// 
//         const QPolygon points = selection();
// //        if ( !points.isEmpty() )
//         {
//             QString num;
// 			const QPointF p = invTransform(pos.toPoint());
// 			num = QString::number(p.x()) + "," + QString::number(p.y());
//             //num.setNum( QLineF( pos, invTransform( points[0] ) ).length() );
// 
//             QColor bg( Qt::white );
//             bg.setAlpha( 200 );
// 
//             text.setBackgroundBrush( QBrush( bg ) );
//             text.setText( num );
//         }
//         return text;
//     }
};

Plot::Plot( QWidget *parent ):
    QwtPlot( parent ),
    d_curve( NULL )
{
	canvas()->setStyleSheet(
		"border: 1px solid Black;"
		"background-color: White;"
		);

    // attach curve
    d_curve = new QwtPlotCurve( "Scattered Points" );
    d_curve->setPen( QColor( "Purple" ) );
    // when using QwtPlotCurve::ImageBuffer simple dots can be
    // rendered in parallel on multicore systems.
    d_curve->setRenderThreadCount( 0 ); // 0: use QThread::idealThreadCount()
    d_curve->attach( this );

	d_curveSelected = new QwtPlotCurve("Selected Points");
	d_curveSelected->setPen(QColor(128, 128, 0));
	d_curveSelected->setStyle( QwtPlotCurve::Dots );
	d_curveSelected->attach(this);

    setSymbol( NULL );

    // panning with the left mouse button
    //(void )new QwtPlotPanner( canvas() );

//     // zoom in/out with the wheel
//     QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( canvas() );
//     magnifier->setMouseButton( Qt::NoButton );
// 
    // distanve measurement with the right mouse button
    //PointPicker *picker = new PointPicker( canvas() );
    //picker->setMousePattern( QwtPlotPicker::MouseSelect1, Qt::RightButton );
    //picker->setRubberBandPen( QPen( Qt::blue ) );
	QwtPlotPicker* picker = new QwtPlotPicker(this->canvas());
	picker->setStateMachine(new QwtPickerClickPointMachine);
	picker->setMousePattern(QwtEventPattern::MouseSelect1,Qt::LeftButton);
	//////////////////////////////////////////////////////////////////////////
	setMouseTracking(true);
	canvas()->setMouseTracking(true);
	setAxisScale(QwtPlot::yLeft, 0.0, 256.0, 16.0);
	setAxisScale(QwtPlot::xBottom, 0.0, 256.0, 16.0);
	setAxisTitle(QwtPlot::yLeft, "Hello");
	setAxisTitle(QwtPlot::xBottom, "World");
	plotLayout()->setAlignCanvasToScales(true);
	connect(picker, SIGNAL(selected(const QPointF &)), this, SLOT(pointSelected(const QPointF &)));
	//////////////////////////////////////////////////////////////////////////
	m_canvasPicker = new CanvasPicker(this);
// 	//////////////////////////////////////////////////////////////////////////
// 	// 随机设置一些点，用于测试
// 	srand(time(0));
// 	for ( int i = 0; i < 10000; i++ )
// 	{
// 		const double x = rand() / float(RAND_MAX) * 254.0 + 1.0;
// 		const double y = ::log( 10.0 * ( x - 1.0 ) + 1.0 ) 	* ( rand() / float(RAND_MAX) * 0.5 + 0.9 ) * 10;
// 
// 		samples += QPointF( x, y );
// 	}
// 	for (int i = 0; i < 10000; ++i)
// 	{
// 		samples.push_back(QPointF(rand() * 256.0f / RAND_MAX, rand() * 256.0f / RAND_MAX));
// 	}
//	setSamples(samples);
//	insertCurve();
}

void Plot::setSymbol( QwtSymbol *symbol )
{
    d_curve->setSymbol( symbol );

    if ( symbol == NULL )
    {
        d_curve->setStyle( QwtPlotCurve::Dots );
    }
}

void Plot::setSamples( const QVector<QPointF> &samples )
{
	printf("setSamples\n");
    d_curve->setPaintAttribute( QwtPlotCurve::ImageBuffer, samples.size() > 1000 );

    d_curve->setSamples( samples );
	replot();
}

// void Plot::paintEvent( QPaintEvent *e )
// {
// 	QwtPlot::paintEvent(e);
// 
// 	QPainter p(this->canvas());
// 	p.drawRect(100, 100, 100, 100);
// }

void Plot::insertCurve()
{
	Qt::Orientation o(Qt::Horizontal);
	double base = 19.6;
	QwtPlotCurve *curve = new QwtPlotCurve();
	QRgb rgb = static_cast<QRgb>( rand() );
	QColor c(rgb);
	curve->setPen( c );
	curve->setSymbol( new QwtSymbol( QwtSymbol::Rect, Qt::gray, c, QSize(8, 8)));
	curve->setStyle(QwtPlotCurve::Dots);
	curve->setPen(QColor(255, 0, 0));
	curve->setBrush(QColor(128, 128, 0, 128));

	double x[4];
	double y[4];
	x[0] = 10.0;
	y[0] = 10.0;
	x[1] = 50.0;
	y[1] = 10.0;
	x[2] = 70.0;
	y[2] = 70.0;
	x[3] = 10.0;
	y[3] = 50.0;

	curve->setSamples( x, y, sizeof( x ) / sizeof( x[0] ) );
	curve->attach( this );
}

void Plot::pointSelected( const QPointF &p )
{
	d_curveSelected->setPen(QColor(128, 128, 0), 5);
	QVector<QPointF> v;
	v << p;
	d_curveSelected->setSamples(v);
	replot();
}

void Plot::resizeEvent( QResizeEvent *e )
{
	QwtPlot::resizeEvent(e);
	QRect r = canvas()->rect();
	m_canvasPicker->resize(r.size().width(), r.size().height());
	m_canvasPicker->move(r.x(), r.y());
}
