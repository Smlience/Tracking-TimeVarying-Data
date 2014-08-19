#include "camera.h"
#include <GL/glew.h>

Camera::Camera()
	: m_center(0,0,0), m_right(1,0,0), m_up(0,1,0), m_bbCenter(0,0,0), m_bbScale(1.0), 
	m_zoomDefault(1.0), m_pArcBall_bak(NULL)
{
	m_distanceToEye = 10;
	m_eye = Cross(m_right, m_up) * m_distanceToEye + m_center;

	m_zoom = m_zoomDefault;
	m_translateX = 0;
	m_translateY = 0;
	m_translateZ = 4;

	m_pArcBall = new ArcBall();
	m_pArcBall->value(m_arcBallMatrix);
//	m_pArcBall->showResult();
// 	m_pArcBall->setViewDefault();
// 	m_pArcBall->update();
}

Camera::~Camera()
{
	if(m_pArcBall != NULL){
		delete m_pArcBall;
		m_pArcBall = NULL;
	}
}

void Camera::setOrtho2DProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLfloat)m_iWinWidth, 0.0, (GLfloat)m_iWinHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
*	set orthographic projection
*/
void Camera::setOrthoProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-m_dAspect * 1.0, m_dAspect * 1.0, -1.0, 1.0, -100.0, 100.0);
	//setSceneBBox(Vector3d(-m_dAspect * 1.0, -1.0, -1.0), Vector3d(m_dAspect * 1.0, 1.0, 1.0));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
*	set perspective projection
*/
void Camera::setPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// perspective transform
	gluPerspective(30.f, m_dAspect, .1, 1000);//60.f
	// viewing transform
	gluLookAt(
		m_eye.x,    m_eye.y,    m_eye.z,
		m_center.x, m_center.y, m_center.z,
		m_up.x,     m_up.y,     m_up.z );
}

/*
* setup model matrix
*/
void Camera::setupModelMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//	glTranslatef(m_translateX, m_translateY, m_translateZ);

	// zoom
	glScalef(m_zoom, m_zoom, m_zoom);

	// bounding box transformation
	//glScalef(m_bbScale, m_bbScale, m_bbScale);

	// arcball transformation
	m_pArcBall->value(m_arcBallMatrix);
	glMultMatrixd((double*)m_arcBallMatrix);

	glTranslatef(-m_bbCenter.x, -m_bbCenter.y, -m_bbCenter.z);

// 1. translate; 2. rotate; 3. scale
}

/*
*	invoke when left mouse begin drag at position 'x' and 'y'
*/
void Camera::beginDrag(int x, int y)
{
	Quat v = ArcBall::arcBallCoord(x, y, m_iWinWidth, m_iWinHeight);
	m_pArcBall->mouse(v);      // sets new mouse coordinates
	m_pArcBall->beginDrag();   // indictes that dragging should begin
	m_pArcBall->update();      // alters the internal state of the arcball
}

/*
*	invoke when left mouse move at position 'x' and 'y'
*/
void Camera::mouseMove(int x, int y)
{
	Quat v = ArcBall::arcBallCoord(x, y, m_iWinWidth, m_iWinHeight);
	m_pArcBall->mouse(v);					// set the mouse position
	m_pArcBall->update();				    // alters the internal state of the arcball
	m_pArcBall->value(m_arcBallMatrix);		// reads the matrix from the arcball
}

/*
*	invoke when left mouse end drag at position 'x' and 'y'
*/
void Camera::endDrag(int x, int y)
{
	Quat v = ArcBall::arcBallCoord(x, y, m_iWinWidth, m_iWinHeight);
	m_pArcBall->mouse(v);					// sets new mouse coordinates
	m_pArcBall->endDrag();					// indictes that dragging should end
	m_pArcBall->update();					// alters the internal state of the arcball
	m_pArcBall->value(m_arcBallMatrix);		// extracts the current matrix transform
}

/*
*	set window width and height
*/
void Camera::setWindowSize(int width, int height)
{
	m_iWinWidth  = width;
	m_iWinHeight = height;
	m_dAspect    = (double)width / Max(height, 1);
}

/*
*	set scene bounding box
*/
void Camera::setSceneBBox(const Vector3d& minPos, const Vector3d& maxPos)
{
	m_bbCenter = (minPos + maxPos) * 0.5;
	m_bbScale = Max(maxPos.x - m_bbCenter.x, m_bbCenter.x - minPos.x);
	m_bbScale = Max(Max(maxPos.y - m_bbCenter.y, m_bbCenter.y - minPos.y), m_bbScale);
	m_bbScale = Max(Max(maxPos.z - m_bbCenter.z, m_bbCenter.z - minPos.z), m_bbScale);
	if(m_bbScale)
		m_bbScale = 1.0 / m_bbScale;
	else
		m_bbScale = 1.0;
}

/*
*	set view as xy plane
*/
void Camera::setViewXY()
{
	m_pArcBall->setViewXY();
	m_pArcBall->update();
	m_pArcBall->value(m_arcBallMatrix);
}

/*
*	set view as xz plane
*/
void Camera::setViewXZ()
{
	m_pArcBall->setViewXZ();
	m_pArcBall->update();
	m_pArcBall->value(m_arcBallMatrix);
}

/*
*	set view yz plane
*/
void Camera::setViewYZ()
{
	m_pArcBall->setViewYZ();
	m_pArcBall->update();
	m_pArcBall->value(m_arcBallMatrix);
}

/*
*	set view default
*/
void Camera::setViewDefault()
{
	m_pArcBall->setViewDefault();
	m_pArcBall->update();
	m_pArcBall->value(m_arcBallMatrix);
	m_zoom = m_zoomDefault;
}

void Camera::setModelTranslate(int x, int y)
{
	m_translateX += x* 0.05;
	m_translateY -= 0.05 * y;
}

void Camera::translate( double x, double y, double z )
{
	m_translateX += x;
	m_translateY += y;
	m_translateZ += z;
}

void Camera::drawArc()
{
	if (m_pArcBall != NULL)
	{
		m_pArcBall->draw(1.0);
	}
}

void Camera::link( Camera* r )
{
	if (m_pArcBall_bak == NULL)
	{
		m_pArcBall_bak = m_pArcBall;
	}
	m_pArcBall = r->getArcBall();
	m_pArcBall->value(m_arcBallMatrix);
}

ArcBall* Camera::getArcBall()
{
	return m_pArcBall;
}

void Camera::breakLink()
{
	if (m_pArcBall_bak != NULL)
	{
		m_pArcBall = m_pArcBall_bak;
		m_pArcBall_bak = NULL;
		m_pArcBall->value(m_arcBallMatrix);
	}
}

Vector3f Camera::viewDirection()
{
	return Vector3f(m_arcBallMatrix[0][2], m_arcBallMatrix[1][2], m_arcBallMatrix[2][2]);
}

void Camera::autoSpin( float angle )
{
	m_pArcBall->AutoSpin(angle);
	m_pArcBall->update();
	m_pArcBall->value(m_arcBallMatrix);
}

void Camera::lookAt( double theta, double phi )
{
	m_pArcBall->lookAT(theta, phi);
	m_pArcBall->update();
	m_pArcBall->value(m_arcBallMatrix);
}
