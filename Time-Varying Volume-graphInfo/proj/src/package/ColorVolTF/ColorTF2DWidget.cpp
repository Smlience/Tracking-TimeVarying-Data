#include "ColorTF2DWidget.h"

ColorTF2DWidget::ColorTF2DWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ColorTF2DWidget::~ColorTF2DWidget()
{

}

void ColorTF2DWidget::on_canvas_sendTF( unsigned char* data )
{
	emit sendTF(data);
}

void ColorTF2DWidget::on_canvas_sendClusterInfo( unsigned char* data )
{
	emit sendClusterInfo(data);
}

void ColorTF2DWidget::setVolume( const Volume* volume )
{
	ui.canvas->setVolume(volume);
}

void ColorTF2DWidget::on_cbMode_currentIndexChanged( int index )
{
	emit changeRenderMode(index);
}
