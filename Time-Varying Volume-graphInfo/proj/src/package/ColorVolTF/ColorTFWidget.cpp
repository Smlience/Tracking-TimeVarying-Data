#include "ColorTFWidget.h"
#include "util/Vector3.h"
#include "dm/Volume.h"

ColorTFWidget::ColorTFWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.widget->setVisible(false);

	toolbar = new QWidget(this);

	btnReset = new QPushButton(toolbar);
	QIcon icon_reset(QStringLiteral(":/MainWin/default.png"));
	btnReset->setIcon(icon_reset);
	btnReset->setIconSize(QSize(16, 16));
	btnReset->setFixedSize(20, 20);
	btnReset->setToolTip(QApplication::translate("ColorTFWidget", "Reset", 0));
	QObject::connect(btnReset, SIGNAL(clicked()), ui.canvas, SLOT(resetTF()));

	btnShowTFCurve = new QPushButton(toolbar);
	QIcon icon_tf_edit(QStringLiteral(":/MainWin/tf_edit.png"));
	btnShowTFCurve->setIcon(icon_tf_edit);
	btnShowTFCurve->setIconSize(QSize(16, 16));
	btnShowTFCurve->setFixedSize(20, 20);
	btnShowTFCurve->setCheckable(true);
	btnShowTFCurve->setChecked(true);
	btnShowTFCurve->setToolTip(QApplication::translate("ColorTFWidget", "Show transfer function curve for editing", 0));
	QObject::connect(btnShowTFCurve, SIGNAL(toggled(bool)), ui.canvas, SLOT(showTFCurve(bool)));

	btnShowHistogram = new QPushButton(toolbar);
	QIcon icon_histogram(QStringLiteral(":/MainWin/histogram.png"));
	btnShowHistogram->setIcon(icon_histogram);
	btnShowHistogram->setIconSize(QSize(16, 16));
	btnShowHistogram->setFixedSize(20, 20);
	btnShowHistogram->setCheckable(true);
	btnShowHistogram->setChecked(true);
	btnShowHistogram->setToolTip(QApplication::translate("ColorTFWidget", "Show histogram", 0));
	QObject::connect(btnShowHistogram, SIGNAL(toggled(bool)), ui.canvas, SLOT(showHistogram(bool)));

	btnSave = new QPushButton(toolbar);
	QIcon icon_tf_save(QStringLiteral(":/MainWin/save.png"));
	btnSave->setIcon(icon_tf_save);
	btnSave->setIconSize(QSize(16, 16));
	btnSave->setFixedSize(20, 20);
	btnSave->setToolTip(QApplication::translate("ColorTFWidget", "Save transfer function to file", 0));
	QObject::connect(btnSave, SIGNAL(clicked()), ui.canvas, SLOT(saveTF()));

	btnLoad = new QPushButton(toolbar);
	QIcon icon_tf_load(QStringLiteral(":/MainWin/load.png"));
	btnLoad->setIcon(icon_tf_load);
	btnLoad->setIconSize(QSize(16, 16));
	btnLoad->setFixedSize(20, 20);
	btnLoad->setToolTip(QApplication::translate("ColorTFWidget", "Load transfer function from file", 0));
	QObject::connect(btnLoad, SIGNAL(clicked()), ui.canvas, SLOT(loadTF()));

	QVBoxLayout *vLayout = new QVBoxLayout(toolbar);
	vLayout->setSpacing(2);
	vLayout->setContentsMargins(0, 0, 0, 0);
	vLayout->addWidget(btnReset);
	vLayout->addWidget(btnShowTFCurve);
	vLayout->addWidget(btnShowHistogram);
	vLayout->addWidget(btnSave);
	vLayout->addWidget(btnLoad);
	toolbar->resize(20, vLayout->count() * (20 + vLayout->spacing()) - vLayout->spacing());
//  // TIFF Image to Raw Data
// 	QImage img("A_VM1625.TIF", "TIFF");
// 	FILE* f = fopen("test.raw", "wb");
// 	unsigned char* data = new unsigned char[img.width() * 3];
// 	for (int i = 0; i < img.height(); ++i)
// 	{
// 		for (int j = 0; j < img.width(); ++j)
// 		{
// 			QRgb pix = img.pixel(j, i);
// 			int idx = j * 3;
// 			data[idx + 0] = (pix & 0x00ff0000) >> 16;
// 			data[idx + 1] = (pix & 0x0000ff00) >> 8;
// 			data[idx + 2] = (pix & 0x000000ff);
// 		}
// 		fwrite(data, sizeof(unsigned char), img.width() * 3, f);
// 	}
// // 	size_t w = fwrite(data, sizeof(unsigned char), img.width() * img.height() * 3, f);
// // 	printf("size:%d %d\n", img.width(), img.height());
// 	fclose(f);
// // 	QImage img2(data, img.width(), img.height(), QImage::Format_RGB888);
// // 	img2.save("test.png");
//  	delete []data;
// // 	printf(">> %d/%d <<\n", img.width() * img.height() * 3, w);
}

ColorTFWidget::~ColorTFWidget()
{

}

// void ColorTFWidget::on_canvas_resetTransferFunction()
// {
// 	emit tfReset();
// }
// 
// void ColorTFWidget::on_canvas_changed()
// {
// 	emit tfChanged();
// }

void ColorTFWidget::on_canvas_sendTF( unsigned char* data )
{
	emit sendTF(data);
}

void ColorTFWidget::on_canvas_sendClusterInfo( unsigned char* data )
{
	emit sendClusterInfo(data);
}

void ColorTFWidget::setVolume( const Volume* volume )
{
	ui.canvas->setVolume(volume);
}

// void ColorTFWidget::on_cbMode_currentIndexChanged( int index )
// {
// 	emit changeRenderMode(index);
// }
// 
// void ColorTFWidget::on_cbGradient_currentIndexChanged( int index )
// {
// 	emit changeGradientMode(index);
// }

void ColorTFWidget::resizeEvent( QResizeEvent *e )
{
	//btnReset->move(5, e->size().height() - btnReset->height() - 5);
	//btnReset->move(e->size().width() - btnReset->width() - 5, 5);
	toolbar->move(e->size().width() - toolbar->width() - 5, 5);
}
