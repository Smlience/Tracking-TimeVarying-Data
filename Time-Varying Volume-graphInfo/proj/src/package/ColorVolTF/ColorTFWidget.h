#ifndef COLORTFWIDGET_H
#define COLORTFWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "ui_ColorTFWidget.h"

class ColorTFWidget : public QWidget
{
	Q_OBJECT

public:
	ColorTFWidget(QWidget *parent = 0);
	~ColorTFWidget();

protected:
	void resizeEvent( QResizeEvent *e );

signals:
	void tfChanged();
	void tfReset();
	void sendTF(unsigned char*);
	void sendClusterInfo(unsigned char*);
	void showSlice(int axis, int slice);
	void changeRenderMode(int mode);
	void changeGradientMode(int mode);

public slots:
// 	void on_canvas_resetTransferFunction();
// 	void on_canvas_changed();
	void on_canvas_sendTF(unsigned char* data);
	void on_canvas_sendClusterInfo(unsigned char* data);
	void setVolume(const Volume* volume);
//	void on_btnMode_clicked(bool checked);
// 	void on_cbMode_currentIndexChanged(int index);
// 	void on_cbGradient_currentIndexChanged(int index);
private:
	Ui::ColorTFWidgetClass ui;
	QPushButton *btnReset;
	QPushButton *btnShowTFCurve;
	QPushButton *btnShowHistogram;
	QPushButton *btnSave;
	QPushButton *btnLoad;
	QWidget* toolbar;
};

#endif // COLORTFWIDGET_H
