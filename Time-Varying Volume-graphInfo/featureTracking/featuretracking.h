#ifndef FEATURETRACKING_H
#define FEATURETRACKING_H

#include "src/dm/Volume.h"
#include <QtWidgets/QMainWindow>
#include "ui_featuretracking.h"

class featureTracking : public QMainWindow
{
	Q_OBJECT

public:
	featureTracking(QWidget *parent = 0);
	~featureTracking();

	virtual void init();
	virtual void setupUi();
	virtual void setupMenu();

public slots:
	void on_actionOpen_triggered();

private:
	Ui::featureTrackingClass ui;
	Volume v;
};

#endif // FEATURETRACKING_H
