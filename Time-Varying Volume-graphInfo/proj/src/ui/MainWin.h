#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>

#define CREATE_APP_WIN(W) W w;w.init();w.show();

class MainWin : public QMainWindow
{
	Q_OBJECT

public:
	MainWin(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~MainWin();
	virtual void init();
	virtual void setupUi();
	virtual void setupMenu();

public slots:
	void on_actionOpen_triggered();
//	void on_tfWidget_sendTF(unsigned char* data);

protected:
	QDockWidget* addDock(QWidget* pWidget, Qt::DockWidgetArea area = Qt::RightDockWidgetArea);

private:
}; 

#endif // MAINWIN_H
