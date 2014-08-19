#ifndef CANVASPOLYGONPICKER_H
#define CANVASPOLYGONPICKER_H

#include "CanvasPicker.h"

class Polygon;

class CanvasPolygonPicker: public CanvasPicker
{
    Q_OBJECT
public:
    CanvasPolygonPicker(QWidget *parent = NULL);
	~CanvasPolygonPicker();
	const QImage result();
	QWidget* createToolbar(QWidget* parent);
	void selectAt(const QPoint &position);
	Polygon* addRect(const QPoint &p1, const QPoint &p2);
	Polygon* addPolygon(QVector<QPoint>& v);
	void movePolygon(Polygon* p, const QPoint &from, const QPoint &to);
	void set_default_tf(int num_of_features);
signals:
	void selectedPolygonOpacityChanged(int);

public slots:
	void selectSelectedPolygonColor();
	void setSelectedPolygonOpacity(int v);
	void clear();

protected:
	virtual void keyPressEvent(QKeyEvent *e);
	virtual void keyReleaseEvent(QKeyEvent *e);
	virtual void paintEvent(QPaintEvent* e);
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseMoveEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent* e);
	virtual void mouseDoubleClickEvent(QMouseEvent* e);

private:
	void drawPolygon(QPainter *const p, Polygon* polygon);
	void drawPolygonForInteraction(QPainter *const p, Polygon* polygon);
	void drawPolygonForSelection(QPainter *const p, unsigned int idx, Polygon* polygon);
	void selectPolygonColor(Polygon* p);

private:
	QPoint m_current;
	QPoint m_last;
	QPoint m_first;
	Polygon* m_selected;
	Polygon* m_drawing;
	QVector<QPoint> m_drawingPolygon;
	int m_editingVertexIdx;
	QVector<Polygon*> m_vPoly;
	QColor m_borderColor;
	QColor m_borderColorSelectedPoly;
	QColor m_borderColorSelectedVertex;
};
#endif// CANVASPOLYGONPICKER_H
