#include "LightParameterWidget.h"
#include "src/render/VolumeLightingRender.h"

LightParameterWidget::LightParameterWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

LightParameterWidget::~LightParameterWidget()
{}

void LightParameterWidget::connectRender( VolumeLightingRender* pRender )
{
	ui.sb_ambient->setValue(pRender->ka());
	ui.sb_diffuse->setValue(pRender->kd());
	ui.sb_specular->setValue(pRender->ks());
	ui.sb_shininess->setValue(pRender->shininess());
	Vector3f lp = pRender->lightPosition();
	ui.lp_x->setValue(lp.x);
	ui.lp_y->setValue(lp.y);
	ui.lp_z->setValue(lp.z);
	connect(ui.sb_ambient, SIGNAL(valueChanged(double)), pRender, SLOT(setKa(double)));
	connect(ui.sb_diffuse, SIGNAL(valueChanged(double)), pRender, SLOT(setKd(double)));
	connect(ui.sb_specular, SIGNAL(valueChanged(double)), pRender, SLOT(setKs(double)));
	connect(ui.sb_shininess, SIGNAL(valueChanged(double)), pRender, SLOT(setShininess(double)));
	connect(this, SIGNAL(lightPositionChanged(Vector3d)), pRender, SLOT(setLightPos(Vector3d)));
	connect(ui.cb_shader, SIGNAL(currentIndexChanged(int)), pRender, SLOT(setRenderMode(int)));
	connect(ui.cb_gradient, SIGNAL(currentIndexChanged(int)), pRender, SLOT(setGradientMode(int)));
}

void LightParameterWidget::on_lp_x_valueChanged( double v )
{
	emit lightPositionChanged(Vector3d(v, ui.lp_y->value(), ui.lp_z->value()));
}

void LightParameterWidget::on_lp_y_valueChanged( double v )
{
	emit lightPositionChanged(Vector3d(ui.lp_x->value(), v, ui.lp_z->value()));
}

void LightParameterWidget::on_lp_z_valueChanged( double v )
{
	emit lightPositionChanged(Vector3d(ui.lp_x->value(), ui.lp_y->value(), v));
}
