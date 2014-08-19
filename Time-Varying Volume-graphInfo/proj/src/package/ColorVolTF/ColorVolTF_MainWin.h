#ifndef COLORVOLTF_MAINWIN_H
#define COLORVOLTF_MAINWIN_H

#include "ui/MainWin.h"

class ColorVolTF_MainWin : public MainWin
{
	Q_OBJECT

public:
	ColorVolTF_MainWin(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ColorVolTF_MainWin();
	virtual void setupUi();
}; 

#endif // COLORVOLTF_MAINWIN_H
