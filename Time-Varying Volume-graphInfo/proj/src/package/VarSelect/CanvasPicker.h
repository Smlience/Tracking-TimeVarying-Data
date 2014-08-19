#ifndef CANVASPICKER_H
#define CANVASPICKER_H

#include <QWidget>

class CanvasPicker : public QWidget
{
	Q_OBJECT
public:
	CanvasPicker(QWidget* parent = NULL):QWidget(parent){};
	virtual ~CanvasPicker(){};
	virtual QWidget* createToolbar(QWidget* parent){return NULL;};
	virtual const QImage result() = 0;
signals:
	void canvasChanged();
};

#endif