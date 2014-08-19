#ifndef TF1DCANVAS_H
#define TF1DCANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include "util/Vector2.h"
#include "util/Vector4.h"

class TFMappingKey;
class TF1D;
class TFMappingKeyCollection;
class Volume;
class HistogramPainter;
class IntensityHistogram;

class TF1DCanvas : public QWidget
{
	Q_OBJECT

public:
	TF1DCanvas(QWidget *parent = 0);
	~TF1DCanvas();

	virtual void paintEvent(QPaintEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual bool eventFilter(QObject *obj, QEvent *e);
	void setThreshold(float l, float u);
	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;
	virtual QSizePolicy sizePolicy() const;
	void setTransFunc(TFMappingKeyCollection* tf);
	void setXAxisText(const std::string& text);
	void setYAxisText(const std::string& text);

signals:
	void colorChanged(const QColor& c);
	void changed();
	void resetTransferFunction();
	void toggleInteractionMode(bool on);
	void sendTF(unsigned char*);

public slots:
	void deleteKey();
	void resetTF();
	void changeCurrentColor();
	void changeCurrentColor(const QColor& c);
	void toggleClipThresholds(bool enabled);
	void setHistogram(IntensityHistogram* histogram);
	void load();
	void save();

protected:
	void insertNewKey(Vector2f& hit);
	TFMappingKey* getOtherKey(TFMappingKey* selectedKey, bool chooseLeftPart);
	void paintKeys(QPainter& paint);
	int hitLine(const Vector2f& p);
	void drawMarker(QPainter& paint, const Color4& color, const Vector2f& p, int props = 0);
	virtual void resizeEvent(QResizeEvent* event);

	Vector2f wtos(Vector2f p);

	Vector2f stow(Vector2f p);

	QColor Col2QColor(const Color4& color);
	Color4 QColor2Col(const QColor& color);
	void hideCoordinates();
	void updateCoordinates(QPoint pos, Vector2f values);
	void setupTransfunc();
	Color4 getMappingKeyValue(float value) const;

protected:
	enum MarkerProps {
		MARKER_NORMAL   =  0,
		MARKER_SELECTED =  4
	};

	TF1D *m_pTransfunc;//1dtf数组
	std::vector<TFMappingKey*> m_keys;
	int m_iMaximumIntensity;

	float m_fThresholdL;
	float m_fThresholdU;

	//交互
	TFMappingKeyCollection* m_keyColletion;
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

	bool m_bIsChanged;

	HistogramPainter* m_pHistogramPainter;
};

#endif // TF1DCANVAS_H
