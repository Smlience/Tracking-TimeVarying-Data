#ifndef VIEWBALL_H
#define VIEWBALL_H

#include "ui/QGLCanvas.h"
#include "util/Vector3.h"
#include <vector>
#include <QTimer>
using namespace std;

typedef struct ViewPoint {
	double theta;
	double phi;
	double coord[3];
	double metric;
	ViewPoint(){};
	ViewPoint(double t, double p, double metric_):theta(t),phi(p),metric(metric_){};
	bool operator < (const ViewPoint& r) const {
		return theta == r.theta ? (phi < r.phi) : (theta < r.theta);
	}
}ViewPoint;

struct ViewSphere {
	vector<ViewPoint> vViewPos;
	vector<Vector3i> vTris;
	vector<size_t> orders;
	vector< pair<int, int> > links;
};

class ViewPointRecorder
{
public:
	ViewPointRecorder();
	~ViewPointRecorder();
	void clear();
	void deleteAll();
	void push(const ViewPoint* p);
	const ViewPoint* begin() const;
	const ViewPoint* end() const;
	vector<const ViewPoint*> all();
	int current();
	const ViewPoint* next();
	bool hasNext();

private:
	vector<const ViewPoint*> m_vPoint;
	int m_current;
};

class ViewSphereRender : public QGLCanvas
{
	Q_OBJECT

public:
	ViewSphereRender(QWidget *parent = NULL);
	~ViewSphereRender(void);
	virtual void initializeGL();
	virtual void paintGL();
	virtual void updateGL();
	virtual void resizeGL(int w, int h);
	void moveTo(ViewPoint* toPoint);
//	void setTargetPoint(double theta, double phi);
	void moveFromTo(ViewPoint* fromPoint, ViewPoint* toPoint);
	//////////////////////////////////////////////////////////////////////////
	void setTF(unsigned char* data);
	void updateTF();
signals:
	void sendTF(unsigned char* data);

public slots:
	void setStartPosTF(unsigned char* data);
	void setEndPosTF(unsigned char* data);
	void moveViewpoint();
	//////////////////////////////////////////////////////////////////////////

public slots:
	void animate();

private:
	/**
	 * @brief 找到从fromPoint到toPoint路径上的下一个点
	 *
	 * 找下一个点的方法：令从fromPoint到toPoint的向量为v1,获取fromPoint的8个邻域点，
	 * 计算得出fromPoint到这8个点向量与v1夹角最小的邻域点即为所求
	 */
	ViewPoint* nextPointNearest(const ViewPoint* fromPoint, const ViewPoint* toPoint);

	//ViewPoint* nextPointWeighted(const ViewPoint* fromPoint, const ViewPoint* toPoint);

	/**
	 * @brief 生成从fromPoint到toPoint的路径，该路径为两点所在大圆的圆弧
	 */
	void generatePathArc(const ViewPoint* fromPoint, const ViewPoint* toPoint, ViewPointRecorder* recorder, int step = -1);

private:
	long m_nside;
	ViewSphere m_viewSphere;
	QTimer *m_timer;
	ViewPoint* m_currentPos;
	ViewPoint* m_startPos;
	ViewPoint* m_targetPos;
	ViewPointRecorder* m_recorder;
	ViewPointRecorder* m_recorderArc;
	std::map<ViewPoint*, unsigned char*> m_tfMap;
	unsigned char m_tf[256][4];
	unsigned char m_startTF[256][4];
	unsigned char m_targetTF[256][4];
};

#endif // VIEWBALL_H
