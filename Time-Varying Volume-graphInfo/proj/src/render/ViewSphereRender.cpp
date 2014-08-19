#include "ViewSphereRender.h"
#include "ext/Healpix/chealpix.h"
#include "util/Functions.h"
#include <set>

//////////////////////////////////////////////////////////////////////////

ViewPointRecorder::ViewPointRecorder():m_current(0)
{}

ViewPointRecorder::~ViewPointRecorder()
{
	m_vPoint.clear();
}

void ViewPointRecorder::clear()
{
	m_vPoint.clear();
	m_current = 0;
}

void ViewPointRecorder::push( const ViewPoint* p )
{
	m_vPoint.push_back(p);
}

const ViewPoint* ViewPointRecorder::begin() const
{
	if (m_vPoint.size() > 0)
		return m_vPoint.at(0);
	else
		return NULL;
}

const ViewPoint* ViewPointRecorder::end() const
{
	if (m_vPoint.size() > 0)
		return m_vPoint.at(m_vPoint.size() - 1);
	else
		return NULL;
}

vector<const ViewPoint*> ViewPointRecorder::all()
{
	return m_vPoint;
}

int ViewPointRecorder::current()
{
	return m_current;
}

const ViewPoint* ViewPointRecorder::next()
{
	const ViewPoint* p = m_vPoint.at(m_current + 1);
	m_current ++;
	return p;
}

bool ViewPointRecorder::hasNext()
{
	return m_current < (m_vPoint.size() - 1);
}

void ViewPointRecorder::deleteAll()
{
	for (vector<const ViewPoint*>::iterator it = m_vPoint.begin(); it != m_vPoint.end(); ++it)
	{
		delete (*it);
	}
	m_vPoint.clear();
}

//////////////////////////////////////////////////////////////////////////

ViewSphereRender::ViewSphereRender(QWidget *parent)
: QGLCanvas(parent), m_nside(32), m_currentPos(NULL), m_startPos(NULL), m_targetPos(NULL)
{
	resize(250, 250);
	m_recorder = new ViewPointRecorder;
	m_recorderArc = new ViewPointRecorder;
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(animate()));

	//m_tf = new unsigned char[256];
}

ViewSphereRender::~ViewSphereRender(void)
{
	m_timer->stop();
	m_recorderArc->deleteAll();
	SAFE_DELETE(m_recorderArc);
	SAFE_DELETE(m_recorder);
	m_currentPos = NULL;
	m_startPos = NULL;
	m_targetPos = NULL;
	//SAFE_DELETE_ARRAY(m_tf);
}

void ViewSphereRender::initializeGL()
{
	QGLCanvas::initializeGL();

	//在球面上绘制视点移动轨迹时需要，防止轨迹与球面重合出现z-fighting的情况
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);

	// Generate view position
	long npix = nside2npix(m_nside);
	double theta,phi;
	for(int i = 0; i < npix; ++i) {
		pix2ang_ring(m_nside, i, &theta, &phi);
		ViewPoint pos(theta, phi, 0.0);
		ang2vec(theta, phi, pos.coord);
		m_viewSphere.vViewPos.push_back(pos);
	}
	printf("View position count = %d\n", m_viewSphere.vViewPos.size());

	int array[8];
	//生成顶点之间的link
	for(int i = 0; i < npix; i++)
	{
		neighbors(m_nside, i, array, 0);
		for(int j = 0; j < 8; j += 2) {
			if(array[j] > i) 
				m_viewSphere.links.push_back(make_pair(i, array[j]));
		}
	}

	// Generate mesh from vertices
	// 生成球面模型
	vector<ViewPoint>& vp = m_viewSphere.vViewPos;
	m_viewSphere.vTris.push_back(Vector3i(0, 1, 2)); // 北极点的两个三角形
	m_viewSphere.vTris.push_back(Vector3i(0, 2, 3));
	m_viewSphere.vTris.push_back(Vector3i(npix - 1, npix - 2, npix - 3)); // 南极点的两个三角形
	m_viewSphere.vTris.push_back(Vector3i(npix - 1, npix - 3, npix - 4));
	for(int i = 0; i < npix; i++)
	{
		neighbors(m_nside, i, array, 0);

		if (array[4] >= 0 && array[5] >= 0 && array[6] >= 0)
		{
			m_viewSphere.vTris.push_back(Vector3i(i, array[6], array[5]));
			m_viewSphere.vTris.push_back(Vector3i(i, array[5], array[4]));
		} else {
			m_viewSphere.vTris.push_back(Vector3i(i, array[6], array[4]));
		}
 	}

