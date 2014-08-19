#ifndef CANVASBRUSHPICKER_H
#define CANVASBRUSHPICKER_H

#include <QWidget>
#include <QPen>
#include <QPoint>
#include "CanvasPicker.h"
#include "util\Vector3.h"

class CanvasBrushPicker: public CanvasPicker
{
	Q_OBJECT

public:
	typedef enum
	{
		BrushTypeFill				= 0,
		BrushTypeOpacityIncrease	= 1,
		BrushTypeOpacityDecrease	= 2,
		BrushTypeErase			= 3
	} BrushType;

public:
    CanvasBrushPicker(QWidget* parent);
	const QImage result();
	QWidget* createToolbar(QWidget* parent);
	QColor getPenColor() const;
	void setPenColor(const QColor &c);
	void setPen(const QColor &c, int w);
	void setBrushType(BrushType t);
	QRgb colorAt(int x, int y);

public slots:
	void clearCanvas();
	void selectBrushColor();
	void changeBrushToOpactiyIncrease();
	void changeBrushToOpactiyDecrease();
	void changeBrushToErase();
	void changeBrushToFill();
	
protected:
	virtual void paintEvent(QPaintEvent *e);
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent* e);
	virtual void resizeEvent(QResizeEvent* e);

private:
	QImage m_canvas;
	QPoint lastPos;
	QPen m_pen;
	int m_brushSize;
	BrushType m_brushType;
};
#endif // CANVASBRUSHPICKER_H
