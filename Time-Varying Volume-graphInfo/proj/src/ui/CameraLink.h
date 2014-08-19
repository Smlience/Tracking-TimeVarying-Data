#ifndef CAMERALINK_H
#define CAMERALINK_H

#include "util/CanvasObserver.h"

class QGLCanvas;

class CameraLink : public IUpdatable
{
public:
	CameraLink(QGLCanvas* l, QGLCanvas* r);
	~CameraLink(void);
	virtual void udpate();

private:
	QGLCanvas* canvas_l;
	QGLCanvas* canvas_r;
};

#endif // CAMERALINK_H