#ifndef COLORSIMPLE_MAINWIN_H
#define COLORSIMPLE_MAINWIN_H

#include "ui/MainWin.h"

class ColorSimple_MainWin : public MainWin
{
	Q_OBJECT

public:
	ColorSimple_MainWin(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ColorSimple_MainWin();
	virtual void setupUi();
}; 

#endif // COLORSIMPLE_MAINWIN_H
