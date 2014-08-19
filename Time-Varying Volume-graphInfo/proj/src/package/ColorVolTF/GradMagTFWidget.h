#ifndef GRADMAGTFWIDGET_H
#define GRADMAGTFWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "ui_GradMagTFWidget.h"

class GradMagTFWidget : public QWidget
{
	Q_OBJECT

public:
	GradMagTFWidget(QWidget *parent = 0);
	~GradMagTFWidget();

signals:
	void sendTF(unsigned char*);

public slots:
	void on_canvas_sendTF(unsigned char* data);
	void setGradMag(const Volume* volume);

protected:
	void resizeEvent(QResizeEvent *e);

private:
	Ui::GradMagTFWidgetClass ui;
	QPushButton *btnReset;
	QPushButton *btnShowTFCurve;
	QPushButton *btnShowHistogram;
	QPushButton *btnSave;
	QPushButton *btnLoad;
	QWidget* toolbar;
};

#endif // GRADMAGTFWIDGET_H