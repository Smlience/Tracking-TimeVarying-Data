#include "mainwin.h"
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include "CameraLink.h"
#include "LightParameterWidget.h"
#include "render/VolumeLightingRender.h"
#include "render/ViewSphereRender.h"
#include "tf/1dtf/TF1DWidget.h"

CameraLink* pLink;
MainWin::MainWin(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{}

MainWin::~MainWin()
{
	delete pLink;
}

void MainWin::init()
{
	setupMenu();
	setupUi();
	QMetaObject::connectSlotsByName(this);
}

void MainWin::on_actionOpen_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open Volume Data", "", "Volume Dataset Header File(*.vifo);;All Files(*.*)");
	if (!filename.isEmpty() && !filename.isNull())
	{
		static_cast<VolumeRender*>(this->centralWidget())->loadData(filename.toLocal8Bit().data());
	}
}

void MainWin::setupUi()
{
	this->resize(800, 600);
	VolumeLightingRender* pRender = new VolumeLightingRender(this);
	this->setCentralWidget(pRender);

	LightParameterWidget* lightParam = new LightParameterWidget();
	ViewSphereRender* viewBall = new ViewSphereRender();
	TF1DWidget* tfWidget = new TF1DWidget();
	pLink = new CameraLink(viewBall, pRender);

	this->addDock(lightParam);
	this->addDock(viewBall)->setMinimumSize(QSize(250, 38));
	this->addDock(tfWidget);

	lightParam->connectRender(pRender);
	connect(pRender, SIGNAL(dataLoaded(const Volume*)), tfWidget, SLOT(setVolume(const Volume*)));
	connect(tfWidget, SIGNAL(sendTF(unsigned char*)), pRender, SLOT(set1DTF(unsigned char*)));
	connect(this->findChild<QAction*>("actionScreenshot"), SIGNAL(triggered()), pRender, SLOT(takeSnapshot()));
}

void MainWin::setupMenu()
{
	QMenuBar* menuBar = new QMenuBar(this);
	menuBar->setObjectName(QString::fromUtf8("menuBar"));
	QAction* actionOpen = new QAction("Open", this);
	actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
	QAction*actionScreenshot = new QAction("Screenshot", this);
	actionScreenshot->setObjectName(QString::fromUtf8("actionScreenshot"));
	QMenu* menuFile = new QMenu("File", menuBar);
	menuFile->setObjectName(QString::fromUtf8("menuFile"));
	menuFile->addAction(actionOpen);
	menuFile->addAction(actionScreenshot);
	menuBar->addAction(menuFile->menuAction());
	this->setMenuBar(menuBar);
}

QDockWidget* MainWin::addDock( QWidget* pWidget, Qt::DockWidgetArea area)
{
	QDockWidget* pDock = new QDockWidget(this);
	pDock->setWidget(pWidget);
	pDock->setWindowTitle(pWidget->windowTitle());
	this->addDockWidget(area, pDock);
	return pDock;
}
