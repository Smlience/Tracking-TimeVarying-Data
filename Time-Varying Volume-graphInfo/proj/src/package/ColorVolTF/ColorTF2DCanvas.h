#ifndef COLORTF2DCANVAS_H
#define COLORTF2DCANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPainter>
#include "ColorCluster.h"

class Volume;
class ClusteredColorTF;

class ColorTF2DCanvas : public QWidget
{
	Q_OBJECT

public:
	ColorTF2DCanvas(QWidget *parent);
	~ColorTF2DCanvas();
	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;
	virtual QSizePolicy sizePolicy() const;
	void setVolume(const Volume* vol);

signals:
	void sendTF(unsigned char*);
	void sendClusterInfo(unsigned char*);

protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual bool eventFilter(QObject *obj, QEvent *e);
	//	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	void setupTransfunc();

	void segment();

public slots:
	void clearTF();

private:
	std::list<QPoint> m_vPoint; /// �����õ����ݡ���
	ClusteredColorTF* m_tf;
	const ColorSample* m_clusters;
	QPoint m_pos[256];
	std::list< std::pair<int, int> > m_segment; /// ��cluster���ֳɵ����䣬ÿ��Ԫ�ذ����������ʼֵ����ֵֹ
	int padding;
};

#endif // COLORTF2DCANVAS_H
