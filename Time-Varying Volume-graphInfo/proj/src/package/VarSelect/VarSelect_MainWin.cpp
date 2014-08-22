#include "VarSelect_MainWin.h"
//#include "render/VolumeLightingRender.h"
#include "VolumeLightingRender2DTF.h"
#include "VolumeLightingRender2DTF_pos.h"
#include "package/timewidget/timeSelect.h"
#include "ScatterPlotWidget.h"
#include "ScatterPlotCalc.h"
#include "package/VarSelect/EvolutionGraphWidget.h"
//#include "plot.h"
#include <QAction>
// #include "render/ViewSphereRender.h"
// #include "ui/CameraLink.h"
// #include "ColorTF2DWidget.h"
// extern CameraLink* pLink;

VarSelect_MainWin::VarSelect_MainWin(QWidget *parent, Qt::WindowFlags flags)
	: MainWin(parent, flags)
{}

VarSelect_MainWin::~VarSelect_MainWin()
{}

void VarSelect_MainWin::setupUi()
{
	this->resize(1664, 800);
	VolumeLightingRender2DTF* pRender = new VolumeLightingRender2DTF(this);
	//VolumeLightingRender2DTF_pos* pRender = new VolumeLightingRender2DTF_pos(this);

	this->setCentralWidget(pRender);

	//ScatterPlotWidget* scatterPlot = new ScatterPlotWidget(this);
//	Plot* scatterPlot = new Plot(this);
	//this->addDock(scatterPlot, Qt::RightDockWidgetArea);

	//ScatterPlotCalc* scatterCalc = new ScatterPlotCalc;
	// LightParameterWidget* lightParam = new LightParameterWidget();
	// lightParam->connectRender(pRender);
	// this->addDock(lightParam, Qt::BottomDockWidgetArea);

	//tabifyDockWidget(tf2dDock, tf1dDock);

	//////////////////////////////////////////////////////////////////////////  BottomDockWidgetArea
	EvolutionGraphWidget *evlutionGraph=new EvolutionGraphWidget(this);
	this->addDock(evlutionGraph, Qt::RightDockWidgetArea);

	//evolution graph 选点  定位时间  定位聚类号
	connect(evlutionGraph, SIGNAL(sendTimeNumSelected(int)), pRender, SLOT(setCurTime(int)));
	//connect(evlutionGraph, SIGNAL(sendTimeNumSelected(int)), scatterPlot, SLOT(setCurTime(int)));
	connect(evlutionGraph, SIGNAL(sendTF( unsigned char *)), pRender, SLOT(set1DTF(unsigned char* )));

	connect(pRender, SIGNAL(sendFileDirName(  char *)), evlutionGraph, SLOT(setFileDirName( char* )));

	//TimeSelectWidget * timeSelect=new TimeSelectWidget(this);
	//this->addDock(timeSelect, Qt::BottomDockWidgetArea);

	//connect(timeSelect->m_sliderTime, SIGNAL(valueChanged(int)), pRender, SLOT(setCurTime(int)));
	//connect(timeSelect->m_sliderTime, SIGNAL(valueChanged(int)), scatterPlot, SLOT(setCurTime(int)));

	connect(this->findChild<QAction*>("actionScreenshot"), SIGNAL(triggered()), pRender, SLOT(takeSnapshot()));
	update();
	//connect(pRender, SIGNAL(dataLoaded(const Volume*)), scatterCalc, SLOT(setVolume(const Volume*)));
//	connect(scatterCalc, SIGNAL(samples(const QVector<QPointF> &)), scatterPlot, SLOT(setSamples(const QVector<QPointF> &)));
	//connect(scatterCalc, SIGNAL(dataChanged(const unsigned char *, std::pair<int,int>, const unsigned char*, std::pair<int,int>, Vector3i)), scatterPlot, SLOT(setData(const unsigned char *, std::pair<int,int>, const unsigned char*, std::pair<int,int>, Vector3i)));
	//connect(scatterCalc, SIGNAL(dataChanged(const unsigned char *, std::pair<int,int>, const unsigned char*, std::pair<int,int>, Vector3i)), pRender, SLOT(setV1V2(const unsigned char *, std::pair<int,int>, const unsigned char*, std::pair<int,int>, Vector3i)));
	//connect(scatterPlot, SIGNAL(tfChanged(unsigned char*, Vector2i)), pRender, SLOT(set2DTF(unsigned char*, Vector2i)));
}
