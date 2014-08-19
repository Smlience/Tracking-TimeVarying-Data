#include "VolumeRender.h"
#include "QGLCanvas.h"
#include <QResizeEvent>

QGLCanvas::QGLCanvas(QWidget *parent)
	: QGLWidget(parent)
{}

QGLCanvas::~QGLCanvas()
{}

void QGLCanvas::initializeGL()
{
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		cout<<"Error: "<<glewGetErrorString(err)<<endl;
	}
	glClearColor(0.156863, 0.156863, 0.156863, 0.0);
	glEnable(GL_DEPTH_TEST);

	m_render.init();
}

void QGLCanvas::paintGL()
{
	m_camera.setupModelMatrix();
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_QUADS);
		glColor3ub(255, 0, 0);
		glVertex3f(-0.5, -0.5, 0.0);
		glColor3ub(0, 255, 0);
		glVertex3f(0.5, -0.5, 0.0);
		glColor3ub(0, 0, 255);
		glVertex3f(0.5, 0.5, 0.0);
		glColor3ub(255, 255, 255);
		glVertex3f(-0.5, 0.5, 0.0);
	glEnd();
}

void QGLCanvas::updateGL()
{}

void QGLCanvas::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	m_camera.setWindowSize(w, h);
	m_camera.setOrthoProjection();
}

void QGLCanvas::mousePressEvent( QMouseEvent *e )
{
	int x = e->x();
	int y = e->y();
	m_camera.beginDrag(e->x(), width() - e->y());

	//makeCurrent();

// 	glReadBuffer(GL_FRONT);
// 	float pix[3];
// 	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pix);
// 	printf("[%d, %d] -> %f %f %f\n", x, y, pix[0], pix[1], pix[2]);
// 
// 	unsigned char pixel[3];
// 	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
// 	printf("[%d, %d] -> %d %d %d\n", x, y, pixel[0], pixel[1], pixel[2]);

	update();
}

void QGLCanvas::mouseMoveEvent( QMouseEvent *e )
{
	m_camera.mouseMove(e->x(), width() - e->y());
	update();
}

void QGLCanvas::mouseReleaseEvent( QMouseEvent *e )
{
	m_camera.endDrag(e->x(), width() - e->y());
	update();
}
