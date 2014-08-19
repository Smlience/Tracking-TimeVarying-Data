#include "QGLCanvas.h"
#include "util/Functions.h"
#include <QResizeEvent>
#include <QDateTime>

QGLCanvas::QGLCanvas(QWidget *parent)
	: QGLWidget(parent), m_bInited(false)
{
	setFocusPolicy(Qt::StrongFocus);
}

QGLCanvas::~QGLCanvas()
{}

void QGLCanvas::initializeGL()
{
	//////////////////////////////////////////////////////////////////////////
	// GLEW Init
	GLenum err = glewInit();
	if (GLEW_OK != err){
		/* Problem: glewInit failed, something is seriously wrong. */
		printf("Error: %s\n", glewGetErrorString(err));
	}
	// Get the entry points for the extension.
	if (!glewIsSupported("GL_NV_fragment_program GL_VERSION_1_2")) {
		printf("Not Support Vertex Program or Fragment Program\n");
	}
	if (!glewIsSupported("GL_VERSION_2_0 GL_ARB_pixel_buffer_object")) {
		printf("Not Support pixel_buffer_object\n");
	}
	//////////////////////////////////////////////////////////////////////////

	glClearColor(0.156863, 0.156863, 0.156863, 0.0);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width(), height());
	m_camera.setWindowSize(width(), height());
	m_camera.setOrthoProjection();

	m_bInited = true;
}

void QGLCanvas::paintGL()
{
	m_camera.setupModelMatrix();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// For test
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
{
	update();
}

void QGLCanvas::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	m_camera.setWindowSize(w, h);
	m_camera.setOrthoProjection();
}

void QGLCanvas::mousePressEvent( QMouseEvent *e )
{
	m_camera.beginDrag(e->x(), e->y());
	notifyUpdate();
}

void QGLCanvas::mouseMoveEvent( QMouseEvent *e )
{
	m_camera.mouseMove(e->x(), e->y());
	notifyUpdate();
}

void QGLCanvas::mouseReleaseEvent( QMouseEvent *e )
{
	m_camera.endDrag(e->x(), e->y());
	notifyUpdate();
}

void QGLCanvas::mouseDoubleClickEvent( QMouseEvent *e )
{}

void QGLCanvas::wheelEvent( QWheelEvent *e )
{
	int numDegrees = e->delta() / 8;
	int numSteps = numDegrees / 15;

	if (numSteps > 0)
	{
		m_camera.zoom(1.1 * numSteps);
	}
	else if(numSteps < 0)
	{
		m_camera.zoom(1.0/(-1.1 * numSteps));
	}
	notifyUpdate();
}

Camera* QGLCanvas::camera()
{
	return &m_camera;
}

void QGLCanvas::udpate()
{
	QGLWidget::update();
}

void QGLCanvas::notifyUpdate()
{
	const std::vector<IUpdatable*> observers = getObservers();
	std::vector<IUpdatable*>::const_iterator iter = observers.begin();
	for (; iter != observers.end(); ++iter)
	{
		(*iter)->udpate();
	}
	update();
}

void QGLCanvas::takeSnapshot( const char* filename )
{
	int w = width();
	int h = height();

	makeCurrent();
	glReadBuffer(GL_FRONT);
	// 取一个点的颜色
	//	unsigned char pix[3];
	// 	glReadPixels(e->x(), h - e->y() - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pix);
	// 	printf("Color[%d, %d] = (%d, %d, %d)\n", e->x(), e->y(), pix[0], pix[1], pix[2] );

	// 截取当前绘制窗口的图像
	unsigned char* pix = new unsigned char[w * h * 4];
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pix);

	// 保存图像
	QImage img1(pix, w, h, QImage::Format_RGB888);
	img1 = img1.mirrored();
	bool result = img1.save(filename, "PNG");

	//计算信息熵
	double entropy = calcImageEntropy(pix, w, h, 3);
	printf("Entropy:%lf\n", entropy);

	delete[] pix;
}

void QGLCanvas::takeSnapshot()
{
	uint now = QDateTime::currentDateTimeUtc().toTime_t();
	char fname[50];
	sprintf(fname, "F:/%u.png", now);
	takeSnapshot(fname);
}

void QGLCanvas::drawCoord()
{
	GLubyte letter_x[8] = {0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00};
	GLubyte letter_y[8] = {0x00, 0x10, 0x10, 0x10, 0x10, 0x28, 0x44, 0x00};
	GLubyte letter_z[8] = {0x00, 0x7e, 0x20, 0x10, 0x08, 0x04, 0x7e, 0x00};

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	double matrix[16];
	Vector4d p0;
	glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, width(), 0.0, height(), -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	double len = 30;
	glTranslated(len + 20, len + 20, 0);

	glColor3ub(255, 0, 0);
	glBegin(GL_LINES);
	p0 = multiply(Vector4d(-len, 0.0, 0.0, len), matrix);
	glVertex3d(p0.x, p0.y, p0.z);
	p0 = multiply(Vector4d(len, 0.0, 0.0, len), matrix);
	glVertex3d(p0.x, p0.y, p0.z);
	glEnd();
	glRasterPos3d(p0.x, p0.y, p0.z);
	glBitmap(8, 8, 0.0, 0.0, 0.0, 0.0, letter_x);

	glColor3ub(0, 255, 0);
	glBegin(GL_LINES);
	p0 = multiply(Vector4d(0.0, -len, 0.0, len), matrix);
	glVertex3d(p0.x, p0.y, p0.z);
	p0 = multiply(Vector4d(0.0,  len, 0.0, len), matrix);
	glVertex3d(p0.x, p0.y, p0.z);
	glEnd();
	glRasterPos3d(p0.x, p0.y, p0.z);
	glBitmap(8, 8, 0.0, 0.0, 0.0, 0.0, letter_y);

	glColor3ub(0, 0, 255);
	glBegin(GL_LINES);
	p0 = multiply(Vector4d(0.0, 0.0, -len, len), matrix);
	glVertex3d(p0.x, p0.y, p0.z);
	p0 = multiply(Vector4d(0.0, 0.0,  len, len), matrix);
	glVertex3d(p0.x, p0.y, p0.z);
	glEnd();
	glRasterPos3d(p0.x, p0.y, p0.z);
	glBitmap(8, 8, 0.0, 0.0, 0.0, 0.0, letter_z);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