// 	m_currentPos = &(m_viewSphere.vViewPos[0]);
// 
// 	long ipix;
// 	Vector4d p0(1.0, 0.5, 0.0, 1.0);
// 	vec2pix_ring(m_nside, (double*)&p0, &ipix);
// 	moveFromTo(m_currentPos, &(m_viewSphere.vViewPos[ipix]));
}

void ViewSphereRender::paintGL()
{
	m_camera.setupModelMatrix();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	float ps;
	glGetFloatv(GL_POINT_SIZE, &ps);
	vector<ViewPoint>& vp = m_viewSphere.vViewPos;
	glPointSize(5);
/*
	// 绘制顶点
	glColor3ub(255, 0, 0);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3dv(vp[0].coord);
	vector<ViewPoint>::iterator iter = vp.begin();
	for (; iter != vp.end(); ++iter)
	{
		glVertex3dv(iter->coord);
	}
	glEnd();
*/
	Vector4d p0(vp[0].coord[0], vp[0].coord[1], vp[0].coord[2], 1.0);

	double matrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
// 	GLint viewport[4];
// 	GLdouble projection[16];
// 	double win_coord[3]; 
// 	win_coord[0] = width() / 2;
// 	win_coord[1] = height() / 2;
// 	glReadPixels( int(win_coord[0]), int(win_coord[1]), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_coord[2] );
// 	glGetDoublev( GL_PROJECTION_MATRIX, projection );
// 	glGetIntegerv( GL_VIEWPORT, viewport );
// 	double pos[3];
// 	gluUnProject( win_coord[0], win_coord[1], win_coord[2], matrix, projection, viewport, &pos[0], &pos[1], &pos[2]);

// 	glPushMatrix();
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();
// 	glPopMatrix();

	p0.set(0, 0, 1.0, 1.0);
	invMatrix(matrix, matrix);
	p0 = multiply(p0, matrix);

	long ipix;
	vec2pix_ring(m_nside, (double*)&p0, &ipix);
	glColor3ub(255, 128, 0);
	glBegin(GL_POINTS);
		glVertex3dv(vp[ipix].coord);
	glEnd();
	m_currentPos = &(vp[ipix]);
/*
	glColor3ub(255, 255, 255);
	// 绘制顶点连线
	glBegin(GL_LINES);
	vector< pair<int, int> >::iterator itLink = m_viewSphere.links.begin();
	for(; itLink != m_viewSphere.links.end(); ++ itLink)
	{
		glVertex3dv(vp[itLink->first].coord);
		glVertex3dv(vp[itLink->second].coord);
	}
	glEnd();

	*/
	// 绘制球面
	vector<Vector3i>& vTris = m_viewSphere.vTris;
	vector<Vector3i>::iterator itTris = vTris.begin();
	glBegin(GL_TRIANGLES);
	for (; itTris != vTris.end(); ++itTris)
	{
		glColor3ub(128, 0, 0);
		glVertex3dv(vp[itTris->x].coord); 
		glColor3ub(0, 128, 0);
		glVertex3dv(vp[itTris->y].coord);
		glColor3ub(0, 0, 128);
		glVertex3dv(vp[itTris->z].coord);
	}
	glEnd();

	// 绘制视点变化曲线
	vector<const ViewPoint*> vPoint = m_recorder->all();
	if (vPoint.size() > 0)
	{
		glColor3ub(255, 255, 0);
		glBegin(GL_POINTS);
		glVertex3dv(vPoint.front()->coord);
		glEnd();
		if (vPoint.size() > 1)
		{
			glColor3ub(255, 102, 0);
			glBegin(GL_LINES);
			glVertex3dv(vPoint.front()->coord);
			for(int i = 0; i < m_recorder->current(); ++i)
			{
				glVertex3dv(vPoint[i]->coord);
				glVertex3dv(vPoint[i]->coord);
			}
			glVertex3dv(vPoint[m_recorder->current()]->coord);
			glEnd();

			glColor3ub(0, 255, 0);
			glBegin(GL_POINTS);
				glVertex3dv(vPoint.back()->coord);
			glEnd();
		}
	}
	//glDisable(GL_POLYGON_OFFSET_FILL);
	//////////////////////////////////////////////////////////////////////////

	glDisable(GL_DEPTH_TEST);
	//drawCoord();
	m_camera.setOrthoProjection();
	m_camera.drawArc();
	glEnable(GL_DEPTH_TEST);

 	glPointSize(ps);

}

