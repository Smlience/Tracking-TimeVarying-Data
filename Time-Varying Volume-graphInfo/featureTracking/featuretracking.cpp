#include "featuretracking.h"
#include "src/dm/Volume.h"
#include <QDockWidget>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>

featureTracking::featureTracking(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

featureTracking::~featureTracking()
{

}

void featureTracking::on_actionOpen_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open Volume Data", "", "Volume Dataset Header File(*.vifo);;All Files(*.*)");
	printf("Hello file open\n");
	if (!filename.isEmpty() && !filename.isNull())
	{
		printf("Hello file open\n");
		v.loadVolume(filename.toLocal8Bit().data());
	}
}

void featureTracking::init()
{
	setupMenu();
	setupUi();
	QMetaObject::connectSlotsByName(this);
}

void featureTracking::setupUi()
{
	this->resize(800, 600);
	

}

void featureTracking::setupMenu()
{
	QMenuBar* menuBar = new QMenuBar(this);
	menuBar->setObjectName(QString::fromUtf8("menuBar"));
	QAction* actionOpen = new QAction("Open", this);
	actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
	
	QMenu* menuFile = new QMenu("File", menuBar);
	menuFile->setObjectName(QString::fromUtf8("menuFile"));
	menuFile->addAction(actionOpen);
	
	menuBar->addAction(menuFile->menuAction());
	this->setMenuBar(menuBar);
}
