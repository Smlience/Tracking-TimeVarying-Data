#ifndef GRADMAGTFCANVAS_H
#define GRADMAGTFCANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QPainter>
#include "util/Vector2.h"
#include "util/Vector4.h"

class TFMappingKey;
class TFMappingKeyCollection;
class Volume;
class TF1D;
class IntensityHistogram;
class HistogramPainter;

class GradMagTFCanvas : public QWidget
{
	Q_OBJECT

public:
	GradMagTFCanvas(QWidget* parent = NULL);
	~GradMagTFCanvas();
	void setGradMag(const Volume* vol);
	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;
	virtual QSizePolicy sizePolicy() const;

protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual bool eventFilter(QObject *obj, QEvent *e);
//	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);

signals:
	void changed();
	void resetTransferFunction();
	void sendTF(unsigned char*);

public slots:
	void resetTF();
	void deleteKey();
	void showTFCurve(bool show);
	void showHistogram(bool show);
	void saveTF();
	void loadTF();

protected:
	virtual void resizeEvent(QResizeEvent* event);
	void insertNewKey(Vector2f& hit);
	TFMappingKey* getOtherKey(TFMappingKey* selectedKey, bool chooseLeftPart);
	int hitLine(const Vector2f& p);
	void paintKeys(QPainter& paint);
	void drawMarker(QPainter& paint, const Color4& color, const Vector2f& p, int props = 0);
	void updateCoordinatesTooltip(QPoint pos, Vector2f values);
	void setupTransfunc();
	Color4 getMappingKeyValue(float value) const;

protected:
	Vector2f wtos(Vector2f p);
	Vector2f stow(Vector2f p);
	QColor Col2QColor(const Color4& color);
	Color4 QColor2Col(const QColor& color);

private:
	enum MarkerProps {
		MARKER_NORMAL   =  0,
		MARKER_SELECTED =  4
	};
	TF1D* m_tf;
	IntensityHistogram* m_pHistogram;
	HistogramPainter* m_pHistogramPainter;
	std::vector<TFMappingKey*> m_keys;
	int m_iMaximumIntensity;

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
	bool m_bShowTFCurve;

	///x轴名字
	QString m_xAxisText;
	///y轴名字
	QString m_yAxisText;
};

#endif // GRADMAGTFCANVAS_H