void ViewSphereRender::updateGL()
{
	QGLCanvas::updateGL();
}

void ViewSphereRender::resizeGL( int w, int h )
{
	QGLCanvas::resizeGL(w, h);
}

void ViewSphereRender::animate()
{
	if (m_targetPos != NULL)
	{
		if (m_recorder->hasNext())
		{
			const ViewPoint* next = m_recorder->next();//nextPointNearest(m_currentPos, m_targetPos);
			if (next != NULL)
			{
				m_camera.lookAt(next->theta, next->phi);
				//////////////////////////////////////////////////////////////////////////
				updateTF();
				//////////////////////////////////////////////////////////////////////////
			}
		}
		else
		{
			m_timer->stop();
		}
		notifyUpdate();
	}
}

void ViewSphereRender::moveTo( ViewPoint* toPoint )
{
	m_recorder->clear();
	m_recorder->push(m_currentPos);
	m_recorderArc->clear();
	m_startPos = m_currentPos;
	m_targetPos = toPoint;

	//生成视点路径
	// 贪婪方法生成路径
	ViewPoint* p  = m_currentPos;
#define PATH_GENERATION_METHOD 3
#if PATH_GENERATION_METHOD == 1
	while(p != m_targetPos)
	{
		p = nextPointNearest(p, m_targetPos);
		m_recorder->push(p);
	}
#elif PATH_GENERATION_METHOD == 2
	// 累加经纬度角度的方法生成路径
	double delta_t = m_targetPos->theta - m_currentPos->theta; 
	double delta_p = m_targetPos->phi - m_currentPos->phi;
	double dt = delta_t / 100;
	double dp = delta_p / 100;
	double it = 0.0;
	double ip = 0.0;
	for (int i = 0; i < 100; i++)
	{
		long idx;
		ang2pix_ring(m_nside, m_currentPos->theta + it, m_currentPos->phi + ip, &idx);
		p = &(m_viewSphere.vViewPos[idx]);
		m_recorder->push(p);
		it += dt;
		ip += dp;
	}
#elif PATH_GENERATION_METHOD == 3
	generatePathArc(m_startPos, m_targetPos, m_recorderArc);
#endif
	//开始自动移动视点
	m_timer->start(50);
}

ViewPoint* ViewSphereRender::nextPointNearest( const ViewPoint* fromPoint, const ViewPoint* toPoint )
{
	int array[8];
	long fromIdx, toIdx;
	ang2pix_ring(m_nside, fromPoint->theta, fromPoint->phi, &fromIdx);
	neighbors(m_nside, fromIdx, array, 0);

	ViewPoint* result = NULL;
	ViewPoint* ptr = NULL;
	Vector3d p0(fromPoint->coord[0], fromPoint->coord[1], fromPoint->coord[2]);
	Vector3d p1(toPoint->coord[0], toPoint->coord[1], toPoint->coord[2]);
	Vector3d v0 = (p1 - p0).normalize();
	Vector3d v1;
	double maxCosValue = -1.0;
	double dotProduct;
	
	for (int i = 0; i < 8; ++i)
	{
		if (array[i] >= 0)
		{
			ptr = &(m_viewSphere.vViewPos[array[i]]);
			Vector3d p2(ptr->coord[0], ptr->coord[1], ptr->coord[2]);
			v1 = (p2 - p0).normalize();
			dotProduct = Dot(v0, v1);
// 			//用测地线距离来取最近点
// 			dotProduct2 = Dot(p1, p2); 
			if (dotProduct > maxCosValue)
			{
				maxCosValue = dotProduct;
				result = ptr;
			}
		}
	}
	return result;
}

