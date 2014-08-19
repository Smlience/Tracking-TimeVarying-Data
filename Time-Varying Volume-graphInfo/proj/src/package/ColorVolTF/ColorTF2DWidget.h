#ifndef COLORTF2DWIDGET_H
#define COLORTF2DWIDGET_H

#include <QWidget>
#include "ui_ColorTF2DWidget.h"

class ColorTF2DWidget : public QWidget
{
	Q_OBJECT

public:
	ColorTF2DWidget(QWidget *parent = 0);
	~ColorTF2DWidget();

signals:
	void sendTF(unsigned char*);
	void sendClusterInfo(unsigned char*);
	void changeRenderMode(int mode);

public slots:
	void on_canvas_sendTF(unsigned char* data);
	void on_canvas_sendClusterInfo(unsigned char* data);
	void setVolume(const Volume* volume);
	void on_cbMode_currentIndexChanged(int index);

private:
	Ui::ColorTF2DWidget ui;
};

#endif // COLORTF2DWIDGET_H
