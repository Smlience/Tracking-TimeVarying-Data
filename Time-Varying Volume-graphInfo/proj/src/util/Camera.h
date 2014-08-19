#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "ArcBall.h"

class Camera
{
	friend class emxMeshRender;

public:
	Camera();
	~Camera();

	ArcBall* getArcBall();
	void link(Camera* r);
	void breakLink();

	Vector3f viewDirection();

	// set 2D orthographic projection
	void setOrtho2DProjection();

	// set orthographic projection
	void setOrthoProjection();

	// set perspective projection
	void setPerspectiveProjection();

	// setup model matrix
	void setupModelMatrix();

	// invoke when left mouse begin drag at position 'x' and 'y'
	void beginDrag(int x, int y);

	// invoke when left mouse move at position 'x' and 'y'
	void mouseMove(int x, int y);

	// invoke when left mouse end drag at position 'x' and 'y'
	void endDrag(int x, int y);

	void drawArc();

	void autoSpin(float angle);

	void lookAt(double theta, double phi);

public:
	void setZoom(double value) {m_zoom = value;}
	// zoom
	void zoom(double value) { assert(value > 0); m_zoom *= value; }

	// set view as xy plane
	void setViewXY();
	// set view as xz plane
	void setViewXZ();
	// set view yz plane
	void setViewYZ();

	// set view default
	void setViewDefault();
	
	void setModelTranslate(int x, int y);

	void translate(double x, double y, double z);

public:
	// set window width and height
	void setWindowSize(int width, int height);

	// set scene bounding box
	void setSceneBBox(const Vector3d& minPos, const Vector3d& maxPos);
	// get center of scene
	Vector3d getSceneCenter() const { return m_bbCenter; }
	// get scale of scene
	double getSceneScale() const { return m_bbScale; }

	// get zoom
	double getZoom() const { return m_zoom;	}

	// get rotate matrix
	double* getRotateMatrix() const { return (double*)m_arcBallMatrix; }

private:
	// viewpoint info
	int m_iWinWidth;
	int m_iWinHeight;
	double m_dAspect;

	// gl
	Vector3d m_center, m_right, m_up;
	double   m_distanceToEye;
	Vector3d m_eye;

	// scene bounding box center(translate) and scale
	Vector3d m_bbCenter; 
	double   m_bbScale;

	// scale
	double    m_zoom;
	const double m_zoomDefault;
	double m_translateX;
	double m_translateY;
	double m_translateZ;

	// rotate
	ArcBall*  m_pArcBall;
	ArcBall*  m_pArcBall_bak;
	HMatrix m_arcBallMatrix;
};

#endif // CAMERA_H