void ViewSphereRender::generatePathArc( const ViewPoint* fromPoint, const ViewPoint* toPoint, ViewPointRecorder* recorder, int step)
{

	ViewPoint* p;
	Vector3d p1(fromPoint->coord[0], fromPoint->coord[1], fromPoint->coord[2]);
	Vector3d p0(toPoint->coord[0], toPoint->coord[1], toPoint->coord[2]);
	Vector3d p2;
	double cos_alpha = Dot(p0, p1);
	double alpha = acos(cos_alpha);
	double sin_alpha = sqrt(1 - cos_alpha * cos_alpha);

	if (step == -1)
		step = int(m_nside * alpha / 3.14159265358979323846 + 0.5) * 2;

	double delta = alpha / step;
	double beta;
	double gamma; // alpha - beta
	double theta;
	double phi;
	for (int i = 0; i < step; i++)
	{
		beta = delta * i;
		gamma = alpha - beta;
		p2 = (p0*sin(beta) + p1*sin(gamma)) / sin_alpha;
		vec2ang((double*)(&p2), &theta, &phi);
		p = new ViewPoint(theta, phi, 0.0);
		p->coord[0] = p2.x;
		p->coord[1] = p2.y;
		p->coord[2] = p2.z;
		m_recorder->push(p);
	}
}

void ViewSphereRender::setTF( unsigned char* data )
{
	unsigned char* tf = new unsigned char[256];
	memcpy(tf, data, 256);
	m_tfMap.insert(std::make_pair(m_currentPos, tf));
}

void ViewSphereRender::moveFromTo( ViewPoint* fromPoint, ViewPoint* toPoint )
{
	if (fromPoint == NULL || toPoint == NULL)
		return;

	m_currentPos = fromPoint;
	m_camera.lookAt(fromPoint->theta, fromPoint->phi);
	notifyUpdate();
	moveTo(toPoint);
}

void ViewSphereRender::updateTF()
{
	ViewPointRecorder* recorder = m_recorder;
	vector<const ViewPoint*> vPoints = recorder->all();
	if (vPoints.size() < 2)
		return;
	unsigned char (&tf0)[256][4] = m_startTF;//m_tfMap[const_cast<ViewPoint*>(vPoints[0])];
	unsigned char (&tf1)[256][4] = m_targetTF;//m_tfMap[m_targetPos];
	if (tf0 == NULL || tf1 == NULL)
		return;

	int i = recorder->current();
	float percent = float(i) / (vPoints.size() - 1);
	if (percent > 1.0)
		percent = 1.0;

	int idx;
	for (int j = 0; j < 256; ++j)
	{
		m_tf[j][0] = tf0[j][0] + (tf1[j][0] - tf0[j][0]) * percent;
		m_tf[j][1] = tf0[j][1] + (tf1[j][1] - tf0[j][1]) * percent;
		m_tf[j][2] = tf0[j][2] + (tf1[j][2] - tf0[j][2]) * percent;
		m_tf[j][3] = tf0[j][3] + (tf1[j][3] - tf0[j][3]) * percent;
	}
	printf("[%d]\n-----------------------------------------\n",i);
	emit sendTF(&(m_tf[0][0]));
}

void ViewSphereRender::setStartPosTF( unsigned char* data )
{
	memcpy(&(m_startTF[0][0]), data, 1024 * sizeof(unsigned char));
	//setTF(data);
	m_startPos = m_currentPos;
}

void ViewSphereRender::setEndPosTF( unsigned char* data )
{
	memcpy(&(m_targetTF[0][0]), data, 1024 * sizeof(unsigned char));
	//setTF(data);
	m_targetPos = m_currentPos;
}

void ViewSphereRender::moveViewpoint()
{
	moveFromTo(m_startPos, m_targetPos);
}
