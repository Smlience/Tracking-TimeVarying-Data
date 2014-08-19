#ifndef VARSELECT_MAINWIN_H
#define VARSELECT_MAINWIN_H

#include "ui/MainWin.h"

class VarSelect_MainWin : public MainWin
{
	Q_OBJECT

public:
	VarSelect_MainWin(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~VarSelect_MainWin();
	virtual void setupUi();
}; 

#endif // VARSELECT_MAINWIN_H
