#include "ColorVolTF_MainWin.h"
#include "ui/LightParameterWidget.h"
#include "ColorTFWidget.h"
#include "ColorVolumeRender.h"
#include "SliceGradientRender.h"
#include "GradMagTFWidget.h"
// #include "render/VolumeLightingRender.h"
// #include "render/ViewSphereRender.h"
// #include "ui/CameraLink.h"
// #include "ColorTF2DWidget.h"
// extern CameraLink* pLink;

ColorVolTF_MainWin::ColorVolTF_MainWin(QWidget *parent, Qt::WindowFlags flags)
	: MainWin(parent, flags)
{}

ColorVolTF_MainWin::~ColorVolTF_MainWin()
{}

void ColorVolTF_MainWin::setupUi()
{
	this->resize(1024, 768);
	ColorVolumeRender* pRender = new ColorVolumeRender(this);
	this->setCentralWidget(pRender);

	SliceGradientRender* pSliceGradRender = new SliceGradientRender(this);
	pSliceGradRender->setMinimumWidth(450);
	this->addDock(pSliceGradRender, Qt::RightDockWidgetArea);

	ColorTFWidget* tfWidget = new ColorTFWidget();
	this->addDock(tfWidget, Qt::BottomDockWidgetArea);

	GradMagTFWidget* gmtf = new GradMagTFWidget(this);
	this->addDock(gmtf, Qt::BottomDockWidgetArea);

	LightParameterWidget* lightParam = new LightParameterWidget();
	lightParam->connectRender(pRender);
	this->addDock(lightParam, Qt::BottomDockWidgetArea);

	//tabifyDockWidget(tf2dDock, tf1dDock);
	connect(this->findChild<QAction*>("actionScreenshot"), SIGNAL(triggered()), pRender, SLOT(takeSnapshot()));
	connect(pRender, SIGNAL(dataLoaded(const Volume*)), tfWidget, SLOT(setVolume(const Volume*)));
	connect(tfWidget, SIGNAL(sendTF(unsigned char*)), pRender, SLOT(set1DTF(unsigned char*)));
	connect(tfWidget, SIGNAL(sendClusterInfo(unsigned char*)), pRender, SLOT(setClusterInfo(unsigned char*)));
	connect(tfWidget, SIGNAL(changeRenderMode(int)), pRender, SLOT(setRenderMode(int)));
	connect(tfWidget, SIGNAL(changeGradientMode(int)), pRender, SLOT(setGradientMode(int)));

	connect(pRender, SIGNAL(sendGradMag(const Volume*)), gmtf,	  SLOT(setGradMag(const Volume*)));
	connect(gmtf,	 SIGNAL(sendTF(unsigned char*)),	 pRender, SLOT(setGMTF(unsigned char*)));
	
	connect(tfWidget, SIGNAL(showSlice(int, int)), pSliceGradRender, SLOT(showSlice(int, int)));
	connect(pRender, SIGNAL(dataLoaded(const Volume*)), pSliceGradRender, SLOT(setVolume(const Volume*)));
	connect(pRender, SIGNAL(gradientChanged(unsigned char*, unsigned char*)), pSliceGradRender, SLOT(setGradient(unsigned char*, unsigned char*)));
}
