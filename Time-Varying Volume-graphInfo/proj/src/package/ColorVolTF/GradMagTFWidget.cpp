#include "GradMagTFWidget.h"
#include "util/Vector3.h"
#include "dm/Volume.h"

GradMagTFWidget::GradMagTFWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//this->setWindowTitle("Gradient Magnitude Transfer Function");

	toolbar = new QWidget(this);

	btnReset = new QPushButton(toolbar);
	QIcon icon_reset(QStringLiteral(":/MainWin/default.png"));
	btnReset->setIcon(icon_reset);
	btnReset->setIconSize(QSize(16, 16));
	btnReset->setFixedSize(20, 20);
	btnReset->setToolTip(QApplication::translate("GradMagTFWidget", "Reset", 0));
	QObject::connect(btnReset, SIGNAL(clicked()), ui.canvas, SLOT(resetTF()));

	btnShowTFCurve = new QPushButton(toolbar);
	QIcon icon_tf_edit(QStringLiteral(":/MainWin/tf_edit.png"));
	btnShowTFCurve->setIcon(icon_tf_edit);
	btnShowTFCurve->setIconSize(QSize(16, 16));
	btnShowTFCurve->setFixedSize(20, 20);
	btnShowTFCurve->setCheckable(true);
	btnShowTFCurve->setChecked(true);
	btnShowTFCurve->setToolTip(QApplication::translate("GradMagTFWidget", "Show transfer function curve for editing", 0));
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
	btnSave->setToolTip(QApplication::translate("GradMagTFWidget", "Save transfer function to file", 0));
	QObject::connect(btnSave, SIGNAL(clicked()), ui.canvas, SLOT(saveTF()));

	btnLoad = new QPushButton(toolbar);
	QIcon icon_tf_load(QStringLiteral(":/MainWin/load.png"));
	btnLoad->setIcon(icon_tf_load);
	btnLoad->setIconSize(QSize(16, 16));
	btnLoad->setFixedSize(20, 20);
	btnLoad->setToolTip(QApplication::translate("GradMagTFWidget", "Load transfer function from file", 0));
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
}

GradMagTFWidget::~GradMagTFWidget()
{

}

// void GradMagTFWidget::on_canvas_resetTransferFunction()
// {
// 	emit tfReset();
// }
// 
// void GradMagTFWidget::on_canvas_changed()
// {
// 	emit tfChanged();
// }

void GradMagTFWidget::on_canvas_sendTF( unsigned char* data )
{
	emit sendTF(data);
}

void GradMagTFWidget::setGradMag( const Volume* volume )
{
	ui.canvas->setGradMag(volume);
}

void GradMagTFWidget::resizeEvent( QResizeEvent *e )
{
	//btnReset->move(5, e->size().height() - btnReset->height() - 5);
	//btnReset->move(e->size().width() - btnReset->width() - 5, 5);
	toolbar->move(e->size().width() - toolbar->width() - 5, 5);
}
