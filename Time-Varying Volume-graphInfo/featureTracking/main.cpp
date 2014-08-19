#include "featuretracking.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	featureTracking w;
	w.init();
	w.show();
	return a.exec();
}
