#ifndef EVOLUTIONGRAPHWIDGET_H
#define EVOLUTIONGRAPHWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "util\Vector2.h"
#include "util\Vector3.h"
#include <vector>
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPainter>
#include "util/Vector4.h"
#include "src/dm/dag.h"

using namespace std;
class TFMappingKey;
class TF1D;
class TFMappingKeyCollection;
class DAG;

class EvolutionGraphWidget : public QWidget
{
	Q_OBJECT

public:
	EvolutionGraphWidget(QWidget *parent = 0);
	~EvolutionGraphWidget();

	
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual bool eventFilter(QObject *obj, QEvent *e);
	//	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;
	virtual QSizePolicy sizePolicy() const;

	void setClusterTimeStep( int timestep );
	void setDataMin(float minData);
	void setDataMax(float maxData);
	void setTimeNum(int timestepsNum);
	
	virtual void mouseDoubleClickEvent(QMouseEvent* event);

	void changeCurrentColor();
	void changeCurrentColor(const QColor& c);

	void loadSequenceInfo();
	void initMappingKey();
	void setupTransfunc();

signals:
	void tfChanged(unsigned char* data, Vector2i dim);
	void changed();
	void sendTF(unsigned char*);
	void sendClusterInfo(unsigned char*);
	void sendVoxelCountInfo(int **);
	void sendSelectedSequence(std::vector<int>);
	void sendTimeNumSelected(int );
	void sendClusterNumSelected(int );

	void colorChanged(const QColor& c);

	public slots:
		void setFileDirName( char* fileDirName );
	//	void onDataLoaded();

protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	int hitLine(const Vector2f& p);
	Vector2i hitDot(const Vector2f& p);
	void drawMarker(QPainter& paint, const Color4& color, const Vector2f& p, int props = 0);
	void updateCoordinatesTooltip(QPoint pos, Vector2f values);

	Vector2f wtos(Vector2f p);
	Vector2f stow(Vector2f p);
	QColor Col2QColor(const Color4& color);
	Color4 QColor2Col(const QColor& color);

private:
	enum MarkerProps {
		MARKER_NORMAL   =  0,
		MARKER_SELECTED =  4
	};
	std::vector<TFMappingKey*> m_keys;
	int m_iMaximumIntensity;
	TF1D *m_pTransfunc;//1dtf数组

	//交互
	TFMappingKeyCollection* m_keyColletion;
	std::vector<TFMappingKeyCollection*> m_keyColletionFull;
	TFMappingKey* m_pSelectedKey;
	bool m_bIsDragging; //托动一个Key
	int m_iDragLine;
	int m_iDragLineStartY;
	float m_fDragLineAlphaLeft;
	float m_fDragLineAlphaRight;
	QPoint m_mousePos; //现在鼠标的位置

	//外观
	///widget的边界
	int m_iPadding;
	///下面显示数字
	int m_iBottomSpace;
	///坐标轴上箭头的长度
	int m_iArrowLength;
	///坐标轴上箭头的宽度
	int m_iArrowWidth;
	///网格的最小尺寸
	int m_iMinCellSize;
	///传输函数一个Key的大小
	int m_iPointSize;
	///x轴方向的范围
	Vector2f m_xRange;
	///y轴方向的范围
	Vector2f m_yRange;  
	///x,y轴的格子数目
	Vector2f m_gridSpacingNum;
	///网格的长宽
	Vector2f m_gridSpacing; 

	bool m_bIsClipThresholds;
	bool m_bIsNoColor;
	bool m_bIsDrawHist;
	///x轴名字
	QString m_xAxisText;
	///y轴名字
	QString m_yAxisText;


	//画聚类 所需信息
	unsigned char dataRange,m_dataMin,m_dataMax;
	int m_curTimeStep;
	int m_timeNum;
	int m_timeWindowWide;


	int m_clusterNum; //new data infomation needed
	std::vector<int> m_MoveSelected;
	int m_mouseMoveSelected;
	int m_sequenceSelected;
	int m_timeNumSelectted;
	int m_clusterNumSelected;
	Vector2i m_dotSelected;

	Vector2i m_startPoint;
	Vector2i m_endPoint;
	Vector2i m_dotAddSelected;

	Edge m_newAddEdge;

	int m_pathNum;

	unsigned char *m_arryNumFeature;
	int m_maxFeatureNum;

	char * m_fileDirName;
	DAG dag;

	bool dataLoad;
};

#endif // EvolutionGraphWidget
