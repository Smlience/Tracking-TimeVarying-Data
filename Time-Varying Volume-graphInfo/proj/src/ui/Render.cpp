#include "Render.h"

Render::Render(void)
	: width(512), height(512), m_linkedCamera(false), m_pOutCamera(NULL)
{}

Render::~Render(void)
{}

void Render::init()
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
	glViewport(0, 0, width, height);
	m_camera.setWindowSize(width, height);
	m_camera.setOrthoProjection();
}

void Render::paint()
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

void Render::update()
{
	paint();
	notifyUpdate();
}

void Render::resize( int w, int h )
{
	width = w;
	height = h;
	glViewport(0, 0, w, h);
	m_camera.setWindowSize(w, h);
	m_camera.setOrthoProjection();
}

void Render::loadData( const char* filename )
{}

 Camera* Render::camera()
{
	return &m_camera;
}

void Render::mousePressEvent( QMouseEvent *e )
{
	m_camera.beginDrag(e->x(), e->y());
	notifyUpdate();
}

void Render::mouseMoveEvent( QMouseEvent *e )
{
	m_camera.mouseMove(e->x(), e->y());
	notifyUpdate();
}

void Render::mouseReleaseEvent( QMouseEvent *e )
{
	m_camera.endDrag(e->x(), e->y());
	notifyUpdate();
}

void Render::mouseDoubleClickEvent( QMouseEvent *e )
{}

void Render::notifyUpdate()
{
	const std::vector<IUpdatable*> observers = getObservers();
	std::vector<IUpdatable*>::const_iterator iter = observers.begin();
	for (; iter != observers.end(); ++iter)
	{
		(*iter)->udpate();
	}
}

void Render::wheelEvent( QWheelEvent *e )
{
	int numDegrees = e->delta() / 8;
	int numSteps = numDegrees / 15;

	if (numSteps > 0)
	{
		m_camera.zoomOut(1.1 * numSteps);
	}
	else if(numSteps < 0)
	{
		m_camera.zoomIn(-1.1 * numSteps);
	}
	notifyUpdate();
}
