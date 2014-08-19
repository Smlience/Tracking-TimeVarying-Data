#ifndef COLORTFCANVAS_H
#define COLORTFCANVAS_H

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
class ClusteredColorTF;
class HistogramPainter;
class ColorBar;

class ColorTFCanvas : public QWidget
{
	Q_OBJECT

public:
	ColorTFCanvas(QWidget* parent = NULL);
	~ColorTFCanvas();
	void setVolume(const Volume* vol);
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
	void sendClusterInfo(unsigned char*);

public slots:
	void resetTF();
	void deleteKey();
	void mousePressedAt(int idx);
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
	
	/**
	 * @brief ��ѡ�����ε���ɫ���ɸ�˹����TF
	 * @param [in] cidx int ѡ����ɫ
	 * @param [in] sidx int ������ʼ��ɫ
	 * @param [in] range int ���ε���ɫ����
	 * @date 2013/04/11
	 */
	void genGaussTF(int cidx, int sidx, int range);

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
	ClusteredColorTF* m_tf;
	HistogramPainter* m_pHistogramPainter;
	ColorBar* m_colorBar;
	std::vector<TFMappingKey*> m_keys;
	int m_iMaximumIntensity;

	//����
	TFMappingKeyCollection* m_keyColletion;
	TFMappingKey* m_pSelectedKey;
	bool m_bIsDragging; //�ж�һ��Key
	int m_iDragLine;
	int m_iDragLineStartY;
	float m_fDragLineAlphaLeft;
	float m_fDragLineAlphaRight;
	QPoint m_mousePos; //��������λ��

	//���
	///widget�ı߽�
	int m_iPadding;
	///������ʾ����
	int m_iBottomSpace;
	///�������ϼ�ͷ�ĳ���
	int m_iArrowLength;
	///�������ϼ�ͷ�Ŀ���
	int m_iArrowWidth;
	///�������С�ߴ�
	int m_iMinCellSize;
	///���亯��һ��Key�Ĵ�С
	int m_iPointSize;
	///x�᷽��ķ�Χ
	Vector2f m_xRange;
	///y�᷽��ķ�Χ
	Vector2f m_yRange;  
	///x,y��ĸ�����Ŀ
	Vector2f m_gridSpacingNum;
	///����ĳ���
	Vector2f m_gridSpacing; 

	bool m_bIsClipThresholds;
	bool m_bIsNoColor;
	bool m_bIsDrawHist;
	bool m_bShowTFCurve;
	bool m_bColorbarSortable;

	///x������
	QString m_xAxisText;
	///y������
	QString m_yAxisText;
};

#endif // COLORTFCANVAS_H