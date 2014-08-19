#ifndef LIGHTPARAMETERWIDGET_H
#define LIGHTPARAMETERWIDGET_H

#include <QWidget>
#include "ui_LightParameterWidget.h"
#include "util/Vector3.h"

class VolumeLightingRender;

class LightParameterWidget : public QWidget
{
	Q_OBJECT

public:
	LightParameterWidget(QWidget *parent = 0);
	~LightParameterWidget();
	void connectRender(VolumeLightingRender* pRender);

signals:
	void lightPositionChanged(Vector3d);

private slots:
	void on_lp_x_valueChanged(double v);
	void on_lp_y_valueChanged(double v);
	void on_lp_z_valueChanged(double v);

private:
	Ui::LightParameterWidget ui;
};

#endif // LIGHTPARAMETERWIDGET_H
