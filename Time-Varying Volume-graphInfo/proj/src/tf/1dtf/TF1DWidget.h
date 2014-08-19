#ifndef TF1D_H
#define TF1D_H

#include <QWidget>
#include "ui_tf1dwidget.h"

class IntensityHistogram;

class TF1DWidget : public QWidget
{
	Q_OBJECT

public:
	TF1DWidget(QWidget *parent = 0);
	~TF1DWidget();

signals:
	void colorChanged(const QColor& c);
	void tfChanged();
	void tfReset();
	void sendTF(unsigned char*);

public slots:
	void on_canvas_colorChanged(const QColor& c);
	void on_canvas_resetTransferFunction();
	void on_canvas_changed();
	void on_canvas_sendTF(unsigned char* data);
	void on_btnSave_clicked();
	void on_btnLoad_clicked();
	void setVolume(const Volume* volume);

private:
	Ui::TF1D ui;
	IntensityHistogram* m_pHistogram;
};

#endif // TF1D_H
