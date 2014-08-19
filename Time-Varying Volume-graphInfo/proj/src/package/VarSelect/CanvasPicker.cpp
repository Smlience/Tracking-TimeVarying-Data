#ifndef CANVASPICKER_H
#define CANVASPICKER_H

#include <QWidget>

class CanvasPicker : public QWidget
{
	Q_OBJECT
public:
	CanvasPicker(QWidget* parent = NULL);
	virtual ~CanvasPicker();
signals:
	void tfChanged();
};

#endif