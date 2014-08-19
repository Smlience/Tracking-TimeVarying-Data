#ifndef SCATTERPLOTMATRIX_H
#define SCATTERPLOTMATRIX_H

#include <QWidget>

class ScatterPlotMatrix : public QWidget
{
	Q_OBJECT

public:
	ScatterPlotMatrix(QWidget *parent = 0);
	~ScatterPlotMatrix();
protected:
	void paintEvent(QPaintEvent *e);
private:

};

#endif // SCATTERPLOTMATRIX_H