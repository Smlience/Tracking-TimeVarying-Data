#ifndef ColorVolumeLightingRender_H
#define ColorVolumeLightingRender_H

#include "render/VolumeLightingRender.h"

class ColorVolumeLightingRender : public VolumeLightingRender
{
	Q_OBJECT
public:
	ColorVolumeLightingRender(QWidget *parent = NULL);
	~ColorVolumeLightingRender(void);
	virtual void initTextures();
	virtual void paintGL();
	virtual void initShader();

public slots:
	virtual void onDataLoaded(const Volume* v);

protected:
};

#endif // ColorVolumeLightingRender_H