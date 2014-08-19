#ifndef PAINTER_H
#define PAINTER_H

#include <GL/glew.h>
#include <QMouseEvent>
#include "util/Camera.h"
#include "util/CanvasObserver.h"

class Render : public Observable<IUpdatable>
{
public:
	Render(void);
	~Render(void);
	virtual void init();
	virtual void paint();
	virtual void update();
	virtual void resize(int w, int h);
	virtual void loadData(const char* filename);
	Camera* camera();
// 	void linkCamera(Camera* camera);
// 	void unlinkCamera();
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);
	virtual void mouseDoubleClickEvent(QMouseEvent *e);
	virtual void wheelEvent(QWheelEvent *e);

private:
	void notifyUpdate();

protected:
	int width;
	int height;
	Camera m_camera;
	bool m_linkedCamera; // When an out camera is linked to this, it is set to be true
	Camera* m_pOutCamera;
};

#endif // PAINTER_H
