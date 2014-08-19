#ifndef SLICEGRADIENTRENDER_H
#define SLICEGRADIENTRENDER_H

#include "ui/QGLCanvas.h"
#include "util/Vector3.h"
#include "util/Vector2.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include "ui_SliceGradientRenderControl.h"

class Volume;

class SliceGradientRender : public QWidget
{
	Q_OBJECT

public:
    SliceGradientRender(QWidget *parent = NULL);

protected:
    void paintEvent(QPaintEvent *e);
	void contextMenuEvent(QContextMenuEvent *e);
	void drawArrow(QPainter *p, const QPointF& p1, const QPointF& p2);
	void draw(QPaintDevice * device);

public slots:
	void on_sbLineWidth_valueChanged(int i);
	void on_sbLineLength_valueChanged(int i);
	void on_sbGradFilter_min_valueChanged(double d);
	void on_sbGradFilter_max_valueChanged(double d);
	void on_cbShowGM_stateChanged(int state);
	void on_cbShowImage_stateChanged(int state);
	void on_cbShowVector_stateChanged(int state);
	void on_sbSlice_valueChanged(int i);
	void on_cbDir_currentIndexChanged(int i);
    void showSlice(int dir, int slice);
    void setVolume(const Volume* v);
    void setGradient(unsigned char *g, unsigned char* gm);
	void saveImage();
	void screenshot();

private:
    typedef enum {XOY = 2, XOZ = 1, YOZ = 0} SliceDir;
    const Volume *m_volume;
    unsigned char *m_gradient;
	unsigned char *m_gradientMag;
//	float m_maxgm;
    int m_slice;
    SliceDir m_dir;
	QImage *m_pix;		 /// Slice图像
	QImage *m_gmpix;	 /// 
	Vector2f *m_gradMap; /// 存储每个slice上的点对应的梯度向量
	Vector3f m_vNormDim;
	Vector2f m_sNormDim;
	Vector2i m_sDim;
	Vector2i m_elementIdx;
	QAction *m_actSaveImg;
	QAction *m_actScreenshot;
	int m_gradArrowLength;
	int m_lineWidth;
// 	QSpinBox* m_sbSlice;
// 	QComboBox* m_cbDir;
// 	QSpinBox* m_sbLineWidth;
// 	QSpinBox* m_sbLineLength;
// 	QDoubleSpinBox* m_sbGradFilter_min;
// 	QDoubleSpinBox* m_sbGradFilter_max;
// 	QCheckBox* m_cbShowGM;
// 	QCheckBox* m_cbShowImage;
// 	QCheckBox* m_cbShowVector;
	QWidget* m_control;
	Vector2f m_gradFilter;
	int m_showGM;
	int m_showImage;
	int m_showVector;
	Ui::SliceGradientRenderControl ui;
};

#endif // SLICEGRADIENTRENDER_H