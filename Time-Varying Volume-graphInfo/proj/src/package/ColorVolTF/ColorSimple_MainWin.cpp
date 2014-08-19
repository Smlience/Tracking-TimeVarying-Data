#include "ColorSimple_MainWin.h"
#include "ColorVolumeSimpleRender.h"
// #include "render/VolumeLightingRender.h"
// #include "render/ViewSphereRender.h"
// #include "ui/CameraLink.h"
// #include "ColorTF2DWidget.h"
// extern CameraLink* pLink;

ColorSimple_MainWin::ColorSimple_MainWin(QWidget *parent, Qt::WindowFlags flags)
	: MainWin(parent, flags)
{}

ColorSimple_MainWin::~ColorSimple_MainWin()
{}

void ColorSimple_MainWin::setupUi()
{
	this->resize(1024, 768);
	ColorVolumeSimpleRender* pRender = new ColorVolumeSimpleRender(this);
	this->setCentralWidget(pRender);
}
