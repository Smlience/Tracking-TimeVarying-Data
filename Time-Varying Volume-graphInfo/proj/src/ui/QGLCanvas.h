#ifndef QGLCANVAS_H
#define QGLCANVAS_H

#include <gl/glew.h>
#include <QGLWidget>
#include "util/Camera.h"
#include "util/CanvasObserver.h"
#include "util/util.h"

class QGLCanvas : public QGLWidget, public Observable<IUpdatable>
{
	Q_OBJECT

public:
	QGLCanvas(QWidget *parent = NULL);
	~QGLCanvas();
	Camera* camera();
	virtual void udpate();
	void takeSnapshot(const char* filename);
	void notifyUpdate();
	void drawCoord();

public slots:
	void takeSnapshot();

protected:
	virtual void initializeGL();
	virtual void paintGL();
	virtual void updateGL();
	virtual void resizeGL(int w, int h);
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void mouseDoubleClickEvent(QMouseEvent *e);
	virtual void wheelEvent(QWheelEvent *e);

protected:
	bool m_bInited;
	Camera m_camera;
};

#endif // QGLCANVAS_H